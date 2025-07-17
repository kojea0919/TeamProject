#include "UI/MainHUD/PlayerMainHUD.h"
#include "Components/Image.h"
#include "UI/MainHUD/SpeedBar.h"
#include "UI/MainHUD/StaminaBar.h"
#include "UI/MainHUD/PlayerStateText.h"
#include "UI/MainHUD/PlayerItemSlot.h"
#include "Components/TextBlock.h"
#include "UI/SmartPhone/SmartPhone.h"
#include "GameFrameWork/MainMap/MainMapPlayerController.h"

void UPlayerMainHUD::NativeConstruct()
{
	Super::NativeConstruct();

	if (W_PlayerHandSlot)
	{		
		W_PlayerHandSlot->SetSlotTypeText(FText::FromString(TEXT("Hands")));
	}

	// if (W_PlayerBagSlot)
	// {
	// 	W_PlayerBagSlot->SetSlotTypeText(FText::FromString(TEXT("Bag")));
	// }

}

void UPlayerMainHUD::SetHandSlotText(FText Text)
{
	if (W_PlayerHandSlot)
		W_PlayerHandSlot->SetItemName(Text);
}

void UPlayerMainHUD::SetHandSlot(const FItemData& InItemData)
{
	if (W_PlayerHandSlot && InItemData.ItemTexture)
	{
		W_PlayerHandSlot->SetItemInfo(InItemData);
	}
}

// void UPlayerMainHUD::SetBagSlotText(FText Text)
// {
// 	if (W_PlayerBagSlot)
// 		W_PlayerBagSlot->SetItemName(Text);
// }
//
// void UPlayerMainHUD::ChangeItemSlot()
// {
// 	if (nullptr == W_PlayerHandSlot || nullptr == W_PlayerBagSlot)
// 		return;
//
// 	FText HandSlotText = W_PlayerHandSlot->GetItemName();
// 	FText BagSlotText = W_PlayerBagSlot->GetItemName();
//
// 	W_PlayerHandSlot->SetItemName(BagSlotText);
// 	W_PlayerBagSlot->SetItemName(HandSlotText);
// }

void UPlayerMainHUD::UpdateRemainTime(int Second)
{
	if (Tb_RemainMinute && Tb_RemainSecond)
	{
		int RemainMinute = Second / 60;
		int RemainSecond = Second - RemainMinute * 60;

		Tb_RemainMinute->SetText(FText::AsNumber(RemainMinute));
		Tb_RemainSecond->SetText(FText::AsNumber(RemainSecond));
	}
}

void UPlayerMainHUD::SetActiveMic(bool Active)
{
	if (Active)
	{
		Img_MicOn->SetVisibility(ESlateVisibility::Visible);
		Img_MicOff->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Img_MicOn->SetVisibility(ESlateVisibility::Hidden);
		Img_MicOff->SetVisibility(ESlateVisibility::Visible);	
	}
}

void UPlayerMainHUD::PlayTalkingAnimation()
{
	if (TalkingAnimation)
		PlayAnimation(TalkingAnimation);
}

void UPlayerMainHUD::Init() const
{
	if (nullptr != W_SmartPhone)
		W_SmartPhone->InitSmartPhone();

	if (Tb_PlayerID)
	{
		if (AMainMapPlayerController * PlayerController =
			Cast<AMainMapPlayerController>(GetOwningPlayer()))
		{
			Tb_PlayerID->SetText(FText::FromString(PlayerController->GetNickName()));
		}
	}
}

void UPlayerMainHUD::AddAllChatSelfMessage(const FChatType & ChatType)
{
	if (W_SmartPhone)
		W_SmartPhone->AddAllChatSelfMessage(ChatType);
}

void UPlayerMainHUD::AddAllChatOtherMessage(const FChatType & ChatType, const FString& NickName)
{
	if (W_SmartPhone)
		W_SmartPhone->AddAllChatOtherMessage(ChatType,NickName);
}

void UPlayerMainHUD::AddTeamChatSelfMessage(const FChatType & ChatType)
{
	if (W_SmartPhone)
		W_SmartPhone->AddTeamChatSelfMessage(ChatType);
}

void UPlayerMainHUD::AddTeamChatOtherMessage(const FChatType & ChatType, const FString& NickName)
{
	if (W_SmartPhone)
		W_SmartPhone->AddTeamChatOtherMessage(ChatType,NickName);
}

void UPlayerMainHUD::SetPlayerJobText(const FString & JobText)
{
	if (Tb_PlayerJob)
	{
		Tb_PlayerJob->SetText(FText::FromString(JobText));
	}
}

void UPlayerMainHUD::SetPlayerNickName(const FString & NickName)
{
	if (Tb_PlayerID)
	{
		Tb_PlayerID->SetText(FText::FromString(NickName));
	}
}

void UPlayerMainHUD::ClearSmartPhone()
{
	if (W_SmartPhone)
		W_SmartPhone->Clear();
}
