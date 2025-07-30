// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/BaseCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "EffectObjectPool/BaseEffectActor.h"
#include "EffectObjectPool/EffectObjectPoolSubSystem.h"
#include "GameFrameWork/MainMap/MainMapGameMode.h"
#include "GameFrameWork/MainMap/MainMapPlayerController.h"
#include "GameTag/STGamePlayTags.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/Character/RunnerCharacter.h"
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
#include "Player/Character/Input/STInputConfig.h"
#include "EnhancedInputSubsystems.h"
#include "Player/Character/Input/STEnhancedInputComponent.h"

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
		// if (AMainMapPlayerController* MainMapController = Cast<AMainMapPlayerController>(GetController()))
		// {
		// 	if (MainMapController->IsLocalPlayerController())
		// 	{
		// 		MainMapController->Client_SetAimUI();
		// 	}
		// }
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

void ABaseCharacter::Destroyed()
{
	Super::Destroyed();

	int a = 10 ;
	++a;

	if (a == 2)
		a = 3;
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

void ABaseCharacter::SetBaseCharacterActive_Implementation(bool Active)
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
	DOREPLIFETIME(ABaseCharacter, bIsGhost);
}

void ABaseCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (HasAuthority() && Data.NewValue <= 0.0f )
	{
		AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
		if (AMainMapGameMode* MainGameMode = Cast<AMainMapGameMode>(GameMode))
		{
			if (MainGameMode->GetCurrentGameMode() == MissionMode)
			{
				OnDied_Server();
			}
			else
			{
				OnGhost_Server();
			}
		}
	}
}

void ABaseCharacter::OnGhost_Server_Implementation()
{
	if (bIsGhost) return;
	bIsGhost = true;

	if (ARunnerCharacter* Character = Cast<ARunnerCharacter>(this))
	{
		Character->SetGhostMode();
	}	
}

void ABaseCharacter::OnRep_IsGhost()
{
	if (bIsGhost)
	{
		if (ARunnerCharacter* Character = Cast<ARunnerCharacter>(this))
		{
			Character->SetGhostMode();
		}	
	}
}


void ABaseCharacter::OnDied_Server_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnDied_Server() → LocallyControlled=%d, Name=%s"), IsLocallyControlled(), *GetName());
	if (bIsDead) return;
	bIsDead = true;

	UAbilitySystemComponent* AbilitySystemComponent = USTFunctionLibrary::NativeGetParentAbilitySystemComponentFromActor(this);
	if (AbilitySystemComponent != nullptr)
	{
		FGameplayEventData EventData;
		EventData.Instigator = this;
		EventData.Target = this;
		EventData.EventTag = STGamePlayTags::Player_Runner_Event_Dead;
						
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, STGamePlayTags::Player_Runner_Event_Dead, EventData);
				
	}
}

void ABaseCharacter::OnRep_IsDead()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_IsDead() → LocallyControlled=%d, Name=%s"), IsLocallyControlled(), *GetName());
	
	if (bIsDead)
	{
		UAbilitySystemComponent* AbilitySystemComponent = USTFunctionLibrary::NativeGetParentAbilitySystemComponentFromActor(this);
		if (AbilitySystemComponent->AbilityActorInfo->IsLocallyControlled())
		{
			UE_LOG(LogTemp, Warning, TEXT(">> Client: We are locally controlled. Sending Event."));
			FGameplayEventData EventData;
			EventData.Instigator = this;
			EventData.Target = this;
			EventData.EventTag = STGamePlayTags::Player_Runner_Event_Dead;

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, STGamePlayTags::Player_Runner_Event_Dead, EventData);
		}
	}
}

void ABaseCharacter::Multicast_PlayerFootStep_Implementation(USoundBase* Sound, FVector Location, float Volume,
	float Pitch)
{
	if (Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, Sound, Location, Volume, Pitch);
	}
}

void ABaseCharacter::CallFootStepEffect_Implementation(const FVector Location)
{
	Multicast_FootStepEffect(Location);
}

void ABaseCharacter::Multicast_FootStepEffect_Implementation(const FVector Location)
{
	if (UEffectObjectPoolSubSystem* ObjectPoolSubsystem = GetWorld()->GetSubsystem<UEffectObjectPoolSubSystem>())
	{
		if (ABaseEffectActor* FootStepNiagara = ObjectPoolSubsystem->GetEffectObject(FootStepEffect))
		{
			FootStepNiagara->SetActorLocation(Location);
		}
	}
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as Input Config"));

	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem);
	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext,0);

	USTEnhancedInputComponent* STInputComponent = CastChecked<USTEnhancedInputComponent>(PlayerInputComponent);
	STInputComponent->BindNativeInputAction(InputConfigDataAsset, STGamePlayTags::Input_CameraModeChange, ETriggerEvent::Started,
		this, &ARunnerCharacter::Input_CameraMode);
	
}

void ABaseCharacter::Input_CameraMode(const FInputActionValue& InputActionValue)
{
	bIsCameraModeYawEnabled = !bIsCameraModeYawEnabled;

	if (HasAuthority())
	{
		Multicast_SetUseControllerRotationYaw(bIsCameraModeYawEnabled);
	}
	else
	{
		Server_SetCameraModeYaw(bIsCameraModeYawEnabled);
	}
}

void ABaseCharacter::Multicast_SetUseControllerRotationYaw_Implementation(bool bNewValue)
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = bNewValue;
}

void ABaseCharacter::Server_SetCameraModeYaw_Implementation(bool bNewValue)
{
	bIsCameraModeYawEnabled = bNewValue;
	Multicast_SetUseControllerRotationYaw(bNewValue);
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




