// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BlackBoard/StartBlackBoard.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameFrameWork/MainMap/MainMapGameMode.h"

void UStartBlackBoard::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Start)
		Btn_Start->OnClicked.AddDynamic(this,&UStartBlackBoard::ClickStartButton);

	if (Btn_DurationDecrease)
		Btn_DurationDecrease->OnClicked.AddDynamic(this,&UStartBlackBoard::ClickDurationDecreaseButton);

	if (Btn_DurationIncrease)
		Btn_DurationIncrease->OnClicked.AddDynamic(this,&UStartBlackBoard::ClickDurationIncreaseButton);

	if (Btn_TaggerCntDecrease)
		Btn_TaggerCntDecrease->OnClicked.AddDynamic(this,&UStartBlackBoard::ClickTaggerCntDecreaseButton);

	if (Btn_TaggerCntIncrease)
		Btn_TaggerCntIncrease->OnClicked.AddDynamic(this,&UStartBlackBoard::ClickTaggerCntIncreaseButton);
}

void UStartBlackBoard::Init()
{
	if (AMainMapGameMode * GameMode = GetWorld()->GetAuthGameMode<AMainMapGameMode>())
	{
		SetTaggerCntText(GameMode->GetTaggerCnt());
		SetDurationText(GameMode->GetGameProgressTime());
		
	}
}

void UStartBlackBoard::ClickStartButton()
{
	if (GetOwningPlayer() && GetOwningPlayer()->HasAuthority())
	{
		if (AMainMapGameMode * GameMode = GetWorld()->GetAuthGameMode<AMainMapGameMode>())
		{
			GameMode->GameStart();
		}
	}
}

void UStartBlackBoard::ClickDurationDecreaseButton()
{
	if (GetOwningPlayer() && GetOwningPlayer()->HasAuthority())
	{
		if (AMainMapGameMode * GameMode = GetWorld()->GetAuthGameMode<AMainMapGameMode>())
		{
			int CurTime = GameMode->DecreaseGameProgressTime();
			SetDurationText(CurTime);
		}
	}
}

void UStartBlackBoard::ClickDurationIncreaseButton()
{
	if (GetOwningPlayer() && GetOwningPlayer()->HasAuthority())
	{
		if (AMainMapGameMode * GameMode = GetWorld()->GetAuthGameMode<AMainMapGameMode>())
		{
			int CurTime = GameMode->IncreaseGameProgressTime();
			SetDurationText(CurTime);
		}
	}
}

void UStartBlackBoard::ClickTaggerCntDecreaseButton()
{
	if (GetOwningPlayer() && GetOwningPlayer()->HasAuthority())
	{
		if (AMainMapGameMode * GameMode = GetWorld()->GetAuthGameMode<AMainMapGameMode>())
		{
			int CurCnt = GameMode->DecreaseTaggerCnt();
			SetTaggerCntText(CurCnt);
		}
	}
}

void UStartBlackBoard::ClickTaggerCntIncreaseButton()
{
	if (GetOwningPlayer() && GetOwningPlayer()->HasAuthority())
	{
		if (AMainMapGameMode * GameMode = GetWorld()->GetAuthGameMode<AMainMapGameMode>())
		{
			int CurCnt = GameMode->IncreaseTaggerCnt();
			SetTaggerCntText(CurCnt);
		}
	}
}

void UStartBlackBoard::PlayFadeIn()
{
	if (FadeIn)
		PlayAnimation(FadeIn);
}

void UStartBlackBoard::PlayFadeOut()
{
	if (FadeOut)
		PlayAnimation(FadeOut);
}

void UStartBlackBoard::SetDurationText(int Time)
{
	if (Tb_Duration)
	{
		int Min = Time / 60;
		int Sec = Time % 60;

		FString TimeText = FString::FromInt(Min) + " : " + FString::FromInt(Sec);
		Tb_Duration->SetText(FText::FromString(TimeText));
	}
}

void UStartBlackBoard::SetTaggerCntText(int Cnt)
{
	if (Tb_TaggerCnt)
		Tb_TaggerCnt->SetText(FText::AsNumber(Cnt));
}
