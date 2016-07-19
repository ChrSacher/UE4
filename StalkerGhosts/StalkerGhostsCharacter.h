// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "WeaponComponent.h"
#include "GameFramework/Character.h"
#include "InventoryComponent.h"
#include "StalkerCharacterAnim.h"
#include "InteractInterface.h"
#include "CharacterAttributes.h"
#include "DamageInterface.h"
#include "DataTables.h"
#include "GrenadeComponent.h"
#include "WeaponComponent.h"
#include "StalkerGhostsCharacter.generated.h"

class UInputComponent;
UCLASS(config=Game)
class AStalkerGhostsCharacter : public ACharacter , public IDamageInterface
{

	GENERATED_BODY()
	
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	
public:
	AStalkerGhostsCharacter();

	

	virtual void BeginPlay();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
		float BaseTurnRate;

	

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UGrenadeComponent* currentGrenade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		class USkeletalMeshComponent* Mesh1P;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GInventory)
		UStalkerCharacterAnim* characterAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GInventory)
		UInventoryComponent* currentInventory;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GInventory)
		UCharacterAttributes* currentAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		UDamageComponent* damageComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GInventory)
		FTimerHandle staminaHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GInventory)
		FTimerHandle fireHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GInventory)
		FTimerHandle speedHandle;

	virtual void doDamage(float suggestedDamage, DamageBodyPart BodyPart, EDamageType type, FVector veloc, FVector location) override;
	UFUNCTION(BlueprintCallable, Category = "Event")
		void startDamage(FString bonename, ABullet* causer) override;
	virtual void takeGrenadeDamage(AGrenade* Causer);
	virtual void startShrapnelDamage(FString bonename, AGrenade* causer) ;


protected:
	
	/** Fires a projectile. */
	void OnFire();
	void OffFire();

	void Fire();

	
	/** Reload*/
	void OnReload();
	void offReload();

	void changeWeapon(FString& ID);
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

	void onGrenade();
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

