// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/Actor/Graffiti/BaseGraffiti.h"
#include "GameFrameWork/MainMap/MainMapGameState.h"
#include "VisualizeTexture.h"
#include "EffectObject/NiagaraEffect/BaseWaterGunBeamEffectActor.h"
#include "GameTag/STGamePlayTags.h"

ABaseGraffiti::ABaseGraffiti()
{
	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	GraffitiMesh = CreateDefaultSubobject<UStaticMeshComponent>("GraffitiMesh");
	GraffitiMesh->SetupAttachment(Root);

	ObjectTypeTag = STGamePlayTags::Object_Actor_Graffiti;

	Interactable = true;
}

void ABaseGraffiti::BeginPlay()
{
	Super::BeginPlay();

	ABaseWaterGunBeamEffectActor::OnSplashHit.AddDynamic(this, &ABaseGraffiti::OnHitBySplash);

	if (BaseMaterial && GraffitiMesh)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		GraffitiMesh->SetMaterial(0, DynamicMaterial);

		DynamicMaterial->SetScalarParameterValue(FName("Opacity"), MaxRatio);
	}
}

bool ABaseGraffiti::GetIsErased() const
{
	return bIsErased;
}

void ABaseGraffiti::OnHitBySplash(AActor* HitActor)
{
	if (HasAuthority())
		Server_Request_OnSplashHit(HitActor);
}

FText ABaseGraffiti::GetObjectName()
{
	return FText::FromString(TEXT("Graffiti"));
}

FText ABaseGraffiti::GetDescription()
{
	return FText::FromString(bIsErased ? TEXT("Erased") : TEXT("Not Erased"));
}

void ABaseGraffiti::Server_Request_OnSplashHit_Implementation(AActor* HitActor)
{
	if (!bIsErased)
	{
		EraseRatio += EraseSpeed;

		if (EraseRatio >= MaxRatio)
		{
			EraseRatio = MaxRatio;
			bIsErased = true;
			
			if (AMainMapGameState* GameState =  GetWorld()->GetGameState<AMainMapGameState>())
			{
				GameState->DecreaseGraffitiCount();
			}
		}
		
		Apply_OnSplashHit(EraseRatio);
	}
}

void ABaseGraffiti::Apply_OnSplashHit_Implementation(float NewEraseRatio)
{
	EraseRatio = NewEraseRatio;

	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue(FName("Opacity"), MaxRatio - EraseRatio);
	}
}
