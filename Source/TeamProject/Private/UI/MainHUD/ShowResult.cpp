#include "UI/MainHUD/ShowResult.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void UShowResult::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FadeOutEndDelegate.BindUFunction(this, TEXT("OnFadeOutFinished"));

	if (FadeOut)
		BindToAnimationFinished(FadeOut,FadeOutEndDelegate);
}

void UShowResult::Init()
{
	if (W_TaggerWin)
		W_TaggerWin->SetVisibility(ESlateVisibility::Hidden);

	if (W_RunnerWin)
		W_RunnerWin->SetVisibility(ESlateVisibility::Hidden);
}

void UShowResult::ShowResult(bool IsTaggerWin)
{
	if (IsTaggerWin)
	{
		if (W_TaggerWin)
			W_TaggerWin->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		if (W_RunnerWin)
			W_RunnerWin->SetVisibility(ESlateVisibility::Visible);
	}

	if (FadeOut)
		PlayAnimation(FadeOut);

	if (ShowResultSound)
		UGameplayStatics::PlaySound2D(this,ShowResultSound, 0.3);
}

void UShowResult::OnFadeOutFinished()
{
	SetVisibility(ESlateVisibility::Hidden);

	UE_LOG(LogTemp,Warning,TEXT("Call"));
	if (W_TaggerWin)
	{
		W_TaggerWin->SetVisibility(ESlateVisibility::Hidden);
	}
	if (W_RunnerWin)
	{
		W_RunnerWin->SetVisibility(ESlateVisibility::Hidden);
	}
}
