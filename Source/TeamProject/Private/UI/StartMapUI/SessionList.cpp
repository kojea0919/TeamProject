#include "UI/StartMapUI/SessionList.h"
#include "Components/TextBlock.h"

void USessionList::SetHostName(FString Name)
{
	if (Tb_HostName)
	{
		Tb_HostName->SetText(FText::FromString(Name));
	}
}

void USessionList::SetSessionPlayerNumber(int8 Number)
{
	if (Tb_SessionPlayerNumber)
	{
		FString PlayerNumberText= FString::FromInt(Number);
		PlayerNumberText += FString(TEXT(" / 6"));
		Tb_SessionPlayerNumber->SetText(FText::FromString(PlayerNumberText));
	}
}
