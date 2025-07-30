// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHUD/AimDot.h"
#include "Components/Image.h"

void UAimDot::SetAimImg(UTexture2D* AimImg)
{
	FSlateBrush NewBrush;
	NewBrush.SetResourceObject(AimImg);
	NewBrush.TintColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.0f)); // 알파값 설정
	Img_Aim->SetBrush(NewBrush);
}
