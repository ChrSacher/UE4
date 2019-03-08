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
	suppresionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("suppresionComp"));
	suppresionBox->InitBoxExtent(FVector(10, 10, 10));
	suppresionBox->BodyInstance.SetCollisionProfileName("OverlapAll");
	physicsMaterialCollection = CreateDefaultSubobject<UPhysicsMaterialCollectionData>(TEXT("PhysicsMaterialCollection"));
	suppresionBox->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlapBegin);
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletComp"));
	mesh->SetupAttachment(CollisionComp);// Players can't walk on it
	suppresionBox->SetupAttachment(mesh);
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
		FMainDamageEvent da;
		da.hit = Hit;
		da.type = damageType;
		OtherActor->TakeDamage(damage, da, controllerOver, this);
		if (OtherComp->IsSimulatingPhysics())	OtherComp->AddImpulseAtLocation(GetVelocity(), Hit.ImpactPoint);
		if (Hit.PhysMaterial.IsValid())
		{
			USoundBase* sound = physicsMaterialCollection->getPhysicsSound(Hit.PhysMaterial.Get()->SurfaceType);
			if (sound) UGameplayStatics::PlaySoundAtLocation(GetWorld(), sound, Hit.ImpactPoint, 0.5f);
		}
		Destroy();
	}
}

ABulletEjectActor::ABulletEjectActor()
{
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	RootComponent = mesh;
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
}

void ABulletEjectActor::eject(FVector direction, UStaticMesh* newMesh)
{
	if (newMesh) if (mesh) mesh->SetStaticMesh(newMesh);
	ProjectileMovement->UpdatedComponent = RootComponent;
	ProjectileMovement->InitialSpeed = 0;
	ProjectileMovement->MaxSpeed = 1000;
	direction.Normalize();
	ProjectileMovement->Velocity = (direction * 300);
	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->bShouldBounce = false;

	InitialLifeSpan = 0.6f;
}

void ABullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && (OverlappedActors.Find(OtherActor) != INDEX_NONE))
	{
		FSuppresionDamageEvent ev;
		OtherActor->TakeDamage(0, ev, controllerOver, this);
		OverlappedActors.Add(OtherActor);
	}
}