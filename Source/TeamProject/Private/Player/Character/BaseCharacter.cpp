// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/BaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "EffectObjectPool/BaseEffectActor.h"
#include "EffectObjectPool/EffectObjectPoolSubSystem.h"
#include "GameFrameWork/MainMap/MainMapPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Player/Character/AbilitySystem/STAbilitySystemComponent.h"
#include "Player/Character/AbilitySystem/Attributes/STAttributeSet.h"
#include "Player/Character/Component/STExtensionComponent.h"
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
	
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (HasAuthority())
	{
		InitAbilityActorInfo();
		BindCallBacksToDependencies();	
	}
		
}

void ABaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
	BindCallBacksToDependencies();	
	
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return ExtensionComponent ? ExtensionComponent->GetAbilitySystemComponent() : nullptr;
}

void ABaseCharacter::OnStaminaChanged(float CurrentStamina, float MaxStamina)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController) return;

	AMainMapPlayerController* MyController = Cast<AMainMapPlayerController>(PlayerController);
	if (!MyController) return;

	if (UPlayerMainHUD* MainHUD = MyController->GetPlayerMainHUD())
	{
		if (UStaminaBar* StaminaBarWidget = MainHUD->GetStaminaBarWidget())
		{
			StaminaBarWidget->UpdateStamina(CurrentStamina, MaxStamina);
		}
	}
	
}

void ABaseCharacter::OnHealthChanged(float CurrentHealth, float MaxHealth)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController) return;

	AMainMapPlayerController* MyController = Cast<AMainMapPlayerController>(PlayerController);
	if (!MyController) return;

	if (UPlayerMainHUD* MainHUD = MyController->GetPlayerMainHUD())
	{
		if (UHealthBar* HealthBarWidget = MainHUD->GetHealthBarWidget())
		{
			HealthBarWidget->UpdateHealth(CurrentHealth, MaxHealth);
		}
	}
}

void ABaseCharacter::SetActive_Implementation(bool Active)
{
	SetActorHiddenInGame(!Active);
	SetActorEnableCollision(Active);
	SetActorTickEnabled(Active);

	if (Active)
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
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
	if (!AttachData.bAttached || !AttachData.TargetActor) return;

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
				STAbilitySystemComponent->AddCharacterAbilities(SelectedClassInfo->StartingAbilities);
				STAbilitySystemComponent->AddCharacterPassiveAbilities(SelectedClassInfo->StartingPassives);
				STAbilitySystemComponent->InitializeDefaultAbilities(SelectedClassInfo->DefaultAttributes);
				STAbilitySystemComponent->AddReactAbilities(SelectedClassInfo->ReactAbilities);
			}
		}
	}
}

void ABaseCharacter::BindCallBacksToDependencies()
{
	if (IsValid(STAbilitySystemComponent) && IsValid(STAttributes))
	{
		STAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(STAttributes->GetStaminaAttribute()).AddLambda(
			[this] (const FOnAttributeChangeData& Data)
			{
				if (IsValid(STAttributes))
				{
					const float MaxStamina = STAttributes->GetMaxStamina();

					if (!FMath::IsFinite(MaxStamina))
					{
						return;
					}
					OnStaminaChanged(Data.NewValue, STAttributes->GetMaxStamina());
				}
			});

		STAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(STAttributes->GetHealthAttribute()).AddLambda(
			[this] (const FOnAttributeChangeData& Data)
			{
				OnHealthChanged(Data.NewValue, STAttributes->GetMaxHealth());
			});

		BroadcastInitialValues();
	}
}

void ABaseCharacter::BroadcastInitialValues()
{
	if (IsValid(STAttributes))
	{
		const float Stamina = STAbilitySystemComponent->GetNumericAttribute(STAttributes->GetStaminaAttribute());
		const float MaxStamina = STAbilitySystemComponent->GetNumericAttribute(STAttributes->GetMaxStaminaAttribute());
		
		OnStaminaChanged(Stamina,MaxStamina);
		OnHealthChanged(STAttributes->GetHealth(), STAttributes->GetMaxHealth());
	}
}



