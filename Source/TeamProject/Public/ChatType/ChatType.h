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
	EChatMessageType MessageType;

	UPROPERTY()
	FText Text;

	UPROPERTY()
	EEmojiType Emoji;
};