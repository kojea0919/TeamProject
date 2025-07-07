// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectObject/NiagaraEffect/BaseWaterGunBeamEffectActor.h"
#include "EffectObject/NiagaraEffect/BaseWaterGunHitEffectActor.h"

#include "NiagaraComponent.h"
#include "EffectObjectPool/EffectObjectPoolSubSystem.h"
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

	BeamLengthBck = BeamLength;
	
	NiagaraComp->SetVariableFloat("User.size", SizeCorrect);

	SetActorTickEnabled(false);

	//EffectSetUp();
}

void ABaseWaterGunBeamEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("BeamActor Active"));
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

	CheckCollision();

	//BeamControl(BeamLength);
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
	HitEffectActorInstance->ReturnEffectActor();
	HitEffectActorInstance = nullptr;
	SetActorTickEnabled(false);
	//NiagaraComp->Deactivate();
	ReturnToObjectPool();;
}

void ABaseWaterGunBeamEffectActor::EffectSetUp()
{
	SetActorTickEnabled(true);
	
	BeamLength = BeamLengthBck;
	
	NiagaraComp->SetVariableVec3(FName(TEXT("User.beamEnd")), FVector(BeamLength, 0.0f, 0.0f));

	if (IsValid(BeamStartActor))
	{
		SetActorTransform(BeamStartActor->GetActorTransform());
	}

	if (HitEffectActorInstance == nullptr)
	{
		HitEffectActorInstance = Cast<ABaseWaterGunHitEffectActor>(GetWorld()->GetSubsystem<UEffectObjectPoolSubSystem>()->GetEffectObject(HitEffectActor));
		//HitEffectActorInstance = GetWorld()->SpawnActor<ABaseWaterGunHitEffectActor>(HitEffectActor);
	}
	
	
	if (HitEffectActorInstance != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("HitActor Cast Complete"));
		SetHitEffectActive(true);
		HitEffectActorInstance->SetEffectActorSize(SizeCorrect * 3);
	}
	
	StartLoop();
}

void ABaseWaterGunBeamEffectActor::CheckCollision_Implementation()
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

	Multicast_ApplyCollision(OutResult);
}

void ABaseWaterGunBeamEffectActor::Multicast_ApplyCollision_Implementation(FHitResult OutResult)
{
	if (HitEffectActorInstance == nullptr)
		return;
	
	if (!OutResult.ImpactPoint.Equals(FVector::ZeroVector, 0.0001))
	{
		if (IsValid(OutResult.GetActor()))
		{
			HitEffectActorInstance->SetActorLocation(OutResult.ImpactPoint);
			UE_LOG(LogTemp, Warning, TEXT("Hit Location : %s"), *OutResult.ImpactPoint.ToString());
			UE_LOG(LogTemp, Warning, TEXT("HitEffectActorInstance Location : %s"), *HitEffectActorInstance->GetActorLocation().ToString());
			SetHitEffectActive(true);
			UE_LOG(LogTemp, Warning, TEXT("Is Hidden : %s"), HitEffectActorInstance->IsHidden() ? TEXT("True") : TEXT("False"));
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Collision"));
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

	NiagaraComp->SetVariableVec3(FName(TEXT("User.beamEnd")), FVector(BeamLength - 50.0f, 0.0f, 0.0f));
}

void ABaseWaterGunBeamEffectActor::BeamControl(float NewBeamLength)
{
	NiagaraComp->SetVariableVec3(FName(TEXT("User.beamEnd")), FVector(NewBeamLength - 50.0f, 0.0f, 0.0f));
}

void ABaseWaterGunBeamEffectActor::SetHitEffectActive(bool IsActive)
{
	if (HitEffectActorInstance == nullptr)
	{
		return;
	}
	
	HitEffectActorInstance->EffectSetActive(IsActive);
}
