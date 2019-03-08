// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "Bullet.h"
#include "DamageInterface.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DataTables.h"
// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SphereComp"));
	CollisionComp->InitBoxExtent(FVector(10,10,10));
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ABullet::OnHit);		// set up a notification for when this component hits something blocking

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletComp"));
	mesh->SetupAttachment(CollisionComp);// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = velocity;
	ProjectileMovement->MaxSpeed = velocity;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) )
	{
		if(OtherComp->IsSimulatingPhysics())	OtherComp->AddImpulseAtLocation(GetVelocity(), GetActorLocation());
		IDamageInterface* act = Cast<IDamageInterface>(OtherActor);
		if (act && Hit.BoneName.ToString() != "None")
		{
			
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Hit.BoneName.ToString());
			act->startDamage(Hit.BoneName.ToString(), this);
		}
		Destroy();
	}
}

void  ABullet::loadFromDataTable(FBulletLookUpTable* row)
{
	//static const FString ContextString(TEXT("GENERAL"));

	//FBulletLookUpTable* row = table->FindRow<FBulletLookUpTable>(FName(*ID), ContextString);
	//if (!row) UE_LOG(LogTemp, Warning, TEXT("BulletRowNotFound"));
	damage = row->damage;
	type = row->type;
	mesh->SetStaticMesh(row->mesh);
	velocity = row->velocity;
}