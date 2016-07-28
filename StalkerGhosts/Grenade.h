// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "DamageEnum.h"
#include "ItemBase.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"

struct FGrenadeLookUpTable;
UCLASS()
class STALKERGHOSTS_API AGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenade();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleDefaultsOnly, Category = Grenade)
		float fuzeTime = 5.0f;

	UPROPERTY(VisibleDefaultsOnly, Category = Grenade)
		uint32 sharpnelAmmount = 10;

	UPROPERTY(VisibleDefaultsOnly, Category = Grenade)
		float explosionForce = 10000;//will form a vector from grenade to actor normalize and multiply

	UPROPERTY(VisibleDefaultsOnly, Category = Grenade)
		float damage = 10;

	UPROPERTY(VisibleDefaultsOnly, Category = Grenade)
		float shrapnelDamage = 10;

	UPROPERTY(VisibleDefaultsOnly, Category = Grenade)
		float range = 150;

	UPROPERTY(VisibleDefaultsOnly, Category = Grenade)
		float shrapnelVelocitry = 1000;

	UPROPERTY(VisibleDefaultsOnly, Category = Grenade)
		UParticleSystem* flash;

	UPROPERTY(VisibleDefaultsOnly, Category = Grenade)
		UStaticMeshComponent* mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Grenade)
		USoundBase* sound;

	FTimerHandle explosionTimer;
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class USphereComponent* CollisionComp;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class USphereComponent* explosionSphere;

		/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;

	UFUNCTION()
		void activate();

	UFUNCTION()
		void explode();

	/** called when projectile hits something */
		UFUNCTION()
			void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
		UFUNCTION()
			void explosionTrace();
		
		/** Returns CollisionComp subobject **/
		FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
		/** Returns ProjectileMovement subobject **/
		FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};
