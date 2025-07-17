// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/AbilitySystem/Attributes/STAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameTag/STGamePlayTags.h"
#include "Net/UnrealNetwork.h"
#include "Player/Character/Libraries/STFunctionLibrary.h"

void USTAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(USTAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USTAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USTAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USTAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	
}

void USTAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
	}

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}

	if (bIsInitialized && GetHealth() == 0.0f)
	{
		USTFunctionLibrary::AddTagToActor(Data.Target.GetAvatarActor(), STGamePlayTags::Player_Runner_Status_Dead);
	}
}

void USTAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USTAttributeSet, Stamina, OldStamina);

	if (GetOwningAbilitySystemComponent())
	{
		FOnAttributeChangeData Data;
		Data.Attribute = GetStaminaAttribute();
		Data.OldValue = OldStamina.GetCurrentValue();
		Data.NewValue = OldStamina.GetCurrentValue();

		GetOwningAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(GetStaminaAttribute()).Broadcast(Data);
	}
}

void USTAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USTAttributeSet, MaxStamina, OldMaxStamina);
}

void USTAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USTAttributeSet, Health, OldHealth);
}

void USTAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USTAttributeSet, MaxHealth, OldMaxHealth);
}
