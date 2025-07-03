#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Healthbar.generated.h"

UCLASS()
class TEAMPROJECT_API UHealthbar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void UpdateCurHealth(float NewHP);

	UFUNCTION(BlueprintCallable)
	void UpdateCurStamina(float NewStamina);
	
	void UpdateStamina(float NewStamina, float InMaxStamina);
	
private:
	void InitMaterial();
	void InitBarState();
	void InitStamina();
	void InitAnimation();

	void IncreaseBar();

	void UpdateTextPos();

	UFUNCTION()
	void OnBlinkAnimFinished();

	

private:
	//UI
	//--------------------------------
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StaminaText;

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_HealthBar;
	
	//--------------------------------

	//UI  Material
	//--------------------------------
	UPROPERTY(VisibleAnywhere, Category = Material)
	UMaterialInstanceDynamic* HealthBarDynMaterial;
	//--------------------------------

	//UI Animation
	//--------------------------------
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* BasicBlinkAnimation;

	//��ųŰ�� ������ ��ų ����� �Ұ����� ��� ����� ȿ��
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* WarningBlinkAnimation;

	FWidgetAnimationDynamicEvent BasicBlinkEndDelegate;
	FWidgetAnimationDynamicEvent WarningBlinkEndDelegate;
	//--------------------------------

	//ü�� ���� ����
	//--------------------------------
	float OldHealth;
	float CurHealth;

	const float MaxHealth = 100;
	//--------------------------------

	float OldStamina;
	float Stamina;
	float MaxStamina;
	//ü�� ������Ʈ ���� ����
	//--------------------------------
	FTimerHandle IncreaseBarFillTimerHandle;		//ü�� ȸ�� ����Ʈ Timer
	float IncreaseHealthSpeed = 0.08f;				//ü�� ȸ�� �ӵ�

	bool IsDecreasing;
	
	//ü�� Text ��ġ
	const int MaxLeftTextPos = -345;	
	//--------------------------------

	//ü�� �� ���� ����
	//--------------------------------
	FLinearColor DefaultActiveOuter;
	FLinearColor DefaultActive;
	FLinearColor Default;
	FLinearColor DefaultInactiveOuter;
	FLinearColor DefaultInactive;
	//--------------------------------
	
};
