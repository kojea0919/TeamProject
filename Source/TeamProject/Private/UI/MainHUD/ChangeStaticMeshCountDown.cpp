#include "UI/MainHUD/ChangeStaticMeshCountDown.h"
#include "Components/TextBlock.h"

void UChangeStaticMeshCountDown::SetCountDownText(int Second)
{
	if (Tb_CountDown)
		Tb_CountDown->SetText(FText::AsNumber(Second));
}
