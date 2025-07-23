// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/Libraries/STFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"
#include "Map/Object/AbilitySystem/ObjectAbilitySystemComponent.h"
#include "Player/Character/AbilitySystem/STAbilitySystemComponent.h"
#include "Player/Character/Component/STExtensionComponent.h"
#include "Player/Character/Interface/RepelInterface.h"


UAbilitySystemComponent* USTFunctionLibrary::NativeGetParentAbilitySystemComponentFromActor(AActor* Actor)
{
	if (!Actor)
		return nullptr;

	return Cast<UAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor));
}

USTAbilitySystemComponent* USTFunctionLibrary::NativeGetAbilitySystemComponentFromActor(AActor* Actor)
{
	if (!Actor)
	{
		return nullptr;
	}

	return Cast<USTAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor));
}

UObjectAbilitySystemComponent* USTFunctionLibrary::NativeGetObjectAbilitySystemComponentFromActor(AActor* Actor)
{
	if (!Actor)
	{
		return nullptr;
	}

	return Cast<UObjectAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor));
}

void USTFunctionLibrary::AddTagToActor(AActor* Actor, FGameplayTag Tag)
{
	USTAbilitySystemComponent* ASC = NativeGetAbilitySystemComponentFromActor(Actor);

	if (!ASC->HasMatchingGameplayTag(Tag))
	{
		ASC->AddLooseGameplayTag(Tag);
	}
}

void USTFunctionLibrary::RemoveTagFromActor(AActor* Actor, FGameplayTag Tag)
{
	USTAbilitySystemComponent* ASC = NativeGetAbilitySystemComponentFromActor(Actor);

	if (ASC->HasMatchingGameplayTag(Tag))
	{
		ASC->RemoveLooseGameplayTag(Tag);
	}
}

bool USTFunctionLibrary::NativeActorHasTag(AActor* Actor, FGameplayTag Tag)
{
	if (!Actor)
	{
		return false;
	}

	// 확장 컴포넌트가 있다면 그걸 통해 ASC 접근
	if (const auto* Ext = Actor->FindComponentByClass<USTExtensionComponent>())
	{
		if (const auto* ASC = Ext->GetAbilitySystemComponent())
		{
			return ASC->HasMatchingGameplayTag(Tag);
		}
	}

	// 기본 인터페이스 접근도 시도 (백업)
	if (const IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(Actor))
	{
		if (const auto* ASC = ASCInterface->GetAbilitySystemComponent())
		{
			return ASC->HasMatchingGameplayTag(Tag);
		}
	}

	return false;
}

URepelComponent* USTFunctionLibrary::NativeGetRepelComponentFromActor(AActor* Actor)
{
	check(Actor);

	UE_LOG(LogTemp, Warning, TEXT("NativeGetRepelComponentFromActor called for: %s"), *Actor->GetName());
	if (IRepelInterface* PawnRepelInterface = Cast<IRepelInterface>(Actor))
	{
		return PawnRepelInterface->GetRepelComponent();
	}

	return nullptr;
}

URepelComponent* USTFunctionLibrary::BP_GetRepelComponentFromActor(AActor* Actor, EBaseValidType& OutValid)
{
	URepelComponent* RepelComponent = NativeGetRepelComponentFromActor(Actor);
	OutValid = RepelComponent ? EBaseValidType::Valid : EBaseValidType::InValid;

	return RepelComponent;
}
