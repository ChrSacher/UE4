// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DamageEnum.h"
#include "ItemBase.h"
#include "Bullet.generated.h"


struct FBulletLookUpTable;

UCLASS(config = Game)
class STALKERGHOSTS_API ABullet :public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, Category = Bullet)
		float damage = 10.0f; 

	UPROPERTY(EditAnywhere, Category = Bullet)
		float velocity = 7000.0f; //in cm/s
	UPROPERTY(EditAnywhere, Category = Bullet)
		EDamageType damageType;
	UPROPERTY(EditAnywhere, Category = Bullet)
		UStaticMeshComponent* mesh;

	
	
								 /** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class UBoxComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;


	/** called when projectile hits something */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	FORCEINLINE class UBoxComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
	
};
