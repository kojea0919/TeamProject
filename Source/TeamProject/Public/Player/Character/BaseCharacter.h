

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "BaseType/BaseStructType.h"
#include "GameFramework/Character.h"
#include "Interface/InterActiveInterface.h"
#include "Interface/RepelInterface.h"
#include "UI/MainHUD/StaminaBar.h"
#include "BaseCharacter.generated.h"


class UHealthBar;
class ABaseEffectActor;
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
	// 캐릭터 Ability System 관련 함수
	void InitAbilityActorInfo();
	void InitClassDefaults();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual USTAttributeSet* GetAttributeSet() const;

	virtual USTExtensionComponent* GetExtensionComponent() const {return ExtensionComponent;}
	
	// AttributeSet Hud 등록
	UFUNCTION()
	void RegisterAttributeSetInHUD();

	

public:
	UFUNCTION(NetMulticast,Reliable)
	void SetActive(bool Active);

	FGameplayTag GetBaseCharacterTag() const { return CharacterTag; }
	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = true))
	TObjectPtr<URepelComponent> RepelComponent;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USTAbilitySystemComponent> STAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USTAttributeSet> STAttributes;

	UPROPERTY(EditAnywhere, Category = "Custom Values|Character Info")
	FGameplayTag CharacterTag;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaminaBar> StaminaWidget;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UHealthBar> HealthWidget;

	UPROPERTY(ReplicatedUsing = OnRep_AttachData)
	FAttachRepData AttachData;
	

	
	

};
