// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "WeaponComponent.h"
#include "GameFramework/Character.h"
#include "InventoryComponent.h"
#include "StalkerCharacterAnim.h"
#include "InteractInterface.h"
#include "CharacterAttributes.h"
#include "StalkerGhostsCharacter.generated.h"

class UInputComponent;
UCLASS(config=Game)
class AStalkerGhostsCharacter : public ACharacter
{

	GENERATED_BODY()
	
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	
public:
	AStalkerGhostsCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	virtual void BeginPlay();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		float sprintSpeed = 800;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		float jogSpeed = 400;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		float walkSpeed = 200;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		float currentSpeed = 200;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		float stamina = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool isStaminaRegenerable = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		float maxStamina = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		float sprintCost = 0.1f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		float staminaRegen = 0.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool isSprinting = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		Movement stance;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		Movement prevStance;
	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;


	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UWeaponComponent* currentWeapon;

	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GInventory)
		UStalkerCharacterAnim* characterAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GInventory)
		UInventoryComponent* currentInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GInventory)
		UCharacterAttributes* currentAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GInventory)
	FTimerHandle staminaHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GInventory)
	FTimerHandle fireHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GInventory)
	FTimerHandle speedHandle;
protected:
	
	/** Fires a projectile. */
	void OnFire();
	void OffFire();

	void Fire();

	
	/** Reload*/
	void OnReload();
	void offReload();

	void changeWeapon(UWeaponComponent* newWeapon);
	bool checkMag(TArray<UItemBase*> Items);
	//sprinting
	void OnSprint();
	void OffSprint();

	//crouching
	void OnCrouch();
	void OffCrouch();

	//inventory
	void OnInventory();
	void OffInventory();

	//prone
	void OnProne();
	void OffProne();

	//jumping
	void Jump() override;
	void StopJumping() override;

	void changeStance(Movement newStance);


	//interact
	void onInteract();
	UFUNCTION()
		void Sprint();

	void regainStamina();
	void OnWalk();
	void OffWalk();
	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface


public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

