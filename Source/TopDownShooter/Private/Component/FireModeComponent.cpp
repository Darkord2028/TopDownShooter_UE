// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/FireModeComponent.h"
#include "Weapon/BaseWeapon.h"

// Sets default values for this component's properties
UFireModeComponent::UFireModeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFireModeComponent::BeginPlay()
{
	Super::BeginPlay();
	OwningWeapon = Cast<ABaseWeapon>(GetOwner());
}
