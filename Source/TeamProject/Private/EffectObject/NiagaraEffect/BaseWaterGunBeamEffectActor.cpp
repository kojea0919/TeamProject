// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectObject/NiagaraEffect/BaseWaterGunBeamEffectActor.h"
#include "EffectObject/NiagaraEffect/BaseWaterGunHitEffectActor.h"

#include "NiagaraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

ABaseWaterGunBeamEffectActor::ABaseWaterGunBeamEffectActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	NiagaraComp->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
	NiagaraComp->SetRelativeScale3D(FVector(1.2f));
}

void ABaseWaterGunBeamEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	NiagaraComp->SetVariableVec3(FName(TEXT("User.beamEnd")), FVector(BeamLength, 0.0f, 0.0f));

	NiagaraComp->SetVariableFloat("User.size", SizeCorrect);
	
	BeamLengthBck = BeamLength;

	if (IsValid(BeamStartActor))
	{
		SetActorTransform(BeamStartActor->GetActorTransform());
	}
	
	HitEffectActorInstance = GetWorld()->SpawnActor<ABaseWaterGunHitEffectActor>(HitEffectActor, GetActorTransform());
	if (HitEffectActorInstance != nullptr)
	{
		SetHitEffectActive(false);
		HitEffectActorInstance->SetEffectActorSize(SizeCorrect * 5);
	}

	
	StartLoop();
}

void ABaseWaterGunBeamEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(BeamStartActor))
	{
		SetActorTransform(BeamStartActor->GetActorTransform());
		AddActorLocalOffset(BeamStartOffset);
	}

	if (IsValid(BeamEndActor))
	{
		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), BeamEndActor->GetActorLocation());
		SetActorRotation(NewRotation);
	}

	FHitResult OutResult = CheckCollision();

	
	if (!OutResult.ImpactPoint.Equals(FVector::ZeroVector, 0.0001))
	{
		if (IsValid(OutResult.GetActor()))
		{
			HitEffectActorInstance->SetActorLocation(OutResult.ImpactPoint);
			SetHitEffectActive(true);
			BeamLength = (OutResult.ImpactPoint - GetActorLocation()).Size();
		}
		else
		{
			SetHitEffectActive(false);
			if (bIsBeamShot)
				BeamLength = BeamLength + (BeamSpeed * GetWorld()->GetDeltaSeconds());
			else
			{
				if (BeamLength < BeamLengthBck)
					BeamLength = BeamLength + (ReturnSpeed * (GetWorld()->GetDeltaSeconds()));

				else if (BeamLength > BeamLengthBck)
						BeamLength = BeamLength - (ReturnSpeed * (GetWorld()->GetDeltaSeconds()));
			}
		}
	}

	else
	{
		SetHitEffectActive(false);
		if (bIsBeamShot)
			BeamLength = BeamLength + (BeamSpeed * GetWorld()->GetDeltaSeconds());
		else
		{
			if (BeamLength < BeamLengthBck)
				BeamLength = BeamLength + (ReturnSpeed * (GetWorld()->GetDeltaSeconds()));

			else
			{
				if (BeamLength > BeamLengthBck)
					BeamLength = BeamLength - (ReturnSpeed * (GetWorld()->GetDeltaSeconds()));
			}
		}
	}

	BeamControl(BeamLength);
}

void ABaseWaterGunBeamEffectActor::StartLoop()
{
	GetWorldTimerManager().SetTimer(LoopTimerHandle, this, &ABaseWaterGunBeamEffectActor::LoopExecution, Duration, false);
}

void ABaseWaterGunBeamEffectActor::LoopExecution()
{
	if (bIsLoop)
		StartLoop();
	else
	{
		FinishLoop();
	}
}

void ABaseWaterGunBeamEffectActor::FinishLoop()
{
	NiagaraComp->Deactivate();
	Destroy();
}

FHitResult ABaseWaterGunBeamEffectActor::CheckCollision() const
{
	FVector StartVector = RootComponent->GetComponentLocation();
	FVector EndVector = StartVector + (UKismetMathLibrary::GetForwardVector(RootComponent->GetComponentRotation()) * (BeamLength));

	//UE_LOG(LogTemp, Warning, TEXT("Start Vector : %s, End Vector : %s"), *StartVector.ToString(), *EndVector.ToString());

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = {
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic),
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic),
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)
	};

	TArray<AActor*> IgnoreActors;
	FHitResult OutResult;
	
	UKismetSystemLibrary::CapsuleTraceSingleForObjects(
		GetWorld(),
		StartVector,
		EndVector,
		10.0f,
		10.0f,
		ObjectTypes,
		true,
		IgnoreActors,
		EDrawDebugTrace::None,
		OutResult,
		true
	);

	return OutResult;
}

void ABaseWaterGunBeamEffectActor::BeamControl(float NewBeamLength)
{
	NiagaraComp->SetVariableVec3(FName(TEXT("User.beamEnd")), FVector(NewBeamLength - 50.0f, 0.0f, 0.0f));
	UE_LOG(LogTemp, Warning, TEXT("Beam Length: %f"), NewBeamLength - 25.0f);
}

void ABaseWaterGunBeamEffectActor::SetHitEffectActive(bool IsActive)
{
	if (HitEffectActorInstance == nullptr)
		return;

	HitEffectActorInstance->SetActorHiddenInGame(!IsActive);         // 화면에서 숨기기
	HitEffectActorInstance->SetActorEnableCollision(IsActive);     // 충돌 끄기
	HitEffectActorInstance->SetActorTickEnabled(IsActive); 
}
