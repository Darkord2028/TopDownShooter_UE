// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "BaseWeapon.generated.h"

UCLASS(Abstract)
class TOPDOWNSHOOTER_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void WeaponShoot();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Reload();

	UFUNCTION(BlueprintCallable, Category = "Weapon|Ammunition")
	int32 GetCurrentMagCapacity() const { return MagCapacity; }

	UFUNCTION(BlueprintCallable, Category = "Weapon|Ammunition")
	int32 GetCurrentMagAmmo() const { return CurrentMagAmmo; }

	UFUNCTION(BlueprintCallable, Category = "Weapon|Ammunition")
	int32 GetMaxReserveAmmo() const { return MaxReserveAmmo; }

	UFUNCTION(BlueprintCallable, Category = "Weapon|Ammunition")
	int32 GetCurrentReserveAmmo() const { return CurrentReserveAmmo; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float FireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Ammunition")
	int32 MagCapacity = 30;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Weapon|Ammunition")
	int32 CurrentMagAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Ammunition")
	int32 MaxReserveAmmo = 120;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Weapon|Ammunition")
	int32 CurrentReserveAmmo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Weapon|State")
	bool bCanFire;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Weapon|State")
	bool bIsReloading;

	void ConsumeAmmo();

	void OnFireCooldownComplete();

	FTimerHandle FireRateTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
