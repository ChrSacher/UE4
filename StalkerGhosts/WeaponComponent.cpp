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
	currentMagazine = CreateDefaultSubobject<UMagazineComponent>(TEXT("Magazine"));
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

