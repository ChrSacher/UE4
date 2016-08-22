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
	ProjectileMovement->InitialSpeed = velocity;
	ProjectileMovement->MaxSpeed = velocity;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
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





		//physics stuff
		FCollisionQueryParams params = FCollisionQueryParams(FName(TEXT("Trace")), true, this);
		FCollisionResponseParams params2 = FCollisionResponseParams();
		FHitResult hitResult;
		const FName TraceTag("MyTraceTag");
		GetWorld()->DebugDrawTraceTag = TraceTag;
		params.TraceTag = TraceTag;
		params.bTraceComplex = true;
		params.bTraceAsyncScene = true;
		params.bReturnPhysicalMaterial = true;
		FVector distance = (Hit.ImpactPoint - GetActorLocation());
		bool traced = GetWorld()->LineTraceSingleByChannel(hitResult, Hit.ImpactPoint - distance, distance + Hit.ImpactPoint, ECollisionChannel::ECC_Visibility, params, params2);
		if (traced)
		{
			if (hitResult.PhysMaterial.IsValid())
			{
				USoundBase* sound = physicsMaterialCollection->getPhysicsSound(hitResult.PhysMaterial.Get()->SurfaceType);
				if (sound) UGameplayStatics::PlaySoundAtLocation(GetWorld(), sound, hitResult.ImpactPoint, 0.5f);
				FPhysicsMaterialPenetration& pen = physicsMaterialCollection->getPhysicsPenetrationData(hitResult.PhysMaterial.Get()->SurfaceType);
				if (pen.allowPenetration)
				{
					FCollisionQueryParams params = FCollisionQueryParams(FName(TEXT("Trace")), true, this);
					FCollisionResponseParams params2 = FCollisionResponseParams();
					TArray<FHitResult> result;
					const FName TraceTag("MyTraceTag");
					GetWorld()->DebugDrawTraceTag = TraceTag;
					params.TraceTag = TraceTag;
					params.bTraceComplex = true;
					params.bTraceAsyncScene = true;
					params.bReturnPhysicalMaterial = true;
					bool traced = GetWorld()->LineTraceMultiByChannel(result, GetActorForwardVector() * 100 + GetActorLocation(), hitResult.ImpactPoint, ECollisionChannel::ECC_Visibility, params, params2);
					if (traced)
					{
						for (int32 i = 0; i < result.Num(); i++)
						{
							if (result[i].Actor == OtherActor)
							{
								float velocityLoss = pen.velocityLossPerCM * FVector::Dist(result[i].ImpactPoint, hitResult.ImpactPoint);
								float newVelocity = ProjectileMovement->Velocity.Size() - velocityLoss;
								if (newVelocity <= 20)
								{
									//did not penetrate the object

								}
								else
								{
									
									FRotator rot = GetActorRotation();
									rot.Pitch += FMath::RandRange(-pen.maxSpreadY, pen.maxSpreadY);
									rot.Yaw += FMath::RandRange(-pen.maxSpreadX, pen.maxSpreadX);
									SetActorLocation(result[i].ImpactPoint + (result[i].ImpactPoint - Hit.ImpactPoint ));
									ProjectileMovement->UpdatedComponent = CollisionComp;
									ProjectileMovement->Velocity.Normalize();
									ProjectileMovement->Velocity = ProjectileMovement->Velocity * (newVelocity);
									ProjectileMovement->InitialSpeed = newVelocity;
									return;
								}

							}
						}
					}
				}
			}
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