// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHUD/ShowRole.h"

void UShowRole::Init()
{
	if (W_Tagger)
		W_Tagger->SetVisibility(ESlateVisibility::Hidden);

	if (W_Runner)
		W_Runner->SetVisibility(ESlateVisibility::Hidden);
}

void UShowRole::ShowRole(bool IsTagger)
{
	if (IsTagger)
	{
		if (W_Tagger)
			W_Tagger->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		if (W_Runner)
			W_Runner->SetVisibility(ESlateVisibility::Visible);
	}

	if (FadeOut)
		PlayAnimation(FadeOut);
}
