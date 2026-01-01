// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FireModeComponent.generated.h"


UCLASS(Abstract, Blueprintable, ClassGroup=(Weapon), meta=(BlueprintSpawnableComponent))
class TOPDOWNSHOOTER_API UFireModeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFireModeComponent();

	virtual void Fire() PURE_VIRTUAL(UFireModeComponent::Fire, );

protected:

	virtual void BeginPlay() override;

	UPROPERTY()
	class ABaseWeapon* OwningWeapon;
};
