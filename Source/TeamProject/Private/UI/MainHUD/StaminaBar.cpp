#include "UI/MainHUD/StaminaBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UStaminaBar::NativeConstruct()
{
	Super::NativeConstruct();

	InitMaterial();
	InitBarState();	
	InitAnimation();
}

void UStaminaBar::UpdateCurStamina(float NewStamina)
{
	Stamina = NewStamina;

	Stamina = FMath::Clamp(Stamina, 0, MaxStamina);

	if (StaminaText)
	{
		StaminaText->SetText(FText::AsNumber(Stamina));
	}


	float UpdateHealthRate = Stamina / MaxStamina;
	if (HealthBarDynMaterial)
	{
		HealthBarDynMaterial->SetScalarParameterValue(TEXT("DefaultValue"), UpdateHealthRate);
		HealthBarDynMaterial->SetScalarParameterValue(TEXT("Progress"), UpdateHealthRate);
	}
	UpdateTextPos();

	if (Stamina > 0)
	{
		IsDecreasing = true; 
		GetWorld()->GetTimerManager().SetTimer(IncreaseBarFillTimerHandle, this, &UStaminaBar::IncreaseBar, 0.01f, true);

		if (Stamina <= MaxStamina / 2)
			PlayAnimation(WarningBlinkAnimation);
		else
			PlayAnimation(BasicBlinkAnimation);
	}
}

void UStaminaBar::InitMaterial()
{
	UMaterialInstanceDynamic* DynMaterial = Img_StaminaBar->GetDynamicMaterial();
	if (DynMaterial)
	{
		HealthBarDynMaterial = DynMaterial;
	}
}

void UStaminaBar::InitBarState()
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

	if (StaminaText)
	{
		StaminaText->SetText(FText::FromString(TEXT("100")));
	}
}


void UStaminaBar::InitAnimation()
{
	BasicBlinkEndDelegate.BindDynamic(this, &UStaminaBar::OnBlinkAnimFinished);
	WarningBlinkEndDelegate.BindDynamic(this, &UStaminaBar::OnBlinkAnimFinished);

	BindToAnimationFinished(BasicBlinkAnimation, BasicBlinkEndDelegate);
	BindToAnimationFinished(WarningBlinkAnimation, WarningBlinkEndDelegate);
}

void UStaminaBar::IncreaseBar() 
{
	//�� Update
	
	OldStamina = Stamina;

	//�ߴ� ����
	if (Stamina >= MaxStamina)
	{
		Stamina = MaxStamina;
		OldStamina = MaxStamina;

		GetWorld()->GetTimerManager().ClearTimer(IncreaseBarFillTimerHandle);
	}

	//ü�¹� Update
	float UpdateStaminaRate = Stamina / MaxStamina;
	if (HealthBarDynMaterial)
	{
		HealthBarDynMaterial->SetScalarParameterValue(TEXT("DefaultValue"), UpdateStaminaRate);
		HealthBarDynMaterial->SetScalarParameterValue(TEXT("Progress"), UpdateStaminaRate);
		UpdateTextPos();
		if (UpdateStaminaRate > 0.5f)
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
	if (StaminaText)
		StaminaText->SetText(FText::AsNumber((int)Stamina));

}

void UStaminaBar::UpdateTextPos()
{
	float CurStaminaRate = Stamina / MaxStamina;

	int CurPos = FMath::Lerp(MaxLeftTextPos, 0, CurStaminaRate);
	CurPos = FMath::Clamp(CurPos, -275, 0);

	if (StaminaText)
	{
		FWidgetTransform TextTransform;
		TextTransform.Translation = FVector2D(CurPos, 0);
		StaminaText->SetRenderTransform(TextTransform);
	}
}

void UStaminaBar::OnBlinkAnimFinished()
{
	if (Stamina == MaxStamina)
		return;

	if (Stamina <= MaxStamina / 2)
		PlayAnimation(WarningBlinkAnimation);
	else
		PlayAnimation(BasicBlinkAnimation);
}

void UStaminaBar::UpdateStamina(float NewStamina, float InMaxStamina)
{
	MaxStamina = InMaxStamina;
	UpdateCurStamina(NewStamina);
}
