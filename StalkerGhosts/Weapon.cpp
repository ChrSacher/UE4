// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "DataTables.h"
#include "Weapon.h"
#include "Bullet.h"
#include "ItemBase.h"

FString toString(WeaponAttachmentSlot slot)
{
	switch (slot)
	{
		case WeaponAttachmentSlot::SCOPE:
		{
			return FString("Scope");
		}
		case WeaponAttachmentSlot::MUZZLEBRAKE:
		{
			return FString("MuzzleBrake");
		}
		case WeaponAttachmentSlot::GRIP:
		{
			return FString("Grip");
		}
	}
	return FString();
}
// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	root = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = root;
	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	mesh->SetupAttachment(root);
	flash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Flash"));

	flash->SetupAttachment(mesh);
	ejectionDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("flecha"));
	ejectionDirection->SetupAttachment(mesh);
	allowedFireModes.Add(WeaponFireMode::AUTO);
	for (uint8 i = 0; i < uint8(WeaponAttachmentSlot::NUM); i++)
	{
		FWeaponAttachment attachment;
		attachment.mesh = CreateDefaultSubobject<UStaticMeshComponent>(*toString(WeaponAttachmentSlot(i)));
		attachment.slot = WeaponAttachmentSlot(i);
		
		attachedMeshes.Add(attachment);
	}
	for (uint8 i = 0; i < attachedMeshes.Num(); i++)
	{
		if(attachedMeshes[i].mesh) attachedMeshes[i].mesh->SetupAttachment(mesh);
	}
	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_FiringDirection = CreateDefaultSubobject<USceneComponent>(TEXT("FiringDirection"));
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	animations.animInstance = Cast<UWeaponAnim>(mesh->GetAnimInstance());
	if (!ejectionDirection->AttachToComponent(mesh, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true), TEXT("EjectionPoint")))
	{
		bulletEjectable = false;
	}
	FP_MuzzleLocation->AttachToComponent(mesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Muzzle"));
	hasFiringDirection = FP_FiringDirection->AttachToComponent(mesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("FiringDirection"));
	dispersion.finalDispersionY = dispersion.standardDispersionY;
	dispersion.finalDispersionX = dispersion.standardDispersionX;
}

// Called every frame
void AWeapon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AWeapon::reloadMag(UBulletItem* mag)
{
	if (!mag) return;
	if(!currentlyLoadedBullet)
	{
		selectedBulletIndex = acceptedBullets.Find(mag->itemIdentifier);
		if (selectedBulletIndex != INDEX_NONE)
		{
			currentlyLoadedBullet = DuplicateObject(mag, NULL);
			currentlyLoadedBullet->ammount = 0;
		}
		else return;

	}
	if (currentlyLoadedBullet->itemIdentifier == mag->itemIdentifier)
	{
		if (currentlyLoadedBullet->ammount < ammoCapacity)
		{
			int32 toAdd = ammoCapacity - currentlyLoadedBullet->ammount;
			if (mag->ammount >= toAdd)
			{

				mag->ammount -= toAdd;
				currentlyLoadedBullet->ammount += toAdd;
			}
			else
			{
				currentlyLoadedBullet->ammount += mag->ammount;
				mag->ammount = 0;
			}
			return;
		}
	}
}

FString AWeapon::getBulletString()
{
	if (selectedBulletIndex >= 0 && selectedBulletIndex < acceptedBullets.Num()) return acceptedBullets[selectedBulletIndex];
	return "";
}

void AWeapon::playSound(FVector place)
{
	if (weaponSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, weaponSound, place);
	}
}

void AWeapon::playEmptySound(FVector place)
{
	if (emptySound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, emptySound, place);
	}
}
void AWeapon::startReload()
{
	startUnUsablity(reloadTime);
	if (animations.weaponReloadAnimation && animations.animInstance)
	{

		animations.animInstance->Montage_Play(animations.weaponReloadAnimation, 1.0f);
	}
}
void AWeapon::startUnUsablity(float time)
{
	isUsable = false;
	GetWorld()->GetTimerManager().SetTimer(reloadHandle, this, &AWeapon::endUnUsablity, time, false);
}
void  AWeapon::startEquip()
{
	startUnUsablity(equipTime);
}

void AWeapon::endUnUsablity()
{
	isUsable = true;
}


bool AWeapon::reload(UBulletItem* bullet)
{

	currentlyLoadedBullet = bullet;
	return bullet != NULL;
}

UBulletItem* AWeapon::getLoadedMag()
{
	return currentlyLoadedBullet;
};

bool AWeapon::Fire()
{
	FVector SpawnLocation = GetActorLocation();
	if (!isUsable) return false;
	if (!currentlyLoadedBullet)
	{
		//needs to be in 2 times because first one is a check if variable is NULL
		playEmptySound(SpawnLocation);
		return false;
	}
	canEndFire = false;
	mustEndFire = true;
	bool empty = currentlyLoadedBullet->ammount <= 0;


	if (empty)
	{
		mustEndFire = true;
		playEmptySound(SpawnLocation);
		return false;
	}
	else //if thjere is no bullets in the mag play the 
	{
		//check for dispersion
		if (dispersion.decreaseSpreadWhenFireEnded)
		{
			GetWorld()->GetTimerManager().ClearTimer(dispersion.decreaseSpreadTimer);
		}
		else
		{
			if (!GetWorld()->GetTimerManager().IsTimerActive(dispersion.decreaseSpreadTimer))
				GetWorld()->GetTimerManager().SetTimer(dispersion.decreaseSpreadTimer, this, &AWeapon::decreaseSpread, dispersion.timePerDecrease, true);
		}

		//spawn Bullet
		FVector dir = FP_MuzzleLocation->GetComponentLocation() - (FP_FiringDirection->GetComponentLocation());
		dir.Normalize();
		FRotator rot = dir.Rotation();
		if (!hasFiringDirection) rot = GetActorForwardVector().Rotation();
		rot.Pitch += FMath::RandRange(-dispersion.finalDispersionY, dispersion.finalDispersionY);
		rot.Yaw += FMath::RandRange(-dispersion.finalDispersionX, dispersion.finalDispersionX);
		
		increaseSpread();
		playSound(SpawnLocation);
		AShotgunBullet* checkIfShotgun = Cast<AShotgunBullet>(currentlyLoadedBullet->bullet->GetDefaultObject());
		ABullet* firedBullet = NULL;
		if (checkIfShotgun)
		{
			for (int32 i = 0; i < checkIfShotgun->submunitionAmmount; i++)
			{
				FRotator rotation = rot;
				rotation.Pitch = rot.Pitch +  FMath::RandRange(checkIfShotgun->minSpreadX, checkIfShotgun->maxSpreadX);
				rotation.Yaw =  rot.Yaw +  FMath::RandRange(checkIfShotgun->minSpreadY, checkIfShotgun->maxSpreadY);
				AShotgunBullet* x = GetWorld()->SpawnActor<AShotgunBullet>(currentlyLoadedBullet->bullet, FP_MuzzleLocation->GetComponentLocation(), rotation);
				if (!x)
				{
					return false;
				}
				if (Cast<ACharacter>(GetOwner()))
				{
					x->owningPlayer = owningPlayer;
					if (owningPlayer) x->controllerOver = owningPlayer->GetController();
				}
				firedBullet = x;
			}
			
		}
		else
		{
			ABullet* x = GetWorld()->SpawnActor<ABullet>(currentlyLoadedBullet->bullet, FP_MuzzleLocation->GetComponentLocation(), rot);
			if (!x)
			{
				return false;
			}
			if (Cast<ACharacter>(GetOwner()))
			{
				x->owningPlayer = owningPlayer;
				if (owningPlayer) x->controllerOver = owningPlayer->GetController();
			}
			firedBullet = x;
		}
		if (!firedBullet) return false;
		//eject a bullter
		currentlyLoadedBullet->ammount -= 1;
		firedBullets += 1;
		weaponFire.ExecuteIfBound();
		
		if (ejectionTemplate && firedBullet->bulletEjectenabled && bulletEjectable)
		{
			ABulletEjectActor* eject = GetWorld()->SpawnActor<ABulletEjectActor>(ejectionTemplate, ejectionDirection->GetComponentLocation(),FRotator());
			if (!firedBullet->ejectionMesh)
			{
				eject->eject(ejectionDirection->GetForwardVector(), firedBullet->mesh->StaticMesh);
			}
			else
			{
				eject->eject(ejectionDirection->GetForwardVector(), firedBullet->ejectionMesh);
			}
			
		}
	}


	//check Firemodes
	if (getFireMode() == WeaponFireMode::SINGLE)
	{
		
		if (firedBullets >= 1)
		{
			mustEndFire = true;	
			canEndFire = true;
		}
		
	}

	if (getFireMode() == WeaponFireMode::BURST)
	{
		mustEndFire = false;
		if (firedBullets >= burstAmmount)
		{
			mustEndFire = true;
			canEndFire = true;
		}
	}
	if (getFireMode() == WeaponFireMode::AUTO)
	{
		mustEndFire = false;
		if (firedBullets >= 0)
		{
			
			canEndFire = true;
		}
	}
	return true;
}

void AWeapon::endFire()
{
	firedBullets = 0;
	if(dispersion.decreaseSpreadWhenFireEnded) GetWorld()->GetTimerManager().SetTimer(dispersion.decreaseSpreadTimer, this, &AWeapon::decreaseSpread, dispersion.timePerDecrease, true);
}



int32  AWeapon::getAmmoCount()
{
	if (currentlyLoadedBullet) return currentlyLoadedBullet->ammount;
	return 0;
}

void AWeapon::removeWeapon()
{
	
}

bool AWeapon::loadMag(UBulletItem* ID)
{
	if (!ID) return false;
	UBulletItem* x = DuplicateObject(ID, NULL);
	x->ammount = 0;
	
	if (ammoCapacity <= ID->ammount)
	{
		x->ammount = ammoCapacity;
		ID->ammount -= ammoCapacity;
	}
	else
	{
		x->ammount = ID->ammount;
		ID->ammount = 0;
	}
	currentlyLoadedBullet = x;
	return true;
}

UBulletItem* AWeapon::unloadMag()
{
	UBulletItem* x = currentlyLoadedBullet;
	currentlyLoadedBullet = NULL;
	return x;
}

void AWeapon::switchFireMode()
{
	selectedFireMode++;
	if (selectedFireMode >= allowedFireModes.Num() || selectedFireMode < 0)
	{
		selectedFireMode = 0;
	}
	firedBullets = 0;
}
WeaponFireMode AWeapon::getFireMode()
{
	if (selectedFireMode <= allowedFireModes.Num() && selectedFireMode >= 0)
	{
		return allowedFireModes[selectedFireMode];
	}
	selectedFireMode = 0;
	if(allowedFireModes.Num() == 0) return WeaponFireMode::SINGLE;
	return allowedFireModes[selectedFireMode];
}
bool  AWeapon::attachAttachment(WeaponAttachmentSlot slot, UWeaponAttachmentItem* item)
{
	if (!((uint8)slot >= 0 && (uint8)slot < attachedMeshes.Num()) || !mesh) return false;
	if (!(attachedMeshes[(uint8)slot].isEnabled)) return false;
	if (item->allowedWeapons.Find(weaponID) == INDEX_NONE) return false;
	if (attachedMeshes[(uint8)slot].isAttached) detachAttachment(slot);
	attachedMeshes[(uint8)slot].mesh->SetStaticMesh(item->attachmentMesh);
	attachedMeshes[(uint8)slot].mesh->SetVisibility(true);
	
	attachedMeshes[(uint8)slot].isAttached = true;
	return true;
}
void  AWeapon::detachAttachment(WeaponAttachmentSlot slot)
{
	if (!((uint8)slot >= 0 && (uint8)slot < attachedMeshes.Num()) || !mesh) return;
	attachedMeshes[(uint8)slot].mesh->SetVisibility(false);
	attachedMeshes[(uint8)slot].isAttached = false;
}
bool  AWeapon::hasAttachment(WeaponAttachmentSlot slot)
{
	if (!((uint8)slot >= 0 && (uint8)slot < attachedMeshes.Num()) || !mesh) return false;
	return attachedMeshes[(uint8)slot].isAttached;
}

void AWeapon::decreaseSpread()
{
	dispersion.finalDispersionX -= dispersion.spreadDecreaseXPerSecond * dispersion.timePerDecrease;
	dispersion.finalDispersionY -= dispersion.spreadDecreaseYPerSecond * dispersion.timePerDecrease;
	if (dispersion.finalDispersionY < dispersion.standardDispersionY) dispersion.finalDispersionY = dispersion.standardDispersionY;
	if (dispersion.finalDispersionX < dispersion.standardDispersionX) dispersion.finalDispersionX = dispersion.standardDispersionX;
	if (dispersion.finalDispersionY <= dispersion.standardDispersionY && dispersion.finalDispersionX <= dispersion.standardDispersionX)
	{
		GetWorld()->GetTimerManager().ClearTimer(dispersion.decreaseSpreadTimer);
	}
	
}

void AWeapon::increaseSpread()
{
	dispersion.finalDispersionX += dispersion.perBulletDispersionX;
	dispersion.finalDispersionY += dispersion.perBulletDispersionY;
	if (dispersion.finalDispersionY >  dispersion.maxDispersionY) dispersion.finalDispersionY = dispersion.maxDispersionY;
	if (dispersion.finalDispersionX >  dispersion.maxDispersionX) dispersion.finalDispersionX = dispersion.maxDispersionX;
}