// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Object/Actor/BaseWeapon.h"

#include "Chaos/AABBTree.h"
#include "Components/BoxComponent.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicates(true);
}

void ABaseWeapon::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap Detected: %s"), *OtherActor->GetName());
	APawn* WeaponOwningPawn = GetInstigator<APawn>();

	if (!OtherActor || OtherActor == this || OtherActor == WeaponOwningPawn)
	{
		return;
	}

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("ExecuteIfBound 호출: %s"), *OtherActor->GetName());
		if (OnWeaponHitTarget.IsBound())
		{
			OnWeaponHitTarget.Execute(OtherActor, SweepResult);
		}
		// OnWeaponHitTarget.ExecuteIfBound(OtherActor, SweepResult);
	}
}

void ABaseWeapon::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* WeaponOwningPawn = GetInstigator<APawn>();

	if (APawn* HitPawn = Cast<APawn>(OtherActor))
	{
		OnWeaponPulledFromTarget.ExecuteIfBound(OtherActor);
	}
}

