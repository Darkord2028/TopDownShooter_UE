#include "Component/AmmoComponent.h"
#include "Component/FireModeComponent.h"
#include "Weapon/BaseWeapon.h"
#include "Component/WeaponComponent.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UWeaponComponent::EquipWeapon()
{
}

bool UWeaponComponent::TryFire()
{
	return PrimaryWeapon && PrimaryWeapon->TryFire();
}

void UWeaponComponent::StartFire()
{
	if (PrimaryWeapon)
	{
		PrimaryWeapon->StartFire();
	}
}

void UWeaponComponent::StopFire()
{
	if (PrimaryWeapon)
	{
		PrimaryWeapon->StopFire();
	}
}

void UWeaponComponent::Reload()
{
	if (PrimaryWeapon)
	{
		PrimaryWeapon->Reload();
	}
}

float UWeaponComponent::GetPrimaryWeaponFireRate()
{
	return PrimaryWeapon->GetFireRate();
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	PrimaryWeapon = GetWorld()->SpawnActor<ABaseWeapon>(DefaultWeaponClass);
	PrimaryWeapon->SetOwner(GetOwner());
	
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

