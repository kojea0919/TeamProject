// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/BaseCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/CapsuleComponent.h"
#include "EffectObjectPool/BaseEffectActor.h"
#include "EffectObjectPool/EffectObjectPoolSubSystem.h"
#include "GameFrameWork/MainMap/MainMapGameMode.h"
#include "GameFrameWork/MainMap/MainMapPlayerController.h"
#include "GameTag/STGamePlayTags.h"
#include "Net/UnrealNetwork.h"
#include "Player/Character/AbilitySystem/STAbilitySystemComponent.h"
#include "Player/Character/AbilitySystem/Attributes/STAttributeSet.h"
#include "Player/Character/Component/STExtensionComponent.h"
#include "Player/Character/Component/Repel/RepelComponent.h"
#include "Player/Character/Data/CharacterClassInfo.h"
#include "Player/Character/Libraries/STAbilitySystemLibrary.h"
#include "Player/Character/Libraries/STFunctionLibrary.h"
#include "Player/Character/PlayerState/STPlayerState.h"
#include "UI/MainHUD/StaminaBar.h"
#include "UI/MainHUD/HealthBar.h"
#include "UI/MainHUD/PlayerMainHUD.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	SetActorTickInterval(0.5f);

	GetMesh()->bReceivesDecals = false;
	GetMesh()->SetIsReplicated(true);

	ExtensionComponent = CreateDefaultSubobject<USTExtensionComponent>(TEXT("ExtensionComponent"));

	GetMesh()->SetOnlyOwnerSee(false);
	GetMesh()->SetOwnerNoSee(false);
	GetMesh()->SetVisibility(true, true);
	GetMesh()->SetHiddenInGame(false);
	SetActorHiddenInGame(false);

	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

	NetUpdateFrequency = 100.f;
	MinNetUpdateFrequency = 66.f;
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (HasAuthority())
	{
		InitAbilityActorInfo();			
	}
	
		
}

void ABaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();

	if (IsLocallyControlled())
	{
		RegisterAttributeSetInHUD();
	}

}

void ABaseCharacter::RegisterAttributeSetInHUD()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController) return;

	AMainMapPlayerController* MyController = Cast<AMainMapPlayerController>(PlayerController);
	if (!MyController) return;

	if (UPlayerMainHUD* MainHUD = MyController->GetPlayerMainHUD())
	{
		MainHUD->InitializeHUD(MyController);
	}
	
}
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(STAbilitySystemComponent) && GetPlayerState())
	{
		InitAbilityActorInfo();
	}	
}


UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return ExtensionComponent ? ExtensionComponent->GetAbilitySystemComponent() : nullptr;
}

USTAttributeSet* ABaseCharacter::GetAttributeSet() const
{
	return STAttributes;
}

void ABaseCharacter::SetActive_Implementation(bool Active)
{
	SetActorHiddenInGame(!Active);
	SetActorEnableCollision(Active);
	SetActorTickEnabled(Active);

	if (Active)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	}
	else
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::AttachActorToComponent_Replicated(AActor* TargetActor, USceneComponent* InParentComponent,
	FName SocketName)
{
	if (!HasAuthority()) return;
	

	if (TargetActor && InParentComponent)
	{
		TargetActor->AttachToComponent(InParentComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);

		AttachData.TargetActor = TargetActor;
		AttachData.AttachSocket = SocketName;
		AttachData.ParentComponentName = InParentComponent->GetFName();
		AttachData.bAttached = true;
		
	}
}

void ABaseCharacter::OnRep_AttachData()
{
	if (!AttachData.bAttached || !AttachData.TargetActor)
	{
		return;
	}

	USceneComponent* ParentComp = GetMesh();

	if (AttachData.ParentComponentName.IsNone())
	{
		TArray<USceneComponent*> Components;
		GetComponents(Components);

		for (USceneComponent* Comp : Components)
		{
			if (Comp && Comp->GetFName() == AttachData.ParentComponentName)
			{
				ParentComp = Comp;
				break;
			}
		}
	}
	if (ParentComp)
	{
		AttachData.TargetActor->AttachToComponent(ParentComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachData.AttachSocket);
		
	}
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, AttachData);
	DOREPLIFETIME(ABaseCharacter, bIsDead);
}

void ABaseCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (HasAuthority() && !bIsDead && Data.NewValue <= 0.0f)
	{
		OnDied_Server();
	}
}

void ABaseCharacter::OnRep_IsDead()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_IsDead() → LocallyControlled=%d, Name=%s"), IsLocallyControlled(), *GetName());
	
	if (bIsDead)
	{
		UAbilitySystemComponent* AbilitySystemComponent = USTFunctionLibrary::NativeGetParentAbilitySystemComponentFromActor(this);
		if (AbilitySystemComponent != nullptr)
		{
			FGameplayEventData EventData;
			EventData.Instigator = this;
			EventData.Target = this;
			EventData.EventTag = STGamePlayTags::Player_Runner_Event_Dead;

			AbilitySystemComponent->HandleGameplayEvent(STGamePlayTags::Player_Runner_Event_Dead, &EventData);

			//UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, STGamePlayTags::Player_Runner_Event_Dead, EventData);
				
		}
		
		// if (ASTPlayerState* STPlayerState = GetPlayerState<ASTPlayerState>())
		// {
		// 	STAbilitySystemComponent = STPlayerState->GetSTAbilitySystemComponent();
		// 	STAbilitySystemComponent->RegisterGameplayTagEvent(STGamePlayTags::Player_Runner_Status_Dead, EGameplayTagEventType::AnyCountChange);
		// }
		//PlayLocalDeathMontage();
	}
}

void ABaseCharacter::OnDied_Server_Implementation()
{
	if (bIsDead) return;
	bIsDead = true;

	if (bIsDead && HasAuthority())
	{
		UAbilitySystemComponent* AbilitySystemComponent = USTFunctionLibrary::NativeGetParentAbilitySystemComponentFromActor(this);
		if (AbilitySystemComponent != nullptr)
		{
			FGameplayEventData EventData;
			EventData.Instigator = this;
			EventData.Target = this;
			EventData.EventTag = STGamePlayTags::Player_Runner_Event_Dead;

			AbilitySystemComponent->HandleGameplayEvent(STGamePlayTags::Player_Runner_Event_Dead, &EventData);
			
			//UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, STGamePlayTags::Player_Runner_Event_Dead, EventData);
				
		}
		//USTFunctionLibrary::AddTagToActor(this,STGamePlayTags::Player_Runner_Status_Dead);
		// if (ASTPlayerState* STPlayerState = GetPlayerState<ASTPlayerState>())
		// {
		// 	STAbilitySystemComponent = STPlayerState->GetSTAbilitySystemComponent();
		// 	STAbilitySystemComponent->RegisterGameplayTagEvent(STGamePlayTags::Player_Runner_Status_Dead, EGameplayTagEventType::AnyCountChange);
		// }
		//PlayLocalDeathMontage();
	}
}


void ABaseCharacter::Multicast_PlayDeathMontage_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("[Multicast] Called on %s | LocallyControlled=%d | Authority=%d"),
		*GetName(), IsLocallyControlled(), HasAuthority());

	if (!DeathMontage)
	{
		UE_LOG(LogTemp, Error, TEXT("[Multicast] DeathMontage is null"));
		HandleDeathFinished();
		return;
	}

	USkeletalMeshComponent* SkelMesh = GetMesh();
	if (!SkelMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("[Multicast] GetMesh() failed"));
		HandleDeathFinished();
		return;
	}

	// 클라이언트 본인은 이미 재생했을 가능성 있음
	if (IsLocallyControlled())
	{
		UE_LOG(LogTemp, Warning, TEXT("[Multicast] Skipping self-replay on LocallyControlled client: %s"), *GetName());
		return;
	}

	// 메시 강제 렌더링 재설정
	SkelMesh->SetVisibility(true, true);
	SkelMesh->SetHiddenInGame(false);
	SkelMesh->bOnlyOwnerSee = false;
	SkelMesh->bOwnerNoSee = false;
	SkelMesh->SetComponentTickEnabled(true);
	SkelMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

	// 강제 메시 갱신
	SkelMesh->MarkRenderDynamicDataDirty();
	SkelMesh->RefreshBoneTransforms();
	SkelMesh->RefreshFollowerComponents();
	SkelMesh->UpdateComponentToWorld();
	SkelMesh->TickComponent(0.f, LEVELTICK_All, nullptr);

	if (!SkelMesh->GetAnimInstance())
	{
		SkelMesh->InitAnim(true);
		UE_LOG(LogTemp, Warning, TEXT("[Multicast] AnimInstance was null, InitAnim called"));
	}

	UAnimInstance* Anim = SkelMesh->GetAnimInstance();
	if (!Anim)
	{
		UE_LOG(LogTemp, Error, TEXT("[Multicast] AnimInstance still null after InitAnim"));
		HandleDeathFinished();
		return;
	}

	const float Duration = Anim->Montage_Play(DeathMontage, 1.f);
	UE_LOG(LogTemp, Warning, TEXT("[Multicast] Montage_Play called on %s | Duration: %.2f"), *GetName(), Duration);

	if (Duration > 0.f)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&ABaseCharacter::HandleDeathFinished,
			Duration,
			false
		);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[Multicast] Montage_Play failed (Duration = 0)"));
		HandleDeathFinished();
	}
}

void ABaseCharacter::Server_TriggerDeath_Implementation()
{
	Multicast_PlayDeathMontage();
}

void ABaseCharacter::PlayLocalDeathMontage()
{
	USkeletalMeshComponent* SkelMesh = GetMesh();
	if (!SkelMesh || !DeathMontage) return;

	SkelMesh->SetVisibility(true, true);
	SkelMesh->SetHiddenInGame(false);
	SkelMesh->SetComponentTickEnabled(true);
	SkelMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	SkelMesh->bEnableUpdateRateOptimizations = false;

	if (!SkelMesh->GetAnimInstance())
		SkelMesh->InitAnim(true);

	UAnimInstance* Anim = SkelMesh->GetAnimInstance();
	if (!Anim) return;

	if (Anim)
	{		
		if (UAnimMontage* CurMontage = Anim->GetCurrentActiveMontage())
		{
			UE_LOG(LogTemp, Warning, TEXT("Current Montage: %s"), *CurMontage->GetName());
		}
	}
	Anim->Montage_Stop(0.1f);

	float Duration = Anim->Montage_Play(DeathMontage, 1.f);
	UE_LOG(LogTemp, Warning, TEXT("Montage_Play by OnRep → Name: %s | Duration: %.2f"), *GetName(), Duration);

	if (Anim)
	{		
		if (UAnimMontage* CurMontage = Anim->GetCurrentActiveMontage())
		{
			UE_LOG(LogTemp, Warning, TEXT("Current Montage: %s"), *CurMontage->GetName());
		}
	}

	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(
			DeathMontageTimerHandle,
			this,
			&ABaseCharacter::HandleDeathFinished,
			Duration,
			false
		);
	}
}

void ABaseCharacter::HandleDeathFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("[HandleDeathFinished] called on %s | HasAuthority: %d"), *GetName(), HasAuthority());
	if (UWorld* World = GetWorld())
	{
		if (AMainMapGameMode* GameMode = Cast<AMainMapGameMode>(World->GetAuthGameMode()))
		{
			GameMode->SendToPrison(this);
		}
	}
}


void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}


URepelComponent* ABaseCharacter::GetRepelComponent() const
{
	return nullptr;
}

UPawnInterActiveComponent* ABaseCharacter::GetInterActiveComponent() const
{
	return nullptr;
}

void ABaseCharacter::InitAbilityActorInfo()
{
	UE_LOG(LogTemp, Warning, TEXT("InitAbilityActorInfo() called on %s, IsLocallyControlled: %d"), *GetName(), IsLocallyControlled());

	if (ASTPlayerState* STplayerState = GetPlayerState<ASTPlayerState>())
	{
		if (ExtensionComponent)
		{
			ExtensionComponent->InitializeAbilitySystem(STplayerState->GetSTAbilitySystemComponent(), this);
		}

		STAbilitySystemComponent = STplayerState->GetSTAbilitySystemComponent();
		STAttributes = STplayerState->GetSTAttributeSet();
		
		if (IsValid(STAbilitySystemComponent))
		{
			STAbilitySystemComponent->InitAbilityActorInfo(STplayerState, this);

			if (HasAuthority())
			{
				InitClassDefaults();
				STAttributes->bIsInitialized = true;
				STAttributes->bRunnerLive = true;
			}

			if (STAttributes)
			{
				STAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(STAttributes->GetHealthAttribute())
				.AddUObject(this, &ABaseCharacter::OnHealthChanged);
			}
		}
	}
}

void ABaseCharacter::InitClassDefaults()
{
	if (!CharacterTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("No Character Tag Selected In this Character %s"), *GetNameSafe(this))
	}
	else if (UCharacterClassInfo* ClassInfo = USTAbilitySystemLibrary::GetCharacterClassDefaultInfo(this))
	{
		if (const FCharacterClassDefaultInfo* SelectedClassInfo = ClassInfo->ClassDefaultInfoMap.Find(CharacterTag))
		{
			if (IsValid(STAbilitySystemComponent))
			{
				if (ASTPlayerState* STPlayerState = GetPlayerState<ASTPlayerState>())
				{
					if (ExtensionComponent)
					{
						ExtensionComponent->UnInitializeAbilitySystem();
						ExtensionComponent->InitializeAbilitySystem(STPlayerState->GetSTAbilitySystemComponent(), this);
					}

					STAbilitySystemComponent->InitAbilityActorInfo(STPlayerState, this);

					STAbilitySystemComponent->AddCharacterAbilities(SelectedClassInfo->StartingAbilities);
					STAbilitySystemComponent->AddCharacterPassiveAbilities(SelectedClassInfo->StartingPassives);
					STAbilitySystemComponent->InitializeDefaultAbilities(SelectedClassInfo->DefaultAttributes);
					STAbilitySystemComponent->AddReactAbilities(SelectedClassInfo->ReactAbilities);
				}
			}
		}
	}
}




