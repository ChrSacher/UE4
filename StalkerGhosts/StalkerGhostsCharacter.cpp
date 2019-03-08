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
	//FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
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
	armorMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("armorMesh"));
	armorMesh->SetupAttachment(Mesh1P);
	bootsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("bootsMesh"));
	bootsMesh->SetupAttachment(Mesh1P);
	pantsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("pantsMesh"));
	pantsMesh->SetupAttachment(Mesh1P);
	bodyArmorMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("bodyArmorMesh"));
	bodyArmorMesh->SetupAttachment(Mesh1P);
	backPackMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("backPackMesh"));
	backPackMesh->SetupAttachment(Mesh1P);
	helmetMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("helmetMesh"));
	helmetMesh->SetupAttachment(Mesh1P);
	// Create a gun mesh component
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("cameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

												// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("followCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	FollowCamera->Deactivate();
	physicsMaterialCollection = CreateDefaultSubobject<UPhysicsMaterialCollectionData>(TEXT("PhysicsMaterialCollection"));
	normalPPComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("normalPostProcess"));
	normalPPComponent->SetupAttachment(RootComponent);
	
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
	characterAnim->pawn = this;
	changeStance(Movement::JOGGING);
	//currentInventory = NewObject<UInventoryComponent>();
	if (currentInventory) 
	{
		//currentInventory->loadUI();
		
	}
	FirstPersonCameraComponent->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Camera"));
	CameraBoom->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Camera"));
	damageComponent->setup();
	currentAttributes = DuplicateObject(currentAttributes, this);
	currentAttributes->setup();
	GetCharacterMovement()->MaxWalkSpeed = currentSpeed = currentAttributes->getAttrib(AttributeType::JOGSPEED)->getFinal();
	if (armorMesh) armorMesh->SetMasterPoseComponent(Mesh1P);
	if (pantsMesh) pantsMesh->SetMasterPoseComponent(Mesh1P);
	if (bodyArmorMesh) bodyArmorMesh->SetMasterPoseComponent(Mesh1P);
	if (backPackMesh)  backPackMesh->SetMasterPoseComponent(Mesh1P);
	if (helmetMesh) helmetMesh->SetMasterPoseComponent(Mesh1P);
	equipment = currentInventory->equipment;
	syncEquipment();
	currentInventory->unEquipDelegate.BindUObject(this, &AStalkerGhostsCharacter::equipmentRemoved);
	currentInventory->equipDelegate.BindUObject(this, &AStalkerGhostsCharacter::equipmentAdded);
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

	InputComponent->BindAction("Quick1", IE_Pressed, this, &AStalkerGhostsCharacter::OnQuickSlot1);
	InputComponent->BindAction("Quick2", IE_Pressed, this, &AStalkerGhostsCharacter::OnQuickSlot2);
	InputComponent->BindAction("Quick3", IE_Pressed, this, &AStalkerGhostsCharacter::OnQuickSlot3);
	InputComponent->BindAction("Quick4", IE_Pressed, this, &AStalkerGhostsCharacter::OnQuickSlot4);

	InputComponent->BindAction("NightVision", IE_Pressed, this, &AStalkerGhostsCharacter::OnNightVision);

	InputComponent->BindAction("AimDown", IE_Pressed, this, &AStalkerGhostsCharacter::aimDownSight);
	InputComponent->BindAction("SwitchPerson", IE_Pressed, this, &AStalkerGhostsCharacter::switchPerson);
	InputComponent->BindAction("FireMode", IE_Pressed, this, &AStalkerGhostsCharacter::switchFireMode); 
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
	

}
void AStalkerGhostsCharacter::OffFire()
{
	
	if (weapon)
	{
		if (weapon->canEndFire)
		{
			weapon->endFire();
			GetWorld()->GetTimerManager().ClearTimer(fireHandle);
		}

	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(fireHandle);
	}
	
}
void AStalkerGhostsCharacter::changeStance(Movement newStance)
{
	
	prevStance = stance;
	stance = newStance;
}
void AStalkerGhostsCharacter::Fire()
{
	// try and fire a projectile
	UWorld* const World = GetWorld();
	if (!weapon || !World)
	{
		OffFire();
		return;
	}
	if (weapon->Fire())
	{
		GetWorld()->GetTimerManager().SetTimer(fireHandle, this, &AStalkerGhostsCharacter::Fire, 60 / weapon->fireRate, false);
	}
	if (weapon->mustEndFire) OffFire();
	
}
void AStalkerGhostsCharacter::weaponFired()
{
	APlayerController* Controller = GetWorld()->GetFirstLocalPlayerFromController()->PlayerController;
	FRecoilPattern& rec = weapon->recoil;

	if (rec.recoilEnabled)
	{
		if (rec.recoilShake) Controller->ClientPlayCameraShake(rec.recoilShake, 1.0f);
		AddControllerPitchInput(FMath::RandRange(rec.yOffsetMin, rec.yOffsetMax));
		AddControllerYawInput(FMath::RandRange(rec.xOffsetMin, rec.xOffsetMax));
	}

	if (weapon->FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(weapon->FireAnimation, 1.f);
		}
	}
}
void AStalkerGhostsCharacter::syncEquipment()
{
	if (weapon)
	{
		stopAimingDownSight();
		weapon->weaponFire.Unbind();
		weapon->weaponFire.BindUObject(this, &AStalkerGhostsCharacter::weaponFired);
		weapon->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));//Attach gun mesh component to Skeleton, doing it here because the skelton is not yet created in the constructor
		
		weapon->mesh->SetVisibility(true);
	}
	
	
}
void AStalkerGhostsCharacter::aimDownSight()
{
	if (isAimingDownSight)
	{
		stopAimingDownSight();
		return;
	}
	if (!weapon) return;
	if (!weapon->mesh) return;
	const USkeletalMeshSocket* eye =  weapon->mesh->GetSocketByName("EyePos");
	if (eye)
	{
		FirstPersonCameraComponent->AttachToComponent(weapon->mesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("EyePos"));
		FirstPersonCameraComponent->SetRelativeRotation(FRotator());
		isAimingDownSight = true;
	}
}
void AStalkerGhostsCharacter::stopAimingDownSight()
{
	FirstPersonCameraComponent->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Camera"));
	isAimingDownSight = false;
}

void AStalkerGhostsCharacter::changeWeapon(FString ID)
{
	UWeaponItem* x = Cast<UWeaponItem>(currentInventory->createItem<UWeaponItem>(ID));
	if(x) changeWeapon(x);
}
void AStalkerGhostsCharacter::changeWeapon(AWeapon* newwep)
{
	if (!newwep) return;
	stopAimingDownSight();
	weapon = newwep;
	weapon->SetActorHiddenInGame(false);
	weapon->startEquip();
	weapon->owningPlayer = this;
	//UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
	if (characterAnim != NULL)
	{
		if (weapon->equipAnimation) characterAnim->Montage_Play(weapon->equipAnimation, 1.f);
		
	}
	syncEquipment();
	
	
}
void AStalkerGhostsCharacter::changeWeapon(UWeaponItem* ID)
{
	if (!ID) return;
	changeWeapon(ID->generatedWeapon);
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
		if (stance == Movement::SPRINTING) changeStance(Movement::JOGGING);
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
	if (!weapon) return;
	if (!currentInventory || weapon->getAmmoCount() == weapon->ammoCapacity) return;
	bool foundMag = false;
	currentInventory->addItem(Cast<UItemBase>(weapon->getLoadedMag()));
	weapon->unloadMag();
	foundMag = checkMag(currentInventory->lookForItems(weapon->getBulletString()));
	if (!foundMag)
	{
		for (int i = 0; i < weapon->acceptedBullets.Num(); i++)
		{
			foundMag = checkMag(currentInventory->lookForItems(weapon->acceptedBullets[i]));
			if (foundMag) break;
		}

	}
	if (!foundMag) return;
	
	playReload();
	return;
}
void AStalkerGhostsCharacter::playReload()
{
	if (!weapon) return;
	stopAimingDownSight();
	weapon->startReload();
	//found mag and reload is possible
	if (weapon->ReloadAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		if (characterAnim != NULL)
		{
			characterAnim->Montage_Play(weapon->ReloadAnimation);
		}
	}
}
void AStalkerGhostsCharacter::doReload(UBulletItem* toReload)
{
	if(weapon) weapon->reloadMag((toReload));
}
void AStalkerGhostsCharacter::offReload()
{

}
bool AStalkerGhostsCharacter::checkMag(TArray<UItemBase*> Items)
{
	if (Items.Num() == 0) return false;
	int32 ammountCounter = 0;
	for (int i = 0; i < Items.Num(); i++)
	{
		
		if (Items[i]->ammount > 0)
		{
			if (Items[i]->IsA<UBulletItem>())
			{
				ammountCounter += Items[i]->ammount;
				doReload(Cast<UBulletItem>(Items[i]));
			}
		}
	}

	return ammountCounter > 0;
	
}
void AStalkerGhostsCharacter::Jump()
{
	characterAnim->isJumping = true;
	bPressedJump = true;
	JumpKeyHoldTime = 0.0f;
	//characterAnim->jumping = true;
}

void AStalkerGhostsCharacter::StopJumping()
{
	characterAnim->isJumping = false;
	bPressedJump = false;
	JumpKeyHoldTime = 0.0f;
	//characterAnim->jumping = false;
	
}
void AStalkerGhostsCharacter::Sprint()
{
	
	switch (stance)
	{
		case Movement::SPRINTING:
		{
			GetCharacterMovement()->MaxWalkSpeed = currentSpeed = currentAttributes->getAttrib(AttributeType::SPRINTSPEED)->getFinal();
			currentAttributes->getAttrib(AttributeType::STAMINA)->addRaw(currentAttributes->getAttrib(AttributeType::SPRINTCOST)->getFinal());
			if (currentAttributes->getAttrib(AttributeType::STAMINA)->getFinal() <= 0)
			{
				currentAttributes->getAttrib(AttributeType::STAMINA)->setRaw(0);
				changeStance(Movement::JOGGING);
				
			}
		}break;
		case Movement::WALKING:
		{
			GetCharacterMovement()->MaxWalkSpeed = currentSpeed = currentAttributes->getAttrib(AttributeType::WALKSPEED)->getFinal();
		}break;
		case Movement::JOGGING:
		{
			GetCharacterMovement()->MaxWalkSpeed = currentSpeed = currentAttributes->getAttrib(AttributeType::JOGSPEED)->getFinal();
			GetWorld()->GetTimerManager().ClearTimer(speedHandle);
				return;
		}break;
		case Movement::CROUCHING:
		{
			GetCharacterMovement()->MaxWalkSpeed = currentSpeed = (currentAttributes->getAttrib(AttributeType::JOGSPEED)->getFinal() - currentAttributes->getAttrib(AttributeType::WALKSPEED)->getFinal()) / 4 + currentAttributes->getAttrib(AttributeType::WALKSPEED)->getFinal();
		}break;
		case Movement::PRONING:
		{
			GetCharacterMovement()->MaxWalkSpeed = currentSpeed = currentAttributes->getAttrib(AttributeType::WALKSPEED)->getFinal();
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
		currentInventory->showInventory();
	}
	else
	{
		currentInventory->hideInventory();
	}
	
}
void AStalkerGhostsCharacter::OffInventory()
{

}

void AStalkerGhostsCharacter::onInteract()
{
	FVector Loc = FirstPersonCameraComponent->GetComponentLocation();
	FVector Rot = FirstPersonCameraComponent->GetForwardVector();
	FVector End = Loc + Rot * 600; //make variable
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

void AStalkerGhostsCharacter::doDamage(float suggestedDamage,DamageBodyPart BodyPart,EDamageType type)
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
	}
}
float AStalkerGhostsCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{

	
	if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		AGrenade* w = Cast<AGrenade>(DamageCauser);
		FRadialDamageEvent*  x = (FRadialDamageEvent*)&DamageEvent;
		if (w)
		{
			doDamage(damageComponent->calculateGrenadeDamage(w), DamageBodyPart::CHEST, w->type);
			APlayerController* Controller = GetWorld()->GetFirstLocalPlayerFromController()->PlayerController;
			if(w->explosionShake) Controller->ClientPlayCameraShake(w->explosionShake, 1.0f);
			if(w->earRingingSound) UGameplayStatics::PlaySound2D(GetWorld(),w->earRingingSound);
		}
		
	}
	if (DamageEvent.IsOfType(FMainDamageEvent::ClassID))
	{
		
		FMainDamageEvent*  causer = (FMainDamageEvent*)&DamageEvent;
		ABullet* w = Cast<ABullet>(DamageCauser);
		
		if (w)
		{
			DamageBodyPart BodyPart = damageComponent->getDamagedBodyPart(causer->hit.BoneName.ToString());
			doDamage(damageComponent->damageAmmount(BodyPart, DamageAmount), BodyPart, w->damageType);
		}
		else
		{
			doDamage(damageComponent->damageAmmount(DamageBodyPart::ALL, DamageAmount), DamageBodyPart::ALL, causer->type);
		}
	}
	if (DamageEvent.IsOfType(FSuppresionDamageEvent::ClassID))
	{
		FSuppresionDamageEvent*  causer = (FSuppresionDamageEvent*)&DamageEvent;
		
	}
	return DamageAmount;
}


void AStalkerGhostsCharacter::onGrenade()
{
	if (currentGrenade) currentGrenade->throwGrenade(GetActorLocation() + FVector(0,0,50),Mesh1P->GetForwardVector().ToOrientationRotator());
}

void visSkeletalMesh(USkeletalMeshComponent* mesh, bool vis)
{
	mesh->SetVisibility(vis);
	mesh->SetHiddenInGame(!vis);
}



void AStalkerGhostsCharacter::equipmentAdded(UItemBase* item, SlotInformation slot, bool& acceptance)
{
	if (!item)
	{
		acceptance = false;
		return;
	}
	switch (slot)
	{
			case SlotInformation::HELMET:
			{
				UHelmetItem* hel = Cast<UHelmetItem>(item);
				if (!hel) 
				{
					UE_LOG(LogTemp, Warning, TEXT("Helmet equip not actually helmet"));
					acceptance = false;
					return;
				}
				helmetMesh->SetSkeletalMesh(hel->mesh);
				visSkeletalMesh(helmetMesh, true);
				for (int32 i = 0; i < hel->getBuffs().Num(); i++) currentAttributes->addBuff(hel->getBuffs()[i]);
				damageComponent->addArmor(hel);
			}break;
			case 	SlotInformation::ARMOR:
			{
				UArmorItem* hel = Cast<UArmorItem>(item);
				if (!hel)
				{
					UE_LOG(LogTemp, Warning, TEXT("Armor equip not actually helmet"));
					acceptance = false;
					return;
				}
				for (int32 i = 0; i < hel->getBuffs().Num(); i++) currentAttributes->addBuff(hel->getBuffs()[i]);
				armorMesh->SetSkeletalMesh(hel->mesh);
				visSkeletalMesh(armorMesh, true);
				damageComponent->addArmor(hel);
			}break;
			case 	SlotInformation::BACKPACK:
			{
				UBackPackItem* hel = Cast<UBackPackItem>(item);
				if (!hel)
				{
					UE_LOG(LogTemp, Warning, TEXT("Backpack equip not actually helmet"));
					acceptance = false;
					return;
				}
				for (int32 i = 0; i < hel->getBuffs().Num(); i++) currentAttributes->addBuff(hel->getBuffs()[i]);
				backPackMesh->SetSkeletalMesh(hel->mesh);
				visSkeletalMesh(backPackMesh, true);
				damageComponent->addArmor(hel);
			}break;
			case 	SlotInformation::BODYARMOR:
			{
				UBodyArmorItem* hel = Cast<UBodyArmorItem>(item);
				if (!hel)
				{
					UE_LOG(LogTemp, Warning, TEXT("Bodyarmor equip not actually helmet"));
					acceptance = false;
					return;
				}
				for (int32 i = 0; i < hel->getBuffs().Num(); i++) currentAttributes->addBuff(hel->getBuffs()[i]);
				bodyArmorMesh->SetSkeletalMesh(hel->mesh);
				visSkeletalMesh(bodyArmorMesh, true);
				damageComponent->addArmor(hel);
			}break;
			case 	SlotInformation::BOOTS:
			{
				UBootItem* hel = Cast<UBootItem>(item);
				if (!hel)
				{
					UE_LOG(LogTemp, Warning, TEXT("Boots equip not actually helmet"));
					acceptance = false;
					return;
				}
				for (int32 i = 0; i < hel->getBuffs().Num(); i++) currentAttributes->addBuff(hel->getBuffs()[i]);
				bootsMesh->SetSkeletalMesh(hel->mesh);
				visSkeletalMesh(bootsMesh, true);
				damageComponent->addArmor(hel);
			}break;
			case 	SlotInformation::PANTS:
			{
				UPantsItem* hel = Cast<UPantsItem>(item);
				if (!hel)
				{
					UE_LOG(LogTemp, Warning, TEXT("Pants equip not actually helmet"));
					acceptance = false;
					return;
				}
				for (int32 i = 0; i < hel->getBuffs().Num(); i++) currentAttributes->addBuff(hel->getBuffs()[i]);
				pantsMesh->SetSkeletalMesh(hel->mesh);
				visSkeletalMesh(pantsMesh, true);
				damageComponent->addArmor(hel);
			}break;
			case 	SlotInformation::WEAPON1:
			{
				UWeaponItem* x = Cast<UWeaponItem>(item);
				if (!x) acceptance = false;
				changeWeapon(x);
				
				
			}break;
			case SlotInformation::GRENADE:
			{
				UGrenadeItem* x = Cast<UGrenadeItem>(item);
				if (!x)
				{
					acceptance = false;
					return;
				}
				currentGrenade->selectedGrenade = x;
			}break;
			case 	SlotInformation::ARTIFACT1:
			{
				UArtifact* x = Cast<UArtifact>(item);
				if (!x)
				{
					acceptance = false;
					return;
				}
				for (int32 i = 0; i < x->getBuffs().Num(); i++) currentAttributes->addBuff(x->getBuffs()[i]);
				
			}
			case 	SlotInformation::ARTIFACT2:
			{
				UArtifact* x = Cast<UArtifact>(item);
				if (!x)
				{
					acceptance = false;
					return;
				}
				for (int32 i = 0; i < x->getBuffs().Num(); i++) currentAttributes->addBuff(x->getBuffs()[i]);
				
			}
			case 	SlotInformation::ARTIFACT3:
			{
				UArtifact* x = Cast<UArtifact>(item);
				if (!x)
				{
					acceptance = false;
					return;
				}
				for (int32 i = 0; i < x->getBuffs().Num(); i++) currentAttributes->addBuff(x->getBuffs()[i]);
			}
			case 	SlotInformation::ARTIFACT4:
			{
				UArtifact* x = Cast<UArtifact>(item);
				if (!x)
				{
					acceptance = false;
					return;
				}
				for (int32 i = 0; i < x->getBuffs().Num();i++) currentAttributes->addBuff(x->getBuffs()[i]);
				
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
	item->equip(this);
	acceptance = true; 
	return;
}

void AStalkerGhostsCharacter::equipmentRemoved(UItemBase* item, SlotInformation slot, bool& acceptance)
{
	if (!item)
	{
		acceptance = false;
		return;
	}
 	switch (slot)
	{
		case SlotInformation::HELMET:
		{
			UHelmetItem* hel = Cast<UHelmetItem>(item);
			if (hel)
			{
				for (int32 i = 0; i < hel->getBuffs().Num(); i++) currentAttributes->removeBuff(hel->getBuffs()[i]);
				damageComponent->removeArmor(hel);
			}
			visSkeletalMesh(helmetMesh, false);
			
			
		}break;
		case 	SlotInformation::ARMOR:
		{
			UArmorItem* hel = Cast<UArmorItem>(item);
			if (hel)
			{
				for (int32 i = 0; i < hel->getBuffs().Num(); i++) currentAttributes->removeBuff(hel->getBuffs()[i]);
				damageComponent->removeArmor(hel);
			}
			visSkeletalMesh(armorMesh, false);
			
		}break;
		case 	SlotInformation::BACKPACK:
		{
			UBackPackItem* hel = Cast<UBackPackItem>(item);
			if (hel)
			{
				for (int32 i = 0; i < hel->getBuffs().Num(); i++) currentAttributes->removeBuff(hel->getBuffs()[i]);
				damageComponent->removeArmor(hel);
			}
			visSkeletalMesh(backPackMesh, false);
			
		}break;
		case 	SlotInformation::BOOTS:
		{
			UBootItem* hel = Cast<UBootItem>(item);
			if (hel)
			{
				for (int32 i = 0; i < hel->getBuffs().Num(); i++) currentAttributes->removeBuff(hel->getBuffs()[i]);
				damageComponent->removeArmor(hel);
			}
			visSkeletalMesh(bootsMesh, false);
			
		}break;
		
		case 	SlotInformation::BODYARMOR:
		{
			UBodyArmorItem* hel = Cast<UBodyArmorItem>(item);
			if (hel)
			{
				for (int32 i = 0; i < hel->getBuffs().Num(); i++) currentAttributes->removeBuff(hel->getBuffs()[i]);
				damageComponent->removeArmor(hel);
			}
			visSkeletalMesh(bodyArmorMesh, false);
		
		}break;
		case 	SlotInformation::PANTS:
			
		{
			UPantsItem* hel = Cast<UPantsItem>(item);
			if (hel)
			{
				for (int32 i = 0; i < hel->getBuffs().Num(); i++) currentAttributes->removeBuff(hel->getBuffs()[i]);
				damageComponent->removeArmor(hel);
			}
			
			visSkeletalMesh(pantsMesh, false);
		}break;
		case 	SlotInformation::WEAPON1:
		{
			weapon->SetActorHiddenInGame(true);
			weapon->DetachRootComponentFromParent();
			stopAimingDownSight();
			OffFire();
			weapon = NULL;
		
		}break;
		case SlotInformation::GRENADE:
		{
			UGrenadeItem* x = Cast<UGrenadeItem>(item);
			currentGrenade->selectedGrenade = NULL;
			if (!x)
			{
				acceptance = true; return;
			}
		}break;
		case 	SlotInformation::ARTIFACT1:
		{
			UArtifact* x = Cast<UArtifact>(item);
			if (!x) {acceptance = true; return;}
			for (int32 i = 0; i < x->getBuffs().Num(); i++) currentAttributes->removeBuff(x->getBuffs()[i]);
		
		}break;
		case 	SlotInformation::ARTIFACT2:
		{
			UArtifact* x = Cast<UArtifact>(item);
			if (!x) {acceptance = true; return;}
			for (int32 i = 0; i < x->getBuffs().Num(); i++) currentAttributes->removeBuff(x->getBuffs()[i]);
		
		}break;
		case 	SlotInformation::ARTIFACT3:
		{
			UArtifact* x = Cast<UArtifact>(item);
			if (!x) {acceptance = true; return;}
			for (int32 i = 0; i < x->getBuffs().Num(); i++) currentAttributes->removeBuff(x->getBuffs()[i]);
		
		}break;
		case 	SlotInformation::ARTIFACT4:
		{
			UArtifact* x = Cast<UArtifact>(item);
			if (!x) {acceptance = true; return;}
			for (int32 i = 0; i < x->getBuffs().Num(); i++) currentAttributes->removeBuff(x->getBuffs()[i]);
		
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
	//currentInventory->addItem(item);
	item->unEquip(this);
	acceptance = true; 
	return;
}

void AStalkerGhostsCharacter::OnQuickSlot1()
{
	UItemBase* x = equipment->getItem<UItemBase>(SlotInformation::QUICK1);
	if(x) x->use(this);
}
void AStalkerGhostsCharacter::OnQuickSlot2()
{
	UItemBase* x = equipment->getItem<UItemBase>(SlotInformation::QUICK2);
	if (x) x->use(this);
}
void AStalkerGhostsCharacter::OnQuickSlot3()
{
	UItemBase* x = equipment->getItem<UItemBase>(SlotInformation::QUICK3);
	if (x) x->use(this);
}
void AStalkerGhostsCharacter::OnQuickSlot4()
{
	UItemBase* x = equipment->getItem<UItemBase>(SlotInformation::QUICK4);
	if (x) x->use(this);
}

void  AStalkerGhostsCharacter::OnNightVision()
{
	UHelmetItem* x = equipment->getItem<UHelmetItem>(SlotInformation::HELMET);
	static bool isNVon = false;
	if (isNVon)
	{
		normalPPComponent->Settings = normalPostProcessSettings;
		isNVon = false;
		
	}
	else
	{
		normalPPComponent->Settings = nightVisionPostProcessSettings;
		isNVon = true;
		
	}

}

void AStalkerGhostsCharacter::switchFireMode()
{
	if (weapon) weapon->switchFireMode();
	OffFire();
}

void AStalkerGhostsCharacter::switchPerson()
{
	if (!in3rdPerson)
	{
		FollowCamera->Activate();
		FirstPersonCameraComponent->Deactivate();
		in3rdPerson = true;
	}
	else
	{
		FollowCamera->Deactivate();
		FirstPersonCameraComponent->Activate();
		in3rdPerson = false;
	}
}

void  AStalkerGhostsCharacter::footStep(Movement animationStance)
{
	if (animationStance != stance) return;
	FVector Loc = GetActorLocation();
	FVector End = GetActorLocation() + FVector(0, 0, -100);
	FCollisionQueryParams params = FCollisionQueryParams(FName(TEXT("Trace")), true, this);
	FCollisionResponseParams params2 = FCollisionResponseParams();
	FHitResult result(ForceInit);
	const FName TraceTag("MyTraceTag");
	GetWorld()->DebugDrawTraceTag = TraceTag;
	params.TraceTag = TraceTag;
	params.bTraceComplex = true;
	params.bTraceAsyncScene = true;
	params.bReturnPhysicalMaterial = true;
	bool traced = GetWorld()->LineTraceSingleByChannel(result, Loc, End, ECollisionChannel::ECC_Visibility, params, params2);
	if (traced)
	{
		USoundBase* sound = physicsMaterialCollection->getPhysicsSound(result.PhysMaterial.Get()->SurfaceType);
		if (sound) UGameplayStatics::PlaySoundAtLocation(GetWorld(), sound, result.ImpactPoint, 0.5f);
		//do AI stuff here
		//spawn particle for footstepw
	}

}