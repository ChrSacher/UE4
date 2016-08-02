// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "StalkerCharacterAnim.generated.h"

class AStalkerGhostsCharacter;
UENUM(BlueprintType)
enum class Movement : uint8
{
	SPRINTING,
	WALKING,
	JOGGING, //normal stance
	CROUCHING,
	PRONING
};

UENUM(BlueprintType)
enum class WeaponHolding : uint8
{
	RELOADING,
	IDLE,
	FIRING
};


UCLASS()
class STALKERGHOSTS_API UStalkerCharacterAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		float speed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		float playerYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		float playerPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		bool isJumping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GInventory)
		UStalkerCharacterAnim* characterAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		int32 strafe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		Movement stance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		WeaponHolding wStance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		AStalkerGhostsCharacter* pawn;
};
