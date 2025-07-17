// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

	public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void UpdateCurHealth(float NewHealth);

	void UpdateHealth(float NewHealth, float InMaxHealth);
	
private:
	void InitMaterial();
	void InitBarState();
	void InitAnimation();

	void IncreaseBar();

	void UpdateTextPos();

	UFUNCTION()
	void OnBlinkAnimFinished();

	

private:
		
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HealthText;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* Img_HealthBar;
	
	//UI  Material
	//--------------------------------
	UPROPERTY(VisibleAnywhere, Category = Material)
	UMaterialInstanceDynamic* HealthBarDynMaterial;
	
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
	float MaxHealth;
	
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

