// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SmartPhone/OtherTalkingBubble.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
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
		Tb_NickName->SetText(FText::FromString(NickName));
	}
}

void UOtherTalkingBubble::SetImage(UMaterialInstanceDynamic* Material)
{
	if (Sb_Frame && Sb_TalkingBubbleFrame && Img_TalkingBubble && Tb_InputText && Material)
	{
		Sb_Frame->SetHeightOverride(345);
		Sb_TalkingBubbleFrame->SetWidthOverride(300);
		Sb_TalkingBubbleFrame->SetHeightOverride(300);

		FSlateBrush Brush;
		Brush.SetResourceObject(Material); 
		Brush.DrawAs = ESlateBrushDrawType::Image;
		Brush.SetImageSize(UE::Slate::FDeprecateVector2DParameter(200.f,200.f));

		Img_TalkingBubble->SetBrush(Brush);
	}
}

void UOtherTalkingBubble::SetProfile(bool IsTaggerTalkingBubble)
{
	if (nullptr == Img_RunnerProfile || nullptr == Img_TaggerProfile)
		return;
	
	if (IsTaggerTalkingBubble)
	{
		Img_RunnerProfile->SetVisibility(ESlateVisibility::Hidden);
		Img_TaggerProfile->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Img_RunnerProfile->SetVisibility(ESlateVisibility::Visible);
		Img_TaggerProfile->SetVisibility(ESlateVisibility::Hidden);
	}
}

