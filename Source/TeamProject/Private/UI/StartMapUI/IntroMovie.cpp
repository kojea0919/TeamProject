#include "UI/StartMapUI/IntroMovie.h"
#include "UI/StartMapUI/ProgressSquare.h"

void UIntroMovie::SetSquareProgressTime(float Time)
{
	if (WB_ProgressSquare)
		WB_ProgressSquare->SetProgress(Time);
}

void UIntroMovie::InitSquareProgress()
{
	if (WB_ProgressSquare)
		WB_ProgressSquare->InitProgress();
}
