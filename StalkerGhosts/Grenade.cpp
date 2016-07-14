// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "DamageInterface.h"
#include "Grenade.h"


// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(10.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AGrenade::OnHit);		// set up a notification for when this component hits something blocking

																			// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;
	flash = CreateDefaultSubobject<UParticleSystem>(TEXT("MyParticle"));
	explosionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionComp"));
	
	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 50;
	ProjectileMovement->MaxSpeed = 50;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGrenade::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
}

void AGrenade::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

}

void AGrenade::activate()
{
	
		GetWorld()->GetTimerManager().SetTimer(explosionTimer, this, &AGrenade::explode, fuzeTime, true);
}

FRotator FRandomRotator()
{
	const float pitch = FMath::FRandRange(-180.f, 180.f);
	const float yaw = FMath::FRandRange(-180.f, 180.f);
	const float roll = FMath::FRandRange(-180.f, 180.f);
	return FRotator(pitch, yaw, roll);
}
void AGrenade::explode()
{
	if (flash)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), flash, GetActorLocation(), GetActorRotation());	
	}
	explosionSphere->SetSphereRadius(range);
	explosionSphere->SetCollisionProfileName(TEXT("OverlapAll"));
	explosionSphere->OnComponentBeginOverlap.AddDynamic(this, &AGrenade::OnOverlap);

	if (bullet)
	{
		FVector Loc = GetActorLocation();
		FCollisionQueryParams params = FCollisionQueryParams(FName(TEXT("Trace")), true, this);
		FCollisionResponseParams params2 = FCollisionResponseParams();
		FHitResult result(ForceInit);
		params.bTraceComplex = true;
		params.bTraceAsyncScene = true;
		params.bReturnPhysicalMaterial = true;
		for (int i = 0; i < sharpnelAmmount; i++)
		{
			
			FRotator Rot = FRandomRotator();
			FVector End = Loc + Rot.Vector() * 600; //make variable
			
			bool traced = GetWorld()->LineTraceSingleByChannel(result, Loc, End, ECollisionChannel::ECC_Visibility, params, params2);
			if (traced)
			{
				//deal with traced
				IDamageInterface* actor = Cast<IDamageInterface>(result.GetActor());
				if (actor)
				{
					actor->startShrapnelDamage(result.BoneName.ToString(),this);
				}
			}
		}
	}
}

void AGrenade::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto* x = Cast < IDamageInterface>(OtherActor);
	if (x) x->takeGrenadeDamage(this);
}