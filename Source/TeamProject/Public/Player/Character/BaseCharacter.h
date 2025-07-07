

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "Interface/RepelInterface.h"
#include "UI/MainHUD/Healthbar.h"
#include "BaseCharacter.generated.h"


class ABaseEffectActor;
class USTAbilitySystemComponent;
class USTAttributeSet;

UCLASS()
class TEAMPROJECT_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface, public IRepelInterface
{
	GENERATED_BODY()

public:
	
	ABaseCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	// 스테미너 변경 함수
	UFUNCTION()
	void OnStaminaChanged(float CurrentStamina, float MaxStamina);

public:
	void SetActive(bool Active);
	
protected:

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	virtual URepelComponent* GetRepelComponent() const override;

	
private:

	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USTAbilitySystemComponent> STAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USTAttributeSet> STAttributes;

	UPROPERTY(EditAnywhere, Category = "Custom Values|Character Info")
	FGameplayTag CharacterTag;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UHealthbar> StaminaWidget;
	

	// 캐릭터 Ability System 관련 함수
	void InitAbilityActorInfo();
	void InitClassDefaults();

	// 캐릭터 Attribute 데이터 관련 함수
	void BindCallBacksToDependencies();

	UFUNCTION(BlueprintCallable)
	void BroadcastInitialValues();
	


};
