// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/FireModeComponent.h"
#include "FireMode_Hitscan.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Weapon), meta = (BlueprintSpawnableComponent))
class TOPDOWNSHOOTER_API UFireMode_Hitscan : public UFireModeComponent
{
	GENERATED_BODY()

public:
	virtual void Fire() override;
	
};
