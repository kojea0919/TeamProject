#include "UI/MainHUD/ShowRole.h"

void UShowRole::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FadeOutEndDelegate.BindDynamic(this, &UShowRole::OnFadeOutFinished);
	
	if (FadeOut)
		BindToAnimationFinished(FadeOut,FadeOutEndDelegate);
}

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
		{
			W_Tagger->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		if (W_Runner)
		{
			W_Runner->SetVisibility(ESlateVisibility::Visible);
		}
	}

	if (FadeOut)
		PlayAnimation(FadeOut);
}

void UShowRole::OnFadeOutFinished()
{
	SetVisibility(ESlateVisibility::Hidden);
	if (W_Tagger)
	{
		W_Tagger->SetVisibility(ESlateVisibility::Hidden);
	}
	if (W_Runner)
	{
		W_Runner->SetVisibility(ESlateVisibility::Hidden);
	}
}
