#include "UI/MainHUD/PlayerItemSlot.h"

#include "DDSFile.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"
#include "Fonts/FontMeasure.h"
#include "Components/Image.h"
#include "Map/Object/Actor/BaseObject.h"
#include "UI/MainHUD/WeaponStatusWidget.h"

void UPlayerItemSlot::NativeConstruct()
{
	Super::NativeConstruct();
	
	ItemName = FText::FromString(TEXT("Empty"));

	if (WeaponStatusWidget)
		WeaponStatusWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerItemSlot::SetSlotTypeText(FText SlotTypeText)
{
	if (Tb_SlotTypeText)
		Tb_SlotTypeText->SetText(SlotTypeText);
}

void UPlayerItemSlot::SetItemName(FText Name)
{
	if (nullptr == Tb_ItemName || nullptr == Sb_Frame)
		return;

	ItemName = Name;
	Tb_ItemName->SetText(Name);

	FSlateFontInfo FontInfo = Tb_ItemName->GetFont();

	TSharedRef<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
	FVector2D MeasuredSize = FontMeasure->Measure(Name, FontInfo);

	TargetWidthSize = MeasuredSize.X + 20;

	CurWidthSize = Sb_Frame->GetWidthOverride();	

	if (TargetWidthSize > CurWidthSize)
	{
		IsIncreaseSize = true;
	}
	else if (TargetWidthSize < CurWidthSize)
	{
		IsIncreaseSize = false;
	}
	else
		return;

	GetWorld()->GetTimerManager().SetTimer(SizeChangeTimerHandle, this, &UPlayerItemSlot::ChangeSize, 0.01f, true);
}

void UPlayerItemSlot::SetItemInfo(const FItemData& InItemData)
{
	if (Img_Item && InItemData.ItemTexture)
	{
		FSlateBrush NewBrush;
		NewBrush.SetResourceObject(InItemData.ItemTexture);
		NewBrush.TintColor = FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.0f)); // 알파값 설정
		Img_Item->SetBrush(NewBrush);
	}

	if (Tb_ItemName)
	{
		SetItemName(FText::FromName(InItemData.ItemName));
	}

	if (WeaponStatusWidget && InItemData.Item.IsValid())
	{
		if (!InItemData.Item->GetDescription().IsEmpty())
		{
			WeaponStatusWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UPlayerItemSlot::UpdateWeaponStatusUI()
{
	if (WeaponStatusWidget)
		WeaponStatusWidget->UpdateWeaponStatusUI();
}

void UPlayerItemSlot::ChangeSize()
{
	CurWidthSize = IsIncreaseSize ? CurWidthSize + SizeChangeSpeed : CurWidthSize - SizeChangeSpeed;

	if ((IsIncreaseSize && TargetWidthSize <= CurWidthSize) ||
		(!IsIncreaseSize && TargetWidthSize >= CurWidthSize))
	{
		GetWorld()->GetTimerManager().ClearTimer(SizeChangeTimerHandle);
		CurWidthSize = TargetWidthSize;
		return;
	}	

	if (Sb_Frame)
		Sb_Frame->SetWidthOverride(CurWidthSize + 25);

	if (Sb_Text)
		Sb_Text->SetWidthOverride(CurWidthSize);

}
