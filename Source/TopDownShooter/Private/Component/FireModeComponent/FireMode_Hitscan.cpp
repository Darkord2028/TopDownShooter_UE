// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/FireModeComponent/FireMode_Hitscan.h"
#include "Weapon/BaseWeapon.h"

void UFireMode_Hitscan::Fire()
{
	if (!OwningWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("FireMode_Hitscan: Owning Weapon is null"));
	}

	AActor* WeaponOwner = OwningWeapon->GetOwner();

	UE_LOG(
		LogTemp,
		Log,
		TEXT("FIRE CALLED | Weapon: %s | Owner: %s"),
		*OwningWeapon->GetName(),
		WeaponOwner ? *WeaponOwner->GetName() : TEXT("None")
	);
}
