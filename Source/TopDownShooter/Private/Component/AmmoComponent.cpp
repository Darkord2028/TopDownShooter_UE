// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/AmmoComponent.h"

// Sets default values for this component's properties
UAmmoComponent::UAmmoComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UAmmoComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentMagAmmo = MaxMagAmmo;
	CurrentReserveAmmo = MaxReserveAmmo;
}

bool UAmmoComponent::CanFire() const
{
	if (CurrentMagAmmo > 0)
	{
		return true;
	}
	return false;
}

void UAmmoComponent::ConsumeAmmo()
{
	if (CurrentMagAmmo <= 0)
	{
		return;
	}

	CurrentMagAmmo--;

	if (CurrentMagAmmo == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Magazine empty"));
	}
}


void UAmmoComponent::Reload()
{
	if (CurrentReserveAmmo <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No reserve ammo"));
		return;
	}

	if (CurrentMagAmmo >= MaxMagAmmo)
	{
		return; // Mag already full
	}

	int32 NeededAmmo = MaxMagAmmo - CurrentMagAmmo;
	int32 AmmoToReload = FMath::Min(NeededAmmo, CurrentReserveAmmo);

	CurrentMagAmmo += AmmoToReload;
	CurrentReserveAmmo -= AmmoToReload;

	UE_LOG(
		LogTemp,
		Log,
		TEXT("Reloaded %d bullets | Mag: %d | Reserve: %d"),
		AmmoToReload,
		CurrentMagAmmo,
		CurrentReserveAmmo
	);
}


