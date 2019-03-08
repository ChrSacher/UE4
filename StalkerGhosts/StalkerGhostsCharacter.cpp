// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "StalkerGhosts.h"
#include "StalkerGhostsCharacter.h"
#include "Bullet.h"

#include "Animation/AnimInstance.h"
#include "GameFramework/InputSettings.h"
#include "Buff.h"
DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AStalkerGhostsCharacter

AStalkerGhostsCharacter::AStalkerGhostsCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(70.f, 96.0f);
	RootComponent = GetCapsuleComponent();
	currentWeapon = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon"));
	currentInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	damageComponent = CreateDefaultSubobject<UDamageComponent>(TEXT("Damage"));
	currentGrenade = CreateDefaultSubobject<UGrenadeComponent>(TEXT("Grenade"));
	SetActorHiddenInGame(false);
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	//FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	currentAttributes = CreateDefaultSubobject<UCharacterAttributes>(TEXT("Attributes"));
	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetupAttachment(GetCapsuleComponent());
	Mesh1P->SetOnlyOwnerSee(false);
	Mesh1P->bCastDynamicShadow = true;
	Mesh1P->CastShadow = true;
	Mesh1P->SetVisibility(true);
	//Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	//Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
	
	// Create a gun mesh component

	
	
	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	stance = Movement::JOGGING;
	//FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));
	
	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 30.0f, 10.0f);
	//
	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P are set in the
	// derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AStalkerGhostsCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
	characterAnim = Cast<UStalkerCharacterAnim>(Mesh1P->GetAnimInstance());
	changeStance(Movement::JOGGING);
	//currentInventory = NewObject<UInventoryComponent>();
	if (currentInventory) 
	{
		//currentInventory->loadUI();
		
	}
	FirstPersonCameraComponent->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Camera"));
	
	
	currentAttributes->getAttrib(AttributeType::ENDURANCE)->setRaw(5);
	currentAttributes->getAttrib(AttributeType::AGILITY)->setRaw(5);
	currentAttributes->getAttrib(AttributeType::INTELLIGENCE)->setRaw(5);
	currentAttributes->getAttrib(AttributeType::CHARISMA)->setRaw(5);
	currentAttributes->getAttrib(AttributeType::STRENGTH)->setRaw(5);
	currentAttributes->getAttrib(AttributeType::LUCK)->setRaw(5);
	currentAttributes->getAttrib(AttributeType::PERCEPTION)->setRaw(5);
	currentAttributes->getAttrib(AttributeType::SPRINTSPEED)->setRaw(600);
	currentAttributes->getAttrib(AttributeType::SPRINTCOST)->setRaw(-0.2);
	currentAttributes->getAttrib(AttributeType::JOGSPEED)->setRaw(400);
	currentAttributes->getAttrib(AttributeType::WALKSPEED)->setRaw(200);
	currentAttributes->getAttrib(AttributeType::MAXSTAMINA)->setRaw(100);
	currentAttributes->getAttrib(AttributeType::STAMINACOST)->setRaw(0.5);
	currentAttributes->getAttrib(AttributeType::STAMINAREGEN)->setRaw(0.5);
	currentAttributes->getAttrib(AttributeType::MAXHEALTH)->setRaw(100);
	GetCharacterMovement()->MaxWalkSpeed = currentAttributes->currentSpeed = currentAttributes->getAttrib(AttributeType::JOGSPEED)->getFinal();
	FString x = FString(TEXT("Ak47"));
	FString y = FString(("Ak47Bullet"));
	UBulletItem* w = currentInventory->createItem<UBulletItem>(y);
	w->ammount = 300;
	//UWeaponItem* y= currentInventory->createItem<UWeaponItem>(y);
	currentInventory->addItem(w);
	changeWeapon(x);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AStalkerGhostsCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AStalkerGhostsCharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &AStalkerGhostsCharacter::StopJumping);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &AStalkerGhostsCharacter::OnSprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &AStalkerGhostsCharacter::OffSprint);

	InputComponent->BindAction("Crouch", IE_Pressed, this, &AStalkerGhostsCharacter::OnCrouch);
	InputComponent->BindAction("Crouch", IE_Released, this, &AStalkerGhostsCharacter::OffCrouch);

	InputComponent->BindAction("Prone", IE_Pressed, this, &AStalkerGhostsCharacter::OnProne);
	InputComponent->BindAction("Prone", IE_Released, this, &AStalkerGhostsCharacter::OffProne);

	InputComponent->BindAction("Walk", IE_Pressed, this, &AStalkerGhostsCharacter::OnWalk);
	InputComponent->BindAction("Walk", IE_Released, this, &AStalkerGhostsCharacter::OffWalk);

	InputComponent->BindAction("Interact", IE_Pressed, this, &AStalkerGhostsCharacter::onInteract);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AStalkerGhostsCharacter::OnFire);
	InputComponent->BindAction("Fire", IE_Released, this, &AStalkerGhostsCharacter::OffFire);

	InputComponent->BindAction("Inventory", IE_Pressed, this, &AStalkerGhostsCharacter::OnInventory);
	InputComponent->BindAction("Inventory", IE_Released, this, &AStalkerGhostsCharacter::OffInventory);

	InputComponent->BindAction("Grenade", IE_Pressed, this, &AStalkerGhostsCharacter::onGrenade);

	InputComponent->BindAction("Reload", IE_Pressed, this, &AStalkerGhostsCharacter::OnReload);

	InputComponent->BindAxis("MoveForward", this, &AStalkerGhostsCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AStalkerGhostsCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AStalkerGhostsCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AStalkerGhostsCharacter::LookUpAtRate);
}

void AStalkerGhostsCharacter::OnFire()
{
	
	if(stance == Movement::JOGGING || stance ==  Movement::SPRINTING) changeStance(Movement::WALKING);
	Fire();
	GetWorld()->GetTimerManager().SetTimer(fireHandle, this, &AStalkerGhostsCharacter::Fire, 60 / currentWeapon->weapon->fireRate, true);

}
void AStalkerGhostsCharacter::OffFire()
{
	GetWorld()->GetTimerManager().ClearTimer(fireHandle);
	
}
void AStalkerGhostsCharacter::changeStance(Movement newStance)
{
	if(characterAnim) characterAnim->stance = newStance;
	prevStance = stance;
	stance = newStance;
}
void AStalkerGhostsCharacter::Fire()
{
	// try and fire a projectile
	UWorld* const World = GetWorld();
	if (!currentWeapon || !World)
	{
		OffFire();
		return;
	}

	const FRotator SpawnRotation = GetControlRotation();
	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	FVector SpawnLocation;
	if (FP_MuzzleLocation != nullptr) SpawnLocation = FP_MuzzleLocation->GetComponentLocation();
		else SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(GunOffset);

	if(!currentWeapon->Fire(SpawnLocation, SpawnRotation)) OffFire();
	else
	{
		if (currentWeapon->weapon->FireAnimation != NULL)
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
			if (AnimInstance != NULL)
			{
				AnimInstance->Montage_Play(currentWeapon->weapon->FireAnimation, 1.f);
			}
		}
	}
}
void AStalkerGhostsCharacter::changeWeapon(FString ID)
{
	UWeaponItem* x = Cast<UWeaponItem>(currentInventory->createItem<UWeaponItem>(ID));
	if(x) changeWeapon(x);
}
void AStalkerGhostsCharacter::changeWeapon(AWeapon* newwep)
{
	if (!newwep) return;
	currentWeapon->loadWeapon( newwep);
	currentWeapon->weapon->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));//Attach gun mesh component to Skeleton, doing it here because the skelton is not yet created in the constructor
	FP_MuzzleLocation->AttachToComponent(currentWeapon->weapon->mesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Muzzle"));

	currentWeapon->weapon->mesh->SetVisibility(true);
}
void AStalkerGhostsCharacter::changeWeapon(UWeaponItem* ID)
{

	currentWeapon->loadWeapon(ID);
	if (!currentWeapon->weapon) return;
	currentInventory->mainInventory->weapon1Equipped->ItemButton->UserPointer = Cast<UItemBase>(currentWeapon->weapon);
	currentWeapon->weapon->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));//Attach gun mesh component to Skeleton, doing it here because the skelton is not yet created in the constructor
	FP_MuzzleLocation->AttachToComponent(currentWeapon->weapon->mesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Muzzle"));
	
	currentWeapon->weapon->mesh->SetVisibility(true);
}

void AStalkerGhostsCharacter::MoveForward(float Value)
{
	characterAnim->speed = GetVelocity().Size() / currentAttributes->getAttrib(AttributeType::SPRINTSPEED)->getFinal() * 100;
	if (Value != 0.0f)
	{
		if (Value < -0.1f && stance == Movement::SPRINTING) OffSprint();
		// add movement in that direction

		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AStalkerGhostsCharacter::MoveRight(float Value)
{
	characterAnim->strafe = Value;
	if (Value != 0.0f)
	{
		
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AStalkerGhostsCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AStalkerGhostsCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AStalkerGhostsCharacter::OnReload()
{
	//look through inventory to see if there is ammo
	if (!currentInventory || currentWeapon->getAmmoCount() == currentWeapon->weapon->ammoCapacity) return;
	bool foundMag = false;
	if (currentWeapon->weapon->currentlyLoadedBullet) foundMag = checkMag(currentInventory->lookForItems(currentWeapon->weapon->currentlyLoadedBullet->itemIdentifier));
	if (!foundMag)
	{
		for (int i = 0; i < currentWeapon->weapon->acceptedBullets.Num(); i++)
		{
			foundMag = checkMag(currentInventory->lookForItems(currentWeapon->weapon->acceptedBullets[i]));
			if (foundMag) break;
		}

	}
	if (!foundMag) return;
	
	doReload();
	return;
}
void AStalkerGhostsCharacter::doReload()
{
	currentWeapon->startReload();
	currentInventory->mainInventory->weapon1BulletEquipped->ItemButton->UserPointer = currentWeapon->weapon->currentlyLoadedBullet;
	//found mag and reload is possible
	if (currentWeapon->weapon->ReloadAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		if (characterAnim != NULL)
		{
			characterAnim->Montage_Play(currentWeapon->weapon->ReloadAnimation);
		}
	}
}
void AStalkerGhostsCharacter::offReload()
{

}
bool AStalkerGhostsCharacter::checkMag(TArray<UItemBase*> Items)
{
	if (Items.Num() == 0) return false;
	int32 ammountCounter = 0;
	UBulletItem* temp = NULL;

	for (int i = 0; i < Items.Num(); i++)
	{
		
		if (Items[i]->ammount > 0)
		{
			if (Items[i]->IsA<UBulletItem>())
			{
				if (!temp)
				{
					temp = NewObject<UBulletItem>(Items[i]);
				}
				currentWeapon->weapon->currentlyLoadedBullet = temp;
				ammountCounter += Items[i]->ammount;
				if (temp->ammount < currentWeapon->weapon->ammoCapacity)
				{
					if (Items[i]->ammount >= currentWeapon->weapon->ammoCapacity - temp->ammount)
					{
						int32 toAdd = currentWeapon->weapon->ammoCapacity - temp->ammount;
						Items[i]->ammount -= toAdd;
						temp->ammount += toAdd;
					}
					else
					{
						temp->ammount += Items[i]->ammount;
						Items[i]->ammount = 0;
					}
				}
				else
				{
					return true;
				}
			}
		}
	}

	return ammountCounter > 0;
	
}
void AStalkerGhostsCharacter::Jump()
{
	bPressedJump = true;
	JumpKeyHoldTime = 0.0f;
	characterAnim->jumping = true;
}

void AStalkerGhostsCharacter::StopJumping()
{
	bPressedJump = false;
	JumpKeyHoldTime = 0.0f;
	characterAnim->jumping = false;
	
}
void AStalkerGhostsCharacter::Sprint()
{
	
	switch (stance)
	{
		case Movement::SPRINTING:
		{
			GetCharacterMovement()->MaxWalkSpeed = currentAttributes->currentSpeed = currentAttributes->getAttrib(AttributeType::SPRINTSPEED)->getFinal();
			currentAttributes->getAttrib(AttributeType::STAMINA)->addRaw(currentAttributes->getAttrib(AttributeType::SPRINTCOST)->getFinal());
			if (currentAttributes->getAttrib(AttributeType::STAMINA)->getFinal() <= 0)
			{
				currentAttributes->getAttrib(AttributeType::STAMINA)->setRaw(0);
				changeStance(Movement::JOGGING);
				
			}
		}break;
		case Movement::WALKING:
		{
			GetCharacterMovement()->MaxWalkSpeed = currentAttributes->currentSpeed = currentAttributes->getAttrib(AttributeType::WALKSPEED)->getFinal();
		}break;
		case Movement::JOGGING:
		{
			GetCharacterMovement()->MaxWalkSpeed = currentAttributes->currentSpeed = currentAttributes->getAttrib(AttributeType::JOGSPEED)->getFinal();
			GetWorld()->GetTimerManager().ClearTimer(speedHandle);
				return;
		}break;
		case Movement::CROUCHING:
		{
			GetCharacterMovement()->MaxWalkSpeed = currentAttributes->currentSpeed = (currentAttributes->getAttrib(AttributeType::JOGSPEED)->getFinal() - currentAttributes->getAttrib(AttributeType::WALKSPEED)->getFinal()) / 4 + currentAttributes->getAttrib(AttributeType::WALKSPEED)->getFinal();
		}break;
		case Movement::PRONING:
		{
			GetCharacterMovement()->MaxWalkSpeed = currentAttributes->currentSpeed = currentAttributes->getAttrib(AttributeType::WALKSPEED)->getFinal();
		}break;
	}
	
}
void AStalkerGhostsCharacter::OnSprint()
{
	
	//if (stance == Movement::CROUCHING || stance == Movement::PRONING) return;
	changeStance(Movement::SPRINTING);
	GetWorld()->GetTimerManager().SetTimer(speedHandle, this, &AStalkerGhostsCharacter::Sprint, 0.1f, true);
	
}
void AStalkerGhostsCharacter::OffSprint()
{
	if (stance == Movement::SPRINTING) changeStance(Movement::JOGGING);
	GetWorld()->GetTimerManager().SetTimer(staminaHandle, this, &AStalkerGhostsCharacter::regainStamina, 0.1f, true);
}

void AStalkerGhostsCharacter::OnWalk()
{
	//if (stance == Movement::CROUCHING || stance == Movement::PRONING) return; 
	changeStance(Movement::WALKING);
	GetWorld()->GetTimerManager().SetTimer(speedHandle, this, &AStalkerGhostsCharacter::Sprint, 0.1f, true);
	
	
}
void AStalkerGhostsCharacter::OffWalk()
{
	if(stance == Movement::WALKING) changeStance(Movement::JOGGING);
	GetWorld()->GetTimerManager().SetTimer(staminaHandle, this, &AStalkerGhostsCharacter::regainStamina, 0.1f, true);
}
void AStalkerGhostsCharacter::regainStamina()
{
	if (stance == Movement::SPRINTING ) return;
	
	if (currentAttributes->getAttrib(AttributeType::STAMINA)->getFinal() >= currentAttributes->getAttrib(AttributeType::MAXSTAMINA)->getFinal())
	{
		currentAttributes->getAttrib(AttributeType::STAMINA)->setRaw(currentAttributes->getAttrib(AttributeType::MAXSTAMINA)->getFinal());
		GetWorld()->GetTimerManager().ClearTimer(staminaHandle);
		return;
	}
	currentAttributes->getAttrib(AttributeType::STAMINA)->addRaw(currentAttributes->getAttrib(AttributeType::STAMINAREGEN)->getFinal());
	
	
	
}

void AStalkerGhostsCharacter::OnCrouch()
{
	//can all be condensed but for testing i leave it like this
	
	if (stance !=  Movement::CROUCHING)
	{
		Crouch();
		changeStance(Movement::CROUCHING);
		GetWorld()->GetTimerManager().SetTimer(speedHandle, this, &AStalkerGhostsCharacter::Sprint, 0.1f, true);
	}
	else
	{
		
		UnCrouch();
		changeStance(Movement::JOGGING);
		
	}
	//Crouch();
	//characterAnim->crouched = true;
	//changeStance(Movement::CROUCHING);
	//GetWorld()->GetTimerManager().SetTimer(speedHandle, this, &AStalkerGhostsCharacter::Sprint, 0.1f, true);
}
void AStalkerGhostsCharacter::OffCrouch()
{
	//UnCrouch();
	//characterAnim->crouched = false;
	//if(stance == Movement::CROUCHING) changeStance(Movement::JOGGING);
}

void AStalkerGhostsCharacter::OnProne()
{
	
	if (stance != Movement::PRONING)
	{
		changeStance(Movement::PRONING);
		GetWorld()->GetTimerManager().SetTimer(speedHandle, this, &AStalkerGhostsCharacter::Sprint, 0.1f, true);
		
	} 
	else
	{
		

		if (stance == Movement::PRONING)changeStance(Movement::JOGGING); 
	}
}
void AStalkerGhostsCharacter::OffProne()
{
	
	//characterAnim->prone = false;
	//if (stance == Movement::PRONING)changeStance(Movement::JOGGING);
}

void AStalkerGhostsCharacter::OnInventory()
{
	if (!currentInventory->mainInventory)
	{
		currentInventory->loadUI();
	}
	if (! (currentInventory->mainInventory->GetVisibility() == ESlateVisibility::Visible))
	{
		currentInventory->mainInventory->SetVisibility(ESlateVisibility::Visible);
	}
	else currentInventory->mainInventory->SetVisibility(ESlateVisibility::Hidden);
	
}
void AStalkerGhostsCharacter::OffInventory()
{

}

void AStalkerGhostsCharacter::onInteract()
{
	FVector Loc = GetActorLocation();
	FRotator Rot = FirstPersonCameraComponent->GetComponentRotation();
	FVector End = Loc + Rot.Vector() * 600; //make variable
	FCollisionQueryParams params = FCollisionQueryParams(FName(TEXT("Trace")), true, this);
	FCollisionResponseParams params2 = FCollisionResponseParams();
	FHitResult result(ForceInit);
	const FName TraceTag("MyTraceTag");
	GetWorld()->DebugDrawTraceTag = TraceTag;
	params.TraceTag = TraceTag;
	params.bTraceComplex = true;
	params.bTraceAsyncScene = true;
	params.bReturnPhysicalMaterial = true;
	bool traced = GetWorld()->LineTraceSingleByChannel(result, Loc, End, ECollisionChannel::ECC_Visibility, params,params2);
	if (traced)
	{
		//deal with traced
		IInteractInterface* actor = Cast<IInteractInterface>(result.GetActor());
		if (actor)
		{
			actor->interact(this);
		}
	}
}

void AStalkerGhostsCharacter::doDamage(float suggestedDamage,DamageBodyPart BodyPart,EDamageType type,FVector veloc,FVector location)
{
	currentAttributes->getAttrib(AttributeType::HEALTH)->addRaw(-suggestedDamage);
	// armor calc here and stuff
	UE_LOG(LogTemp, Warning, TEXT("%f"), suggestedDamage);
	if (currentAttributes->getAttrib(AttributeType::HEALTH)->getFinal() <= 0)
	{
		Mesh1P->SetSimulatePhysics(true);
		Mesh1P->WakeRigidBody();
		RootComponent = Mesh1P;
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("OverlapAll"));
		Mesh1P->AddImpulseAtLocation(veloc, location);
	}
}

void AStalkerGhostsCharacter::startDamage(FString bonename, ABullet* causer)
{
	DamageBodyPart BodyPart = damageComponent->getDamagedBodyPart(bonename);
	doDamage(damageComponent->damageAmmount(BodyPart, causer), BodyPart,causer->damageType,causer->GetVelocity(),causer->GetActorLocation());
}

void AStalkerGhostsCharacter::takeGrenadeDamage(AGrenade* Causer)
{
	doDamage(damageComponent->calculateGrenadeDamage(Causer), DamageBodyPart::CHEST, EDamageType::EXPLOSION, Causer->GetVelocity(), Causer->GetActorLocation());
}
void AStalkerGhostsCharacter::startShrapnelDamage(FString bonename, AGrenade* causer)
{
	DamageBodyPart BodyPart = damageComponent->getDamagedBodyPart(bonename);
	doDamage(damageComponent->damageAmmount(BodyPart, causer->shrapnelDamage), BodyPart,EDamageType::SHRAPNEL,((causer->GetActorLocation() - GetActorLocation()) * causer->explosionForce), causer->GetActorLocation());
}

void AStalkerGhostsCharacter::onGrenade()
{
	if (currentGrenade) currentGrenade->throwGrenade(FP_MuzzleLocation->GetComponentLocation(), FirstPersonCameraComponent->GetComponentRotation());
}


bool AStalkerGhostsCharacter::equipmentAdded(UItemBase* item, SlotInformation slot)
{
	if (!item) return false;
	switch (slot)
	{
			case SlotInformation::HELMET:
			{
				if (equipment.helmet) equipment.helmet->unEquip(this);
				equipment.helmet = item;
				item->equip(this);
			}break;
			case 	SlotInformation::ARMOR:
			{
				equipment.armor = item;
				item->equip(this);
			}break;
			case 	SlotInformation::BACKPACK:
			{
				equipment.backPack = item;
				item->equip(this);
			}break;
			case 	SlotInformation::BOOTS:
			{
				equipment.boots = item;
				item->equip(this);
			}break;
			case 	SlotInformation::WEAPON1:
			{
				AWeapon* x = Cast<AWeapon>(item);
				if (!x) return false;
				equipment.weapon1 = x;
				changeWeapon(x);
				item->equip(this);
			}break;
			case 	SlotInformation::WEAPON1AMMUNITION:
			{
				ABullet* x = Cast<ABullet>(item);
				if (!x) return false;
				equipment.weapon1Bullet = x;
				item->equip(this);
			}break;
			case 	SlotInformation::ARTIFACT1:
			{
				UArtifact* x = Cast<UArtifact>(item);
				if (!x) return false;
				equipment.art1 = x;
				for (int32 i = 0; i < x->attachedBuffs.Num(); i++) currentAttributes->addBuff(x->attachedBuffs[i]);
				item->equip(this);
			}
			case 	SlotInformation::ARTIFACT2:
			{
				UArtifact* x = Cast<UArtifact>(item);
				if (!x) return false;
				equipment.art2 = x;
				for (int32 i = 0; i < x->attachedBuffs.Num(); i++) currentAttributes->addBuff(x->attachedBuffs[i]);
				item->equip(this);
			}
			case 	SlotInformation::ARTIFACT3:
			{
				UArtifact* x = Cast<UArtifact>(item);
				if (!x) return false;
				equipment.art3 = x;
				for (int32 i = 0; i < x->attachedBuffs.Num(); i++) currentAttributes->addBuff(x->attachedBuffs[i]);
			}
			case 	SlotInformation::ARTIFACT4:
			{
				UArtifact* x = Cast<UArtifact>(item);
				if (!x) return false;
				equipment.art4 = x;
				for (int32 i = 0; i < x->attachedBuffs.Num();i++) currentAttributes->addBuff(x->attachedBuffs[i]);
				item->equip(this);
			}break;
			case 	SlotInformation::QUICK1:
			{

			}break;
			case 	SlotInformation::QUICK2:
			{

			}break;
			case 	SlotInformation::QUICK3:
			{

			}break;
			case 	SlotInformation::QUICK4:
			{

			}break;
	}
	return true;
}

bool AStalkerGhostsCharacter::equipmentRemoved(UItemBase* item, SlotInformation slot)
{
	switch (slot)
	{
	case SlotInformation::HELMET:
	{

	}
	case 	SlotInformation::ARMOR:
	{

	}
	case 	SlotInformation::BACKPACK:
	{

	}
	case 	SlotInformation::BOOTS:
	{

	}
	case 	SlotInformation::WEAPON1:
	{

	}
	case 	SlotInformation::WEAPON1AMMUNITION:
	{

	}
	case 	SlotInformation::ARTIFACT1:
	{
		UArtifact* x = Cast<UArtifact>(item);
		if (!x) return false;
		equipment.art1 = x;
		for (int32 i = 0; i < x->attachedBuffs.Num(); i++) currentAttributes->removeBuff(x->attachedBuffs[i]);
		item->unEquip(this);
	}
	case 	SlotInformation::ARTIFACT2:
	{
		UArtifact* x = Cast<UArtifact>(item);
		if (!x) return false;
		equipment.art2 = x;
		for (int32 i = 0; i < x->attachedBuffs.Num(); i++) currentAttributes->removeBuff(x->attachedBuffs[i]);
		item->unEquip(this);
	}
	case 	SlotInformation::ARTIFACT3:
	{
		UArtifact* x = Cast<UArtifact>(item);
		if (!x) return false;
		equipment.art3 = x;
		for (int32 i = 0; i < x->attachedBuffs.Num(); i++) currentAttributes->removeBuff(x->attachedBuffs[i]);
		item->unEquip(this);
	}
	case 	SlotInformation::ARTIFACT4:
	{
		UArtifact* x = Cast<UArtifact>(item);
		if (!x) return false;
		equipment.art4 = x;
		for (int32 i = 0; i < x->attachedBuffs.Num(); i++) currentAttributes->removeBuff(x->attachedBuffs[i]);
		item->unEquip(this);
	}
	case 	SlotInformation::QUICK1:
	{

	}
	case 	SlotInformation::QUICK2:
	{

	}
	case 	SlotInformation::QUICK3:
	{

	}
	case 	SlotInformation::QUICK4:
	{

	}
	}
	return true;
}