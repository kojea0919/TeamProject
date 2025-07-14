#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatType/ChatType.h"
#include "UI/SmartPhone/SmartPhoneEnumType.h"
#include "Emoji.generated.h"

DECLARE_DELEGATE_OneParam(FOnAddTalkingBubble, EEmojiType)

UCLASS()
class TEAMPROJECT_API UEmoji : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetEmoji();
	
	FORCEINLINE	EEmojiType GetEmojiType() const { return EmojiType; }

	UMaterialInstanceDynamic* GetCopyMaterialInstance();
public:
	FOnAddTalkingBubble OnAddTalkingBubble;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void ClickedEmoji();
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Btn_Emoji;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Img_Preview;

	UPROPERTY(EditAnywhere)
	UMaterialInterface * BaseMaterial;

	UPROPERTY(EditAnywhere)
	EEmojiType EmojiType;
};
