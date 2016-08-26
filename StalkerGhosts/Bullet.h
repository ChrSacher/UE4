// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DamageEnum.h"
#include "ItemBase.h"
#include "PhysicsMaterialCollectionData.h"
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
	UPROPERTY(EditAnywhere, Category = Bullet)
		UStaticMesh* ejectionMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(EditAnywhere, Category = Bullet)
		AController* controllerOver;
	UPROPERTY(EditAnywhere, Category = Bullet)
		AActor* owningPlayer;
	UPROPERTY(EditAnywhere, Category = Bullet)
		bool bulletEjectenabled = true;						 /** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class UBoxComponent* CollisionComp;
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		TArray<AActor*> OverlappedActors;
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class UBoxComponent* suppresionBox;
	UPROPERTY(EditAnywhere, Category = Bullet)
		UPhysicsMaterialCollectionData* physicsMaterialCollection;
	UPROPERTY(EditAnywhere, Category = Bullet)
		FTimerHandle penetrationTimer;
	/** Projectile movement component */


	void startPenetration(float time);

	void endPenetration();
	/** called when projectile hits something */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/** Returns CollisionComp subobject **/
	FORCEINLINE class UBoxComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
	
};
UCLASS(config = Game)
class STALKERGHOSTS_API ABulletEjectActor :public AActor
{
	GENERATED_BODY()

public:
	ABulletEjectActor();

	void eject(FVector direction, UStaticMesh* newMesh);
	UPROPERTY(EditAnywhere, Category = Bullet)
		UStaticMeshComponent* mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;
	
};

UCLASS(config = Game)
class STALKERGHOSTS_API AShotgunBullet :public ABullet
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = Bullet)
		float submunitionAmmount = 12.0f;

	UPROPERTY(EditAnywhere, Category = Bullet)
		float maxSpreadX = 1.0f;
	UPROPERTY(EditAnywhere, Category = Bullet)
		float maxSpreadY = 1.0f;
	UPROPERTY(EditAnywhere, Category = Bullet)
		float minSpreadX = 1.0f;
	UPROPERTY(EditAnywhere, Category = Bullet)
		float minSpreadY = 1.0f;
};