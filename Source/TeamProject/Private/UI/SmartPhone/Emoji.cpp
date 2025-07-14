#include "UI/SmartPhone/Emoji.h"
#include "UI/SmartPhone/ChattingRoom.h"
#include "Components/Button.h"
#include "Components/Image.h"

void UEmoji::SetEmoji()
{
	if (BaseMaterial)
	{	
		if (UMaterialInstanceDynamic * EmojiImage =  UMaterialInstanceDynamic::Create(BaseMaterial,this))
		{
			Img_Preview->SetBrushFromMaterial(EmojiImage);
		}
	}
}

UMaterialInstanceDynamic* UEmoji::GetCopyMaterialInstance()
{
	if (UMaterialInstanceDynamic * EmojiImage =  UMaterialInstanceDynamic::Create(BaseMaterial,this))
	{
		return EmojiImage;		
	}
	return nullptr;
}

void UEmoji::NativeConstruct()
{
	Super::NativeConstruct();

	SetEmoji();

	if (Btn_Emoji)
	{
		Btn_Emoji->OnClicked.AddDynamic(this, &UEmoji::ClickedEmoji);
	}
}

void UEmoji::ClickedEmoji()
{
	if (BaseMaterial)
	{	
		if (UMaterialInstanceDynamic * EmojiImage =  UMaterialInstanceDynamic::Create(BaseMaterial,this))
		{
			if (OnAddTalkingBubble.IsBound())
				OnAddTalkingBubble.Execute(EmojiType);
		}
	}
}
