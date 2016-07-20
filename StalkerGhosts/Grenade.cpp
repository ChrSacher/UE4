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
	explosionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionComp"));
	explosionSphere->InitSphereRadius(100.0f);
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
				//deal with traced
				IDamageInterface* actor = Cast<IDamageInterface>(result.GetActor());
				if (actor)
				{
					UE_LOG(LogTemp, Warning, TEXT("OverlapGrenade"));
					actor->startShrapnelDamage(result.BoneName.ToString(),this);
				}
			}
		}
		Destroy();
		
}

void AGrenade::explosionTrace()
{
	explosionSphere->SetSphereRadius(range);
	explosionSphere->SetCollisionProfileName(TEXT("OverlapAll"));
	
	
	const FVector& Start = GetActorLocation();
	const FVector& End = GetActorLocation() + FVector(0,1,0);
	const float Radius = range;
	TArray<FHitResult> HitOut;
	ECollisionChannel TraceChannel = ECollisionChannel::ECC_Pawn;
	FCollisionQueryParams TraceParams(FName(TEXT("Trace")), true, this);
	TraceParams.bTraceComplex = false;
	//TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.AddIgnoredActor(this);
	
		//Get World Source
	
		bool traced = GetWorld()->SweepMultiByChannel(
			HitOut,
			Start,
			End,
			FQuat(),
			TraceChannel,
			FCollisionShape::MakeSphere(Radius),
			TraceParams
			);
	 
		if (traced)
		{
			TArray<AActor*> ignore;
			
			for (int32 i = 0; i < HitOut.Num(); i++)
			{
				
				auto* x = Cast < IDamageInterface>(HitOut[i].GetActor());
				if (x && (ignore.Find(HitOut[i].GetActor()) == INDEX_NONE))
				{
					const FVector& Start = GetActorLocation();
					const FVector& End = GetActorLocation() + (HitOut[i].GetActor()->GetActorLocation()- GetActorLocation()  )*10;
					FCollisionQueryParams params = FCollisionQueryParams(FName(TEXT("Trace")), true, this);
					FCollisionResponseParams params2 = FCollisionResponseParams();
					FHitResult result(ForceInit);
					//const FName TraceTag("MyTraceTag");
					//GetWorld()->DebugDrawTraceTag = TraceTag;
					//params.TraceTag = TraceTag;
					params.bTraceComplex = true;
					params.bTraceAsyncScene = true;
					params.bReturnPhysicalMaterial = true;
					bool traced2 = GetWorld()->LineTraceSingleByChannel(result, Start, End, ECollisionChannel::ECC_Visibility, params, params2);
					if (traced2)
					{
						if (HitOut[i].GetActor() == result.GetActor())
						{
							x->takeGrenadeDamage(this);
							ignore.Add(HitOut[i].GetActor());
						}
						
					}
				}
				
			}
		}

}

void AGrenade::loadGrenadeFromDataTable(FGrenadeLookUpTable* row)
{
	
	fuzeTime = row->fuzeTime;
	sharpnelAmmount = row->sharpnelAmmount;
	damage = row->damage;
	shrapnelDamage = row->shrapnelDamage;
	range = row->range;
	shrapnelVelocitry = row->shrapnelVelocitry;
	flash = row->flash;
	sound = row->sound;
	mesh->SetStaticMesh(row->mesh);
}