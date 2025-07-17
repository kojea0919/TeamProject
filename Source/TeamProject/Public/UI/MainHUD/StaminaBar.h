#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StaminaBar.generated.h"

UCLASS()
class TEAMPROJECT_API UStaminaBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void UpdateCurStamina(float NewStamina);
	
	void UpdateStamina(float NewStamina, float InMaxStamina);
	
private:
	void InitMaterial();
	void InitBarState();
	void InitAnimation();

	void IncreaseBar();

	void UpdateTextPos();

	UFUNCTION()
	void OnBlinkAnimFinished();

	

private:
	//UI
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StaminaText;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* Img_StaminaBar;
	

	//UI  Material
	//--------------------------------
	UPROPERTY(VisibleAnywhere, Category = Material)
	UMaterialInstanceDynamic* HealthBarDynMaterial;

	// UPROPERTY(VisibleAnywhere, Category = Material)
	// UMaterialInstanceDynamic* StaminaBarDynMaterial;
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
	float OldStamina;
	float Stamina;
	float MaxStamina;
	//--------------------------------

	
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
