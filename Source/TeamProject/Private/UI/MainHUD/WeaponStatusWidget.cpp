// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHUD/WeaponStatusWidget.h"

#include "Components/TextBlock.h"
#include "GameFrameWork/MainMap/MainMapPlayerController.h"
#include "Map/Object/Actor/BaseObject.h"
#include "Player/Character/RunnerCharacter.h"
#include "Player/Character/Component/Interactive/RunnerInterActiveComponent.h"

void UWeaponStatusWidget::UpdateWeaponStatusUI()
{
	if (ABaseObject* Weapon = GetCurrentEquippedWeapon())
	{
		if (IsCurrentEquippedWeaponActive())
		{
			SetVisibility(ESlateVisibility::Visible);
			WeaponStatus->SetText(Weapon->GetStatusText());
			TB_StatusOwner->SetText(Weapon->GetObjectName());
		}

		else
		{
			SetVisibility(ESlateVisibility::Hidden);
		}
	}

	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

ABaseObject* UWeaponStatusWidget::GetCurrentEquippedWeapon()
{
	ARunnerCharacter* RC = Cast<ARunnerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (RC)
	{
		if (URunnerInterActiveComponent* RIC = RC->GetRunnerInterActiveComponent())
		{
			if (ABaseObject* CurrentWeapon = RIC->GetCharacterEquippedWeapon())
			{
				return CurrentWeapon;
			}
		}
	}
	return nullptr;
}

bool UWeaponStatusWidget::IsCurrentEquippedWeaponActive()
{
	ARunnerCharacter* RC = Cast<ARunnerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (RC)
	{
		if (URunnerInterActiveComponent* RIC = RC->GetRunnerInterActiveComponent())
		{
			return RIC->bEquippedWeaponIsActive;
		}
	}
	return false;
}
