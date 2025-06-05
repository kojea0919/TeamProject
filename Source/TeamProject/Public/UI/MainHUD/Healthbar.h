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

private:
	void InitMaterial();
	void InitBarState();
	void InitHP();
	void InitAnimation();

	void IncreaseBar();

	void UpdateTextPos();

	UFUNCTION()
	void OnBlinkAnimFinished();

private:
	//UI
	//--------------------------------
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Tb_HealthText;

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
