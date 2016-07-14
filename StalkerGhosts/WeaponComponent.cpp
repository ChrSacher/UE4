// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "WeaponComponent.h"


// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	acceptedBullets.Push("Default");
	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	light = CreateDefaultSubobject<USpotLightComponent>(TEXT("Light"));
	light->SetupAttachment(mesh);
	// ...
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	light->AttachToComponent(mesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Muzzle"));
	// ...
	
}


// Called every frame
void UWeaponComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UWeaponComponent::playSound(FVector place)
{
	if (weaponSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, weaponSound, place);
	}
}

void UWeaponComponent::playEmptySound(FVector place)
{
	if (emptySound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, emptySound, place);
	}
}

void UWeaponComponent::reload(float bulletammount)
{
	currentAmmoCount = bulletammount;
	isReloading = true;
	GetWorld()->GetTimerManager().SetTimer(reloadHandle, this, &UWeaponComponent::endReload, reloadTime, false);
}
void UWeaponComponent::endReload()
{
	isReloading = false;
}
bool UWeaponComponent::Fire(FVector SpawnLocation, FRotator SpawnRotation)
{
	if (isReloading)
	{
		//needs to be in 2 times because first one is a check if variable is NULL
		playEmptySound(SpawnLocation);
		return false;
	}
	bool empty = currentAmmoCount <= 0;


	if (!empty)
	{
		playSound(SpawnLocation);
		GetWorld()->SpawnActor<ABullet>(bullet, SpawnLocation, SpawnRotation);
		currentAmmoCount -= 1;
		return true;

	}
	else //if thjere is no bullets in the mag play the 
	{
		playEmptySound(SpawnLocation);
		return false;

	}
}