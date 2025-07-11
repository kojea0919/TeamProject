

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "BaseType/BaseStructType.h"
#include "GameFramework/Character.h"
#include "Interface/InterActiveInterface.h"
#include "Interface/RepelInterface.h"
#include "UI/MainHUD/Healthbar.h"
#include "BaseCharacter.generated.h"


class UPawnInterActiveComponent;
class USTAbilitySystemComponent;
class USTAttributeSet;
class USTExtensionComponent;

UCLASS()
class TEAMPROJECT_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface, public IRepelInterface, public IInterActiveInterface
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
	// AttachToComponent
	UFUNCTION(BlueprintCallable)
	void AttachActorToComponent_Replicated(AActor* TargetActor, USceneComponent* InParentComponent, FName SocketName);

	UFUNCTION()
	void OnRep_AttachData();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	
protected:

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	virtual URepelComponent* GetRepelComponent() const override;
	virtual UPawnInterActiveComponent* GetInterActiveComponent() const override;

	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = true))
	TObjectPtr<USTExtensionComponent> ExtensionComponent;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USTAbilitySystemComponent> STAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USTAttributeSet> STAttributes;

	UPROPERTY(EditAnywhere, Category = "Custom Values|Character Info")
	FGameplayTag CharacterTag;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UHealthbar> StaminaWidget;

	UPROPERTY(ReplicatedUsing = OnRep_AttachData)
	FAttachRepData AttachData;
	

	// 캐릭터 Ability System 관련 함수
	void InitAbilityActorInfo();
	void InitClassDefaults();

	// 캐릭터 Attribute 데이터 관련 함수
	void BindCallBacksToDependencies();

	UFUNCTION(BlueprintCallable)
	void BroadcastInitialValues();
	


};
