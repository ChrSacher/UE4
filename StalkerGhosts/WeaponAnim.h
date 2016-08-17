// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "WeaponAnim.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class WeaponMode: uint8
{
	SINGLE,
	BURST,
	AUTO
};
UENUM(BlueprintType)
enum class WeaponStance : uint8
{
	FIRING,
	IDLE,
	RELOADING,
	JAMMED,
	OVERHEATED,
	EMPTY
};
UCLASS()
class STALKERGHOSTS_API UWeaponAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		WeaponStance weaponStance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		WeaponMode weaponMode;
	
	
};
