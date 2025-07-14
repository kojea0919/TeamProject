// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SmartPhone/SelfTalkingBubble.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Fonts/FontMeasure.h"

void USelfTalkingBubble::SetText(FText InputText)
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

void USelfTalkingBubble::SetImage(UMaterialInstanceDynamic* Material)
{
	if (Sb_Frame && Sb_TalkingBubbleFrame && Img_TalkingBubble && Tb_InputText && Material)
	{
		Sb_Frame->SetHeightOverride(330);
		Sb_TalkingBubbleFrame->SetWidthOverride(300);
		Sb_TalkingBubbleFrame->SetHeightOverride(300);

		FSlateBrush Brush;
		Brush.SetResourceObject(Material); 
		Brush.DrawAs = ESlateBrushDrawType::Image;
		Brush.SetImageSize(UE::Slate::FDeprecateVector2DParameter(200.f,200.f));

		Img_TalkingBubble->SetColorAndOpacity(FLinearColor::White);
		Img_TalkingBubble->SetBrush(Brush);		
	}
}
