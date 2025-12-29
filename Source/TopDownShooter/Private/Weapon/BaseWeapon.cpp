// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseWeapon.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	MagCapacity = 30;
	MaxReserveAmmo = 120;

	CurrentMagAmmo = MagCapacity;
	CurrentReserveAmmo = MaxReserveAmmo;

	bCanFire = true;
	bIsReloading = false;

}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseWeapon::ConsumeAmmo()
{
	CurrentMagAmmo = FMath::Clamp(CurrentMagAmmo - 1, 0, MagCapacity);
}

void ABaseWeapon::OnFireCooldownComplete()
{
	bCanFire = true;
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseWeapon::WeaponShoot()
{
	if (!bCanFire)
	{
		if (CurrentMagAmmo <= 0)
		{
			Reload();
		}
		return;
	}

	ConsumeAmmo();
	
	bCanFire = false;

	GetWorld()->GetTimerManager().SetTimer(
		FireRateTimerHandle,
		this,
		&ABaseWeapon::OnFireCooldownComplete,
		FireRate,
		false
	);

}

void ABaseWeapon::Reload()
{
}

