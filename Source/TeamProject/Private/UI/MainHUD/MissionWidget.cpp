#include "UI/MainHUD/MissionWidget.h"
#include "Components/TextBlock.h"

void UMissionWidget::UpdateAboveNumber(int Num)
{
	if (AboveNumber)
		AboveNumber->SetText(FText::AsNumber(Num));
}

void UMissionWidget::UpdateTotalNumber(int Num)
{
	if (TotalNumber)
		TotalNumber->SetText(FText::AsNumber(Num));
}
