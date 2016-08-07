// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "ExplosiveMine.h"


// Sets default values
AExplosiveMine::AExplosiveMine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	triggerLocation = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerCollision"));
	groundMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	groundMesh->SetupAttachment(triggerLocation);
	
	triggerLocation->OnComponentBeginOverlap.AddDynamic(this, &AExplosiveMine::OnOverlapBegin);
	triggerLocation->OnComponentEndOverlap.AddDynamic(this, &AExplosiveMine::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AExplosiveMine::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplosiveMine::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AExplosiveMine::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor == nullptr) && (OtherActor == this) && (OtherComp == nullptr)) return;
	if(stepOnSound) UGameplayStatics::PlaySoundAtLocation(this, stepOnSound, GetActorLocation());
	
}
void AExplosiveMine::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor == nullptr) && (OtherActor == this) && (OtherComp == nullptr)) return;
	if(!isAboutToExplode) explode();
}

void AExplosiveMine::explode()
{
	if (stepOffSound) UGameplayStatics::PlaySoundAtLocation(this, stepOffSound, GetActorLocation());
	if (explosiveTemplate)
	{
		AGrenade* gren = GetWorld()->SpawnActor<AGrenade>(explosiveTemplate, GetActorLocation() + GetActorUpVector() * 10, GetActorUpVector().ToOrientationRotator());
		if (gren)
		{
			gren->ProjectileMovement->MaxSpeed = 100;
			gren->ProjectileMovement->InitialSpeed = 100;
			gren->ProjectileMovement->Velocity = (GetActorUpVector() * 10);
			gren->activate();
		}

	}
	isAboutToExplode = true;
	Destroy();
}