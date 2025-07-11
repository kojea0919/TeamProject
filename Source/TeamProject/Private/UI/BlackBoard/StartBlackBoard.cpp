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

	if (Btn_GraffitiCntDecrease)
		Btn_GraffitiCntDecrease->OnClicked.AddDynamic(this,&UStartBlackBoard::ClickGraffitiCntDecreaseButton);

	if (Btn_GraffitiCntIncrease)
		Btn_GraffitiCntIncrease->OnClicked.AddDynamic(this,&UStartBlackBoard::ClickGraffitiCntIncreaseButton);
}

void UStartBlackBoard::Init()
{
	if (AMainMapGameMode * GameMode = GetWorld()->GetAuthGameMode<AMainMapGameMode>())
	{
		SetTaggerCntText(GameMode->GetTaggerCnt());
		SetDurationText(GameMode->GetGameProgressTime());
		SetGraffitiCntText(GameMode->GetGraffitiCnt());		
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
			int CurCnt = GameMode->IncreaseGraffitiCnt();
			SetTaggerCntText(CurCnt);
		}
	}
}

void UStartBlackBoard::ClickGraffitiCntDecreaseButton()
{
	if (GetOwningPlayer() && GetOwningPlayer()->HasAuthority())
	{
		if (AMainMapGameMode * GameMode = GetWorld()->GetAuthGameMode<AMainMapGameMode>())
		{
			int CurCnt = GameMode->DecreaseGraffitiCnt();
			SetGraffitiCntText(CurCnt);
		}
	}
}

void UStartBlackBoard::ClickGraffitiCntIncreaseButton()
{
	if (GetOwningPlayer() && GetOwningPlayer()->HasAuthority())
	{
		if (AMainMapGameMode * GameMode = GetWorld()->GetAuthGameMode<AMainMapGameMode>())
		{
			int CurCnt = GameMode->IncreaseGraffitiCnt();
			SetGraffitiCntText(CurCnt);
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

void UStartBlackBoard::SetGraffitiCntText(int Cnt)
{
	if (Tb_GraffitiCnt)
		Tb_GraffitiCnt->SetText(FText::AsNumber(Cnt));
}
