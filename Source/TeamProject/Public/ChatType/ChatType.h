#pragma once
#include "ChatType.generated.h"

UENUM(BlueprintType)
enum class EEmojiType : uint8
{
	IDontKnow,
	LetsGo,
	There,
	Tire,
	Smile,
	KillU,
	None
};

UENUM(BlueprintType)
enum class EChatMessageType : uint8
{
	Text,
	Emoji
};

USTRUCT(BlueprintType)
struct TEAMPROJECT_API FChatType
{
	GENERATED_BODY()
	
	UPROPERTY()
	EChatMessageType MessageType = EChatMessageType::Text;

	UPROPERTY()
	FText Text = FText::GetEmpty();

	UPROPERTY()
	EEmojiType Emoji = EEmojiType::None;

	UPROPERTY()
	bool IsTagger = false;
};