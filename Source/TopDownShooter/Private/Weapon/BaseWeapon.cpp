// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseWeapon.h"
#include "Component/FireModeComponent.h"
#include "Component/FireModeComponent/FireMode_Hitscan.h"
#include "Component/AmmoComponent.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Ammo = CreateDefaultSubobject<UAmmoComponent>(TEXT("Ammo Component"));
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	FireMode = FindComponentByClass<UFireModeComponent>();
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ABaseWeapon::TryFire()
{
	if (!Ammo || !Ammo->CanFire() || !FireMode)
	{
		return false;
	}

	StartFire();
	return true;
}

void ABaseWeapon::StartFire()
{
	if (bIsFiring) return;

	bIsFiring = true;

	HandleFire();

	GetWorld()->GetTimerManager().SetTimer(
		FireTimerHandle,
		this,
		&ABaseWeapon::HandleFire,
		FireRate,
		true
	);
}

void ABaseWeapon::HandleFire()
{
	if (!Ammo || !Ammo->CanFire() || !FireMode)
	{
		StopFire();
		return;
	}

	FireMode->Fire();
	Ammo->ConsumeAmmo();
}

void ABaseWeapon::StopFire()
{
	bIsFiring = false;
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}

void ABaseWeapon::Reload()
{
	Ammo->Reload();
}

int32 ABaseWeapon::GetCurrentMagAmmo() const
{
	return Ammo->GetCurrentMagAmmo();
}

