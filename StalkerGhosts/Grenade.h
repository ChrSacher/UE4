// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "DamageEnum.h"
#include "GameFramework/Actor.h"
#include "Grenade.generated.h"

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

	UPROPERTY(EditAnywhere, Category = Grenade)
		float fuzeTime = 5.0f;

	UPROPERTY(EditAnywhere, Category = Grenade)
		uint16 sharpnelAmmount = 10;

	UPROPERTY(EditAnywhere, Category = Grenade)
		float damage = 10;

	UPROPERTY(EditAnywhere, Category = Grenade)
		float shrapnelDamage = 10;

	UPROPERTY(EditAnywhere, Category = Grenade)
		float range = 25;

	UPROPERTY(EditAnywhere, Category = Grenade)
		float shrapnelVelocitry = 1000;

	UPROPERTY(EditAnywhere, Category = Grenade)
		UParticleSystem* flash;
	UPROPERTY(EditAnywhere, Category = Grenade)
		TSubclassOf<class ABullet> bullet;

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
			void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		
		/** Returns CollisionComp subobject **/
		FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
		/** Returns ProjectileMovement subobject **/
		FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};
