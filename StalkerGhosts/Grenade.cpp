// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "DamageInterface.h"
#include "Grenade.h"
#include "DataTables.h"

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
	SetActorHiddenInGame(false);
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(CollisionComp);
	
	// Set as root component
	
	flash = CreateDefaultSubobject<UParticleSystem>(TEXT("MyParticle"));
	RootComponent = CollisionComp;
	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = RootComponent;
	ProjectileMovement->InitialSpeed = 1000;
	ProjectileMovement->MaxSpeed = 1000;
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
	const float pitch = FMath::FRandRange(0, 180.f);
	const float yaw = FMath::FRandRange(-180.f, 180.f);
	const float roll = FMath::FRandRange(-180.f, 180.f);
	return FRotator(pitch, yaw, roll);
}
void AGrenade::explode()
{
	if (flash)
	{
		//write CSV for grenades and load this with it
		

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), flash,FTransform(GetActorLocation()));
	}
	explosionTrace();
	UGameplayStatics::PlaySoundAtLocation(this, sound, GetActorLocation());

		FVector Loc = GetActorLocation();
		FCollisionQueryParams params = FCollisionQueryParams(FName(TEXT("Trace")), true, this);
		FCollisionResponseParams params2 = FCollisionResponseParams();
		FHitResult result(ForceInit);
		//const FName TraceTag("MyTraceTag");
		//GetWorld()->DebugDrawTraceTag = TraceTag;
		//params.TraceTag = TraceTag;
		params.bTraceComplex = true;
		params.bTraceAsyncScene = true;
		params.bReturnPhysicalMaterial = true;
		for (uint32 i = 0; i < sharpnelAmmount; i++)
		{
			
			FRotator Rot = FRandomRotator();
			FVector End = Loc + Rot.Vector() * 600; //make variable
			
			bool traced = GetWorld()->LineTraceSingleByChannel(result, Loc, End, ECollisionChannel::ECC_Visibility, params, params2);
			if (traced)
			{
				result.PhysMaterial.Get()->SurfaceType;
				//deal with traced
				FMainDamageEvent da;
				da.hit = result;
				da.type = type;
				result.GetActor()->TakeDamage(damage, da, controllerOver, this);
			}
		}
		Destroy();
		
}

void AGrenade::explosionTrace()
{

	TArray<AActor*> x;
	x.Add(this);
	UGameplayStatics::ApplyRadialDamage(GetWorld(), damage, GetActorLocation(), range, NULL, x, this, controllerOver);

}

