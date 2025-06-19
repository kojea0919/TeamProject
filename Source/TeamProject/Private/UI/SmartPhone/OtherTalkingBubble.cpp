// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SmartPhone/OtherTalkingBubble.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Fonts/FontMeasure.h"

void UOtherTalkingBubble::SetInputText(const FText & InputText)
{
	if (Tb_InputText && Sb_TalkingBubbleFrame)
	{
		Tb_InputText->SetText(InputText);

		FSlateFontInfo FontInfo = Tb_InputText->GetFont();

		TSharedRef<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
		FVector2D MeasuredSize = FontMeasure->Measure(InputText, FontInfo);

		Sb_TalkingBubbleFrame->SetWidthOverride( MeasuredSize.X + WidthPaddingSize);		
	}
}

void UOtherTalkingBubble::SetNickName(const FString & NickName)
{
	if (Tb_NickName)
	{
		Tb_InputText->SetText(FText::FromString(NickName));
	}
}

