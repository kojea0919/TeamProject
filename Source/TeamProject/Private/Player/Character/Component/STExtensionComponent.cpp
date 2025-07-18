// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/Component/STExtensionComponent.h"

#include "Player/Character/AbilitySystem/STAbilitySystemComponent.h"
#include "Player/Character/AbilitySystem/Attributes/STAttributeSet.h"

// Sets default values for this component's properties
USTExtensionComponent::USTExtensionComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

}

void USTExtensionComponent::InitializeAbilitySystem(USTAbilitySystemComponent* InASC, AActor* InOwnerActor)
{
	UE_LOG(LogTemp, Warning, TEXT("InitializeAbilitySystem Executed"));
	if (bInitialized || !InASC || !InOwnerActor)
	{
		return;
	}

	AbilitySystemComponent = InASC;
	InASC->InitAbilityActorInfo(InOwnerActor, GetOwner());

	bInitialized = true;

	for (const FSimpleDelegate& Delegate : OnInitializedDelegate)
	{
		Delegate.ExecuteIfBound();
	}
	OnInitializedDelegate.Empty();
	
}

void USTExtensionComponent::UnInitializeAbilitySystem()
{
	if (!AbilitySystemComponent) return;

	if (AbilitySystemComponent->GetAvatarActor() == GetOwner())
	{
		AbilitySystemComponent->CancelAbilities();
		AbilitySystemComponent->ClearAllAbilities();
		AbilitySystemComponent->RemoveAllGameplayCues();
		AbilitySystemComponent->SetAvatarActor(nullptr);
	}

	AbilitySystemComponent = nullptr;
	bInitialized = false;
}

void USTExtensionComponent::RegisterOnAbilitySystemInitialized(const FSimpleDelegate& Delegate)
{
	UE_LOG(LogTemp, Warning, TEXT("RegisterOnAbilitySystemInitialized Called"));
	if (bInitialized)
	{
		Delegate.ExecuteIfBound();
	}
	else
	{
		OnInitializedDelegate.Add(Delegate);
	}
}





