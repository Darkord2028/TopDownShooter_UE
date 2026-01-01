// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AmmoComponent.generated.h"


UCLASS( ClassGroup=(Weapon), meta=(BlueprintSpawnableComponent) )
class TOPDOWNSHOOTER_API UAmmoComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAmmoComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	bool CanFire() const;
	void ConsumeAmmo();
	void Reload();

	int32 GetCurrentMagAmmo() const { return CurrentMagAmmo; }

private:
	UPROPERTY(VisibleAnywhere)
	int32 CurrentMagAmmo;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxMagAmmo = 30;

	UPROPERTY(VisibleAnywhere)
	int32 CurrentReserveAmmo;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxReserveAmmo = 120;

};
