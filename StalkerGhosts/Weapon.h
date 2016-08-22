// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Bullet.h"
#include "WeaponAnim.h"
#include "Weapon.generated.h"

struct FWeaponLookUpTable;
class ABullet;
class UBulletItem;
class UWeaponAttachmentItem;
UENUM(BlueprintType)
enum class WeaponAttachmentSlot : uint8
{
	SCOPE,
	MUZZLEBRAKE,
	GRIP,
	NUM
};

USTRUCT(Blueprintable)
struct FRecoilPattern
{
	GENERATED_BODY()
public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		TSubclassOf<UCameraShake> recoilShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		float xOffsetMin = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		float xOffsetMax = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		float yOffsetMin = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		float yOffsetMax = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		bool recoilEnabled = true;
};

USTRUCT(Blueprintable)
struct FDispersionPattern
{
	GENERATED_BODY()
public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		float standardDispersionX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		float standardDispersionY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		float maxDispersionX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		float maxDispersionY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		float perBulletDispersionX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		float perBulletDispersionY;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
		float finalDispersionX;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
		float finalDispersionY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		float spreadDecreaseXPerSecond;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		float spreadDecreaseYPerSecond;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
		float timePerDecrease = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		bool decreaseSpreadWhenFireEnded = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		FTimerHandle decreaseSpreadTimer;
};


USTRUCT(Blueprintable)
struct FWeaponAttachment
{
		GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		WeaponAttachmentSlot slot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		bool isEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		bool isAttached = false;


};

USTRUCT(Blueprintable)
struct FAnimationPattern
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* equipAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* ReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* weaponReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* IdlingAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		UWeaponAnim* animInstance;

};
UENUM(BlueprintType)
enum class WeaponFireMode : uint8
{
	SINGLE,
	BURST,
	AUTO
};

UCLASS()
class STALKERGHOSTS_API AWeapon :  public AActor
{
	GENERATED_BODY()

	DECLARE_DELEGATE(WeaponFire)
public:	
	// Sets default values for this actor's properties
	AWeapon();
	WeaponFire weaponFire;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	UPROPERTY(EditAnywhere, Category = Weapon)
		UParticleSystemComponent* flash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		USkeletalMeshComponent* mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		TArray< FWeaponAttachment> attachedMeshes;

	

	UPROPERTY(EditAnywhere, Category = Magazine)
		int32 ammoCapacity = 30;

	UPROPERTY(EditAnywhere, Category = Magazine)
		UBulletItem* currentlyLoadedBullet;

	UPROPERTY(EditAnywhere, Category = Weapon)
		float muzzleVelocityCoeff = 1.0f;

	UPROPERTY(EditAnywhere, Category = Weapon)
		float reloadTime = 2.0f;

	UPROPERTY(EditAnywhere, Category = Weapon)
		float fireRate = 600.0f;

	UPROPERTY(EditAnywhere, Category = Weapon)
		float equipTime = 2.0f;

	UPROPERTY(EditAnywhere, Category = Weapon)
		bool mustEndFire = false;
	UPROPERTY(EditAnywhere, Category = Weapon)
		bool canEndFire = true;
	UPROPERTY(EditAnywhere, Category = Weapon)
		int32 selectedFireMode = 0;

	UPROPERTY(EditAnywhere, Category = Weapon)
		TArray<WeaponFireMode> allowedFireModes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USceneComponent* root;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
		int32 firedBullets = 0;

	UPROPERTY(EditAnywhere, Category = Weapon)
		USoundBase*  weaponSound;

	UPROPERTY(EditAnywhere, Category = Weapon)
		USoundBase*  emptySound;

	UPROPERTY(EditAnywhere, Category = Weapon)
		FString weaponID = "";
	/** AnimMontage to play each time we fire */

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FAnimationPattern animations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TSubclassOf<ABulletEjectActor> ejectionTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UArrowComponent* ejectionDirection;
	UPROPERTY(EditAnywhere, Category = Weapon)
		TArray<FString> acceptedBullets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GInventory)
		FTimerHandle reloadHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GInventory)
		FTimerHandle equippingHandle;
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* FP_MuzzleLocation;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* FP_FiringDirection;
	UPROPERTY(EditAnywhere, Category = Weapon)
		bool isUsable = true;
	UPROPERTY(EditAnywhere, Category = Weapon)
		bool bulletEjectable = true;
	UPROPERTY(EditAnywhere, Category = Weapon)
		bool hasFiringDirection = false;
	UPROPERTY(EditAnywhere, Category = Weapon)
		float burstAmmount = 3; 
	UPROPERTY(EditAnywhere, Category = Weapon)
		int32 selectedBulletIndex;



	UPROPERTY(EditAnywhere, Category = Weapon)
		ACharacter* owningPlayer;
	UPROPERTY(EditAnywhere, Category = Weapon)
		FRecoilPattern recoil;
	UPROPERTY(EditAnywhere, Category = Weapon)
		FDispersionPattern dispersion;




	void reloadMag(UBulletItem* mag);
	FString getBulletString();

	void playEmptySound(FVector place);
	void playSound(FVector place);
	void startUnUsablity(float time);
	void startReload();
	void startEquip();
	void endUnUsablity();

	void switchFireMode();
	WeaponFireMode getFireMode();
	bool loadWeapon(AWeapon* ID);
	bool loadMag(UBulletItem* ID);
	UBulletItem* unloadMag();

	int32 getAmmoCount();

	bool attachAttachment(WeaponAttachmentSlot slot, UWeaponAttachmentItem* item);
	void detachAttachment(WeaponAttachmentSlot slot);
	bool hasAttachment(WeaponAttachmentSlot slot);
	void removeWeapon();
	bool reload(UBulletItem* bullet);

	bool Fire();
	void endFire();
	UBulletItem* getLoadedMag();

	void decreaseSpread();
	void increaseSpread();
};

