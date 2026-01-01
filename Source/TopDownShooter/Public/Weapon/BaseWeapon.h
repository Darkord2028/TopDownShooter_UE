// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "BaseWeapon.generated.h"

class UAmmoComponent;
class UFireModeComponent;

UCLASS(Abstract)
class TOPDOWNSHOOTER_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseWeapon();

	float GetFireRate() const { return FireRate; }
	int32 GetCurrentMagAmmo() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float FireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	float Damage;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Weapon|State")
	bool bIsAutomatic;

	bool bIsFiring = false;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UAmmoComponent* Ammo;

	UPROPERTY(VisibleAnywhere)
	UFireModeComponent* FireMode;

	FTimerHandle FireTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool TryFire();
	void StartFire();
	void StopFire();
	void Reload();

private:
	void HandleFire();

};
