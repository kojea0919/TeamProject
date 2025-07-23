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

	if (Btn_GameModeLeft)
		Btn_GameModeLeft->OnClicked.AddDynamic(this,&UStartBlackBoard::ClickGameModeLeftButton);

	if (Btn_GameModeRight)
		Btn_GameModeRight->OnClicked.AddDynamic(this,&UStartBlackBoard::ClickGameModeRightButton);

	if (Btn_TaggerStartTimeDecrease)
		Btn_TaggerStartTimeDecrease->OnClicked.AddDynamic(this,&UStartBlackBoard::ClickTaggerStartTimeDecreaseButton);

	if (Btn_TaggerStartTimeIncrease)
		Btn_TaggerStartTimeIncrease->OnClicked.AddDynamic(this,&UStartBlackBoard::ClickTaggerStartTimeIncreaseButton);
		
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

			int TaggerStartTime = GameMode->GetTaggerStartTime();
			if (CurTime <= TaggerStartTime)
			{
				TaggerStartTime = GameMode->DecreaseTaggerStartTime();
				SetTaggerStartTimeText(TaggerStartTime);
			}
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

void UStartBlackBoard::ClickGameModeLeftButton()
{
	if (GetOwningPlayer() && GetOwningPlayer()->HasAuthority())
	{
		if (Tb_GameMode)
		{
			FText CurMode = Tb_GameMode->GetText();
			if (CurMode.EqualTo(FText::FromString(TEXT("Hide"))))
			{
				Tb_GameMode->SetText(FText::FromString(TEXT("Mission")));
				if ( AMainMapGameMode * GameMode =GetWorld()->GetAuthGameMode<AMainMapGameMode>())
				{
					GameMode->SetCurrentGameMode(true);
				}
			}
		}
	}
}

void UStartBlackBoard::ClickGameModeRightButton()
{
	if (GetOwningPlayer() && GetOwningPlayer()->HasAuthority())
	{
		if (Tb_GameMode)
		{
			FText CurMode = Tb_GameMode->GetText();
			if (CurMode.EqualTo(FText::FromString(TEXT("Mission"))))
			{
				Tb_GameMode->SetText(FText::FromString(TEXT("Hide")));
				if ( AMainMapGameMode * GameMode =GetWorld()->GetAuthGameMode<AMainMapGameMode>())
				{
					GameMode->SetCurrentGameMode(false);
				}
			}
		}
	}
}

void UStartBlackBoard::ClickTaggerStartTimeDecreaseButton()
{
	if (GetOwningPlayer() && GetOwningPlayer()->HasAuthority())
	{
		if (AMainMapGameMode * GameMode = GetWorld()->GetAuthGameMode<AMainMapGameMode>())
		{
			int CurCnt = GameMode->DecreaseTaggerStartTime();
			SetTaggerStartTimeText(CurCnt);
		}
	}
}

void UStartBlackBoard::ClickTaggerStartTimeIncreaseButton()
{
	if (GetOwningPlayer() && GetOwningPlayer()->HasAuthority())
	{
		if (AMainMapGameMode * GameMode = GetWorld()->GetAuthGameMode<AMainMapGameMode>())
		{
			int CurCnt = GameMode->IncreaseTaggerStartTime();
			SetTaggerStartTimeText(CurCnt);
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

void UStartBlackBoard::SetTaggerStartTimeText(int Time)
{
	if (Tb_TaggerStartTime)
	{
		int Min = Time / 60;
		int Sec = Time % 60;

		FString TimeText = FString::FromInt(Min) + " : " + FString::FromInt(Sec);
		Tb_TaggerStartTime->SetText(FText::FromString(TimeText));
	}
}
