#include "UI/StartMapUI/ProgressSquare.h"
#include "Components/Image.h"

void UProgressSquare::SetProgress(float Time)
{
	if (Img_ProgressSquare)
	{
		if (UMaterialInstanceDynamic * DynMaterial = Img_ProgressSquare->GetDynamicMaterial())
		{
			Time = FMath::Clamp(Time, 0.0f, 1.0f);			
			DynMaterial->SetScalarParameterValue(TEXT("PlayingTime"),Time);
		}
	}
}

void UProgressSquare::InitProgress()
{
	if (Img_ProgressSquare)
	{
		if (UMaterialInstanceDynamic * DynMaterial = Img_ProgressSquare->GetDynamicMaterial())
		{
			DynMaterial->SetScalarParameterValue(TEXT("PlayingTime"),0);
		}
	}
}

void UProgressSquare::NativeConstruct()
{
	Super::NativeConstruct();

	if (ProgressSquareMaterial)
	{
		UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(ProgressSquareMaterial, this);
		Img_ProgressSquare->SetBrushFromMaterial(DynMat);
	}
}
	
