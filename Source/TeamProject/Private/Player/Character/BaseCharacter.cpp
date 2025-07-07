// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/BaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "EffectObjectPool/BaseEffectActor.h"
#include "EffectObjectPool/EffectObjectPoolSubSystem.h"
#include "GameFrameWork/MainMap/MainMapPlayerController.h"
#include "Player/Character/AbilitySystem/STAbilitySystemComponent.h"
#include "Player/Character/AbilitySystem/Attributes/STAttributeSet.h"
#include "Player/Character/Data/CharacterClassInfo.h"
#include "Player/Character/Libraries/STAbilitySystemLibrary.h"
#include "Player/Character/PlayerState/STPlayerState.h"
#include "UI/MainHUD/Healthbar.h"
#include "UI/MainHUD/PlayerMainHUD.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;
	
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
	BindCallBacksToDependencies();
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return STAbilitySystemComponent;
}

void ABaseCharacter::OnStaminaChanged(float CurrentStamina, float MaxStamina)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController) return;

	AMainMapPlayerController* MyController = Cast<AMainMapPlayerController>(PlayerController);
	if (!MyController) return;

	if (UPlayerMainHUD* MainHUD = MyController->GetPlayerMainHUD())
	{
		if (UHealthbar* HealthBarWidget = MainHUD->GetHealthBarWidget())
		{
			HealthBarWidget->UpdateStamina(CurrentStamina, MaxStamina);
		}
	}
	
}

void ABaseCharacter::SetActive(bool Active)
{
	SetActorHiddenInGame(!Active);
	SetActorEnableCollision(Active);
	SetActorTickEnabled(Active);

	if (Active)
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	else
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	BindCallBacksToDependencies();
}

URepelComponent* ABaseCharacter::GetRepelComponent() const
{
	return nullptr;
}

void ABaseCharacter::InitAbilityActorInfo()
{
	if (ASTPlayerState* STplayerState = GetPlayerState<ASTPlayerState>())
	{
		STAbilitySystemComponent = STplayerState->GetSTAbilitySystemComponent();
		STAttributes = STplayerState->GetSTAttributeSet();

		if (IsValid(STAbilitySystemComponent))
		{
			STAbilitySystemComponent->InitAbilityActorInfo(STplayerState, this);
			
			if (HasAuthority())
			{
				InitClassDefaults();
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
				OnStaminaChanged(Data.NewValue, STAttributes->GetMaxStamina());
			});

		BroadcastInitialValues();
	}
}

void ABaseCharacter::BroadcastInitialValues()
{
	if (IsValid(STAttributes))
	{
		OnStaminaChanged(STAttributes->GetMaxStamina(), STAttributes->GetMaxStamina());
	}
}



