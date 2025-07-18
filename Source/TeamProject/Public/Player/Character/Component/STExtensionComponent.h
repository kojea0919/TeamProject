// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STExtensionComponent.generated.h"

class USTAttributeSet;
class USTAbilitySystemComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMPROJECT_API USTExtensionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USTExtensionComponent();

	void InitializeAbilitySystem(USTAbilitySystemComponent* InASC, AActor* InOwnerActor);
	void UnInitializeAbilitySystem();

	UFUNCTION(BlueprintCallable, Category = "GAS")
	USTAbilitySystemComponent* GetAbilitySystemComponent() const {return AbilitySystemComponent;}

	void RegisterOnAbilitySystemInitialized(const FSimpleDelegate& Delegate);

	// AttributeSet
	UFUNCTION(BlueprintCallable, Category = "GAS")
	USTAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
	
protected:
	
	UPROPERTY(Transient)
	TObjectPtr<USTAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(Transient)
	TObjectPtr<USTAttributeSet> AttributeSet;

private:
	bool bInitialized = false;
	
	TArray<FSimpleDelegate> OnInitializedDelegate;
	
};
