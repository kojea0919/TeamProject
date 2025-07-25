#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Player/Character/BaseType/BaseStructType.h"
#include "PlayerItemSlot.generated.h"

class UTexture2D;
/**
 * 
 */
UCLASS()
class TEAMPROJECT_API UPlayerItemSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	void SetSlotTypeText(FText SlotTypeText);
	void SetItemName(FText Name);

	void SetItemInfo(const FItemData& InItemData);

	FText GetItemName() const { return ItemName; }

	void UpdateWeaponStatusUI();
private:
	void ChangeSize();

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Tb_SlotTypeText;

	UPROPERTY(meta = (BindWidget))
	class USizeBox* Sb_Text;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Tb_ItemName;

	UPROPERTY(meta = (BindWidget))
	class USizeBox* Sb_Frame;

	UPROPERTY(meta = (BindWidget))
	class UImage* Img_Item;

	UPROPERTY(meta= (BindWidget))
	class UWeaponStatusWidget* WeaponStatusWidget;

	FText ItemName;

	//������ ���� ���� ����
	//--------------------------------------
	const float SizeChangeSpeed = 5.f;

	FTimerHandle SizeChangeTimerHandle;

	double TargetWidthSize;
	double CurWidthSize;

	bool IsIncreaseSize;
	//--------------------------------------
};
