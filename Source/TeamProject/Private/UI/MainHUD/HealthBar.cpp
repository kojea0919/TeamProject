// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHUD/HealthBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UHealthBar::NativeConstruct()
{
	Super::NativeConstruct();

	InitMaterial();
	InitBarState();
	InitAnimation();
}

void UHealthBar::UpdateCurHealth(float NewHealth)
{
	CurHealth = NewHealth;

	CurHealth = FMath::Clamp(CurHealth, 0, MaxHealth);

	if (HealthText)
	{
		HealthText->SetText(FText::AsNumber(CurHealth));
	}

	float UpdateHealthRate = CurHealth / MaxHealth;
	if (HealthBarDynMaterial)
	{
		HealthBarDynMaterial->SetScalarParameterValue(TEXT("DefaultValue"), UpdateHealthRate);
		HealthBarDynMaterial->SetScalarParameterValue(TEXT("Progress"), UpdateHealthRate);
	}
	UpdateTextPos();

	if (CurHealth >= 0)
	{
		IsDecreasing = true; 
		GetWorld()->GetTimerManager().SetTimer(IncreaseBarFillTimerHandle, this, &UHealthBar::IncreaseBar, 0.01f, true);

		if (CurHealth <= MaxHealth / 2)
			PlayAnimation(WarningBlinkAnimation);
		else
			PlayAnimation(BasicBlinkAnimation);
	}
	
	
}

void UHealthBar::UpdateHealth(float NewHealth, float InMaxHealth)
{
	MaxHealth = FMath::Max(InMaxHealth,1);
	UpdateCurHealth(NewHealth);
}

void UHealthBar::InitMaterial()
{
	UMaterialInstanceDynamic* DynMaterial = Img_HealthBar->GetDynamicMaterial();
	if (DynMaterial)
	{
		HealthBarDynMaterial = DynMaterial;
	}
}

void UHealthBar::InitBarState()
{
	if (HealthBarDynMaterial)
	{
		HealthBarDynMaterial->SetScalarParameterValue(TEXT("DefaultValue"), 1.0f);
		HealthBarDynMaterial->SetScalarParameterValue(TEXT("Progress"), 1.0f);

		HealthBarDynMaterial->GetVectorParameterValue(TEXT("Active__Outer_RGBA"), DefaultActiveOuter);
		HealthBarDynMaterial->GetVectorParameterValue(TEXT("Active_RGBA"), DefaultActive);
		HealthBarDynMaterial->GetVectorParameterValue(TEXT("Default_RGBA"), Default);
		HealthBarDynMaterial->GetVectorParameterValue(TEXT("Inactive_Outer_RGBA"), DefaultInactiveOuter);
		HealthBarDynMaterial->GetVectorParameterValue(TEXT("Inactive_RGBA"), DefaultInactive);
	}

	if (HealthText)
	{
		HealthText->SetText(FText::FromString(TEXT("100")));
	}
}

void UHealthBar::InitAnimation()
{
	BasicBlinkEndDelegate.BindDynamic(this, &UHealthBar::OnBlinkAnimFinished);
	WarningBlinkEndDelegate.BindDynamic(this, &UHealthBar::OnBlinkAnimFinished);

	BindToAnimationFinished(BasicBlinkAnimation, BasicBlinkEndDelegate);
	BindToAnimationFinished(WarningBlinkAnimation, WarningBlinkEndDelegate);
}

void UHealthBar::IncreaseBar()
{
	//�� Update
	OldHealth = CurHealth;

	//�ߴ� ����
	if (CurHealth >= MaxHealth)
	{
		CurHealth = MaxHealth;
		OldHealth = MaxHealth;

		GetWorld()->GetTimerManager().ClearTimer(IncreaseBarFillTimerHandle);
	}

	//ü�¹� Update
	float UpdateHealthRate = CurHealth / MaxHealth;
	if (HealthBarDynMaterial)
	{
		HealthBarDynMaterial->SetScalarParameterValue(TEXT("DefaultValue"), UpdateHealthRate);
		HealthBarDynMaterial->SetScalarParameterValue(TEXT("Progress"), UpdateHealthRate);
		UpdateTextPos();
		if (UpdateHealthRate > 0.5f)
		{
			HealthBarDynMaterial->SetVectorParameterValue(TEXT("Active__Outer_RGBA"), DefaultActiveOuter);
			HealthBarDynMaterial->SetVectorParameterValue(TEXT("Active_RGBA"), DefaultActive);
			HealthBarDynMaterial->SetVectorParameterValue(TEXT("Default_RGBA"), Default);
			HealthBarDynMaterial->SetVectorParameterValue(TEXT("Inactive_Outer_RGBA"), DefaultInactiveOuter);
			HealthBarDynMaterial->SetVectorParameterValue(TEXT("Inactive_RGBA"), DefaultInactive);
		}
		else
		{
			FLinearColor Red(1.0f, 0.0f, 0.0f, 0.6f);

			HealthBarDynMaterial->SetVectorParameterValue(TEXT("Active__Outer_RGBA"), Red);

			Red.A = 0.8f;
			HealthBarDynMaterial->SetVectorParameterValue(TEXT("Active_RGBA"), Red);
			Red.A = 1.0f;
			HealthBarDynMaterial->SetVectorParameterValue(TEXT("Default_RGBA"), Red);
			Red.A = 0.2f;
			HealthBarDynMaterial->SetVectorParameterValue(TEXT("Inactive_Outer_RGBA"), Red);
			Red.A = 0.3f;
			HealthBarDynMaterial->SetVectorParameterValue(TEXT("Inactive_RGBA"), Red);
		}
	}
	//ü�� Text Update
	if (HealthText)
		HealthText->SetText(FText::AsNumber((int)CurHealth));
}

void UHealthBar::UpdateTextPos()
{
	float CurStaminaRate = CurHealth / MaxHealth;

	int CurPos = FMath::Lerp(MaxLeftTextPos, 0, CurStaminaRate);
	CurPos = FMath::Clamp(CurPos, -275, 0);

	if (HealthText)
	{
		FWidgetTransform TextTransform;
		TextTransform.Translation = FVector2D(CurPos, 0);
		HealthText->SetRenderTransform(TextTransform);
	}
}

void UHealthBar::OnBlinkAnimFinished()
{
	if (CurHealth == MaxHealth)
		return;

	if (CurHealth <= MaxHealth / 2)
		PlayAnimation(WarningBlinkAnimation);
	else
		PlayAnimation(BasicBlinkAnimation);
}
