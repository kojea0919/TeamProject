// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/BaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "EffectObjectPool/BaseEffectActor.h"
#include "EffectObjectPool/EffectObjectPoolSubSystem.h"
#include "GameFrameWork/MainMap/MainMapGameMode.h"
#include "GameFrameWork/MainMap/MainMapPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Player/Character/AbilitySystem/STAbilitySystemComponent.h"
#include "Player/Character/AbilitySystem/Attributes/STAttributeSet.h"
#include "Player/Character/Component/STExtensionComponent.h"
#include "Player/Character/Component/Repel/RepelComponent.h"
#include "Player/Character/Data/CharacterClassInfo.h"
#include "Player/Character/Libraries/STAbilitySystemLibrary.h"
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
}


void ABaseCharacter::Multicast_PlayDeathMontage_Implementation()
{
	if (!DeathMontage || !GetMesh())
	{
		HandleDeathFinished();
		return;
	}

	// ✅ 강제 Mesh 보이게 만들기
	GetMesh()->SetVisibility(true, true);
	GetMesh()->SetHiddenInGame(false);
	GetMesh()->bOwnerNoSee = false;
	GetMesh()->bOnlyOwnerSee = false;
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->bPauseAnims = false;

	// ✅ AnimInstance가 없으면 강제로 재설정 (극히 드물게 발생)
	if (!GetMesh()->GetAnimInstance())
	{
		GetMesh()->InitAnim(true); // 또는 SetAnimInstanceClass(DefaultAnimBPClass);
	}

	UAnimInstance* Anim = GetMesh()->GetAnimInstance();

	if (Anim && DeathMontage)
	{
		// ✅ 강제 로컬 재생 (Multicast 무시된 클라 포함)
		const float Duration = Anim->Montage_Play(DeathMontage, 1.f);

		if (Duration > 0.f)
		{
			// ✅ 애니메이션이 끝나면 후처리 (감옥 이동 등)
			FTimerHandle Timer;
			GetWorld()->GetTimerManager().SetTimer(
				Timer,
				this,
				&ABaseCharacter::HandleDeathFinished,
				Duration,
				false
			);
		}
		else
		{
			HandleDeathFinished(); // 실패 시 즉시 후처리
		}
	}
	else
	{
		HandleDeathFinished();
	}
}

void ABaseCharacter::Server_TriggerDeath_Implementation()
{
	Multicast_PlayDeathMontage();
}

void ABaseCharacter::HandleDeathFinished()
{
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




