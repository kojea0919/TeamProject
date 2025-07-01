// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectObject/NiagaraEffect/BaseWaterGunEffectActor.h"

#include "NiagaraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

ABaseWaterGunEffectActor::ABaseWaterGunEffectActor()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	BeamComponent = CreateDefaultSubobject<UNiagaraComponent>("Beam");
	BeamComponent->SetupAttachment(Root);
	BeamComponent->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
	BeamComponent->SetRelativeScale3D(FVector(1.2f));
	HitComponent = CreateDefaultSubobject<UNiagaraComponent>("Hit");
	HitComponent->SetupAttachment(Root);
	HitComponent->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
	HitComponent->SetRelativeScale3D(FVector(1.0f));

	BeamComponent->SetVariableVec3("User.beamEnd", FVector(BeamLength, 0.0f, 0.0f));

	BeamComponent->SetVariableFloat("User.size", SizeCorrect);
	HitComponent->SetVariableFloat("User.size", SizeCorrect);
}

void ABaseWaterGunEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	BeamLengthBck = BeamLength;

	if (IsValid(BeamStartActor))
	{
		SetActorTransform(BeamStartActor->GetActorTransform());
	}

	StartLoop();
}

void ABaseWaterGunEffectActor::Tick(float DeltaTime)
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

	if (OutResult.ImpactPoint.Equals(FVector::ZeroVector, 0.0001))
	{
		if (IsValid(OutResult.GetActor()))
		{
			HitComponent->SetWorldLocation(OutResult.ImpactPoint);
			HitComponent->Activate();
			BeamLength = (OutResult.ImpactPoint - GetActorLocation()).Size();
		}
		else
		{
			HitComponent->Deactivate();
			if (bIsBeamShot)
				BeamLength = BeamLength + (BeamSpeed * GetWorld()->GetDeltaSeconds());
			else
			{
				if (BeamLength < BeamLengthBck)
					BeamLength = BeamLength + (ReturnSpeed * (GetWorld()->GetDeltaSeconds()));

				else
				{
					if (BeamLength < BeamLengthBck)
						BeamLength = BeamLength - (ReturnSpeed * (GetWorld()->GetDeltaSeconds()));
				}
			}
		}
	}

	BeamControl(BeamLength);
}

void ABaseWaterGunEffectActor::StartLoop()
{
	GetWorldTimerManager().SetTimer(LoopTimerHandle, this, &ABaseWaterGunEffectActor::LoopExecution, Duration, false);
}

void ABaseWaterGunEffectActor::LoopExecution()
{
	if (bIsLoop)
		StartLoop();
	else
	{
		FinishLoop();
	}
}

void ABaseWaterGunEffectActor::FinishLoop()
{
	BeamComponent->Deactivate();
	Destroy();
}

FHitResult ABaseWaterGunEffectActor::CheckCollision() const
{
	FVector StartVector = Root->GetComponentLocation();
	FVector EndVector = StartVector + (UKismetMathLibrary::GetForwardVector(Root->GetComponentRotation()) * BeamLength);

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
		EDrawDebugTrace::ForDuration,
		OutResult,
		true
	);

	return OutResult;
}

void ABaseWaterGunEffectActor::BeamControl(float NewBeamLength)
{
	BeamComponent->SetVariableVec3("User.beamEnd", FVector(NewBeamLength - 25.0f, 0.0f, 0.0f));
}
