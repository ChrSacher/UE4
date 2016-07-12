// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "StalkerGhosts.h"
#include "StalkerGhostsCharacter.h"
#include "Bullet.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/InputSettings.h"

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
	currentInventory->loadUI();
	currentInventory->addItem(currentWeapon->currentMagazine);
	currentInventory->refresh();
	FirstPersonCameraComponent->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Camera"));
	
	GetCharacterMovement()->MaxWalkSpeed = currentSpeed = jogSpeed;
	changeWeapon(currentWeapon);
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
	

	Fire();
	if(stance == Movement::JOGGING || stance ==  Movement::SPRINTING) changeStance(Movement::WALKING);
	GetWorld()->GetTimerManager().SetTimer(fireHandle, this, &AStalkerGhostsCharacter::Fire, 60 / currentWeapon->fireRate, true);

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
	if (!currentWeapon) return;
	
	UWorld* const World = GetWorld();
	if (!World) return;

	const FRotator SpawnRotation = GetControlRotation();
	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	FVector SpawnLocation;
	if (FP_MuzzleLocation != nullptr) SpawnLocation = FP_MuzzleLocation->GetComponentLocation();
		else SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(GunOffset);

	if(!currentWeapon->Fire(SpawnLocation, SpawnRotation)) OffFire();
	else
	{
		if (currentWeapon->FireAnimation != NULL)
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
			if (AnimInstance != NULL)
			{
				AnimInstance->Montage_Play(currentWeapon->FireAnimation, 1.f);
			}
		}
	}
}

void AStalkerGhostsCharacter::changeWeapon(UWeaponComponent* newWeapon)
{
	if (!newWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("WEAPON = NULL"));
		return;
	}
	if (!newWeapon->mesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("WEAPONMESH = NULL"));
		return;
	}
	newWeapon->mesh->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));//Attach gun mesh component to Skeleton, doing it here because the skelton is not yet created in the constructor
	FP_MuzzleLocation->AttachToComponent(newWeapon->mesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Muzzle"));
	//newWeapon->mesh->SetRelativeLocation(newWeapon->mesh->GetSocketLocation("GripPoint"));
	newWeapon->mesh->SetVisibility(true);
}

void AStalkerGhostsCharacter::MoveForward(float Value)
{
	characterAnim->speed = GetVelocity().Size() / sprintSpeed * 100;
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
	if (!currentInventory) return;
	
	if (!checkMag(currentInventory->lookForItems(currentWeapon->currentMagazine->name)))
	{
		for (int i = 0; i < currentWeapon->acceptedBullets.Num(); i++)
		{

			if (checkMag(currentInventory->lookForItems(currentWeapon->acceptedBullets[i]))) break;
			

		}
		return; //couldn 't find mag
	}
	//found mag and reload is possible
	if (currentWeapon->ReloadAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		if (characterAnim != NULL)
		{
			characterAnim->Montage_Play(currentWeapon->ReloadAnimation, currentWeapon->reloadTime);
		}
	}

	return;
}

void AStalkerGhostsCharacter::offReload()
{

}
bool AStalkerGhostsCharacter::checkMag(TArray<UItemBase*> Items)
{
	if (Items.Num() == 0) return false;
	for (int i = 0; i < Items.Num(); i++)
	{
		if (currentWeapon->currentMagazine->currentAmmoCount <= currentWeapon->currentMagazine->ammoCapacity)
		{
			if (Items[i]->ammount >= (currentWeapon->currentMagazine->ammoCapacity - currentWeapon->currentMagazine->currentAmmoCount))
			{
				currentWeapon->currentMagazine->currentAmmoCount = currentWeapon->currentMagazine->ammoCapacity;
				Items[i]->ammount =- currentWeapon->currentMagazine->ammoCapacity - currentWeapon->currentMagazine->currentAmmoCount;
			}
			else
			{
				currentWeapon->currentMagazine->currentAmmoCount += Items[i]->ammount;
				Items[i]->ammount = 0;
			}
			
		}
		
	}
	return true;
	
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
	changeWeapon(currentWeapon);
}
void AStalkerGhostsCharacter::Sprint()
{
	
	switch (stance)
	{
		case Movement::SPRINTING:
		{
			GetCharacterMovement()->MaxWalkSpeed = currentSpeed = sprintSpeed;
			stamina -= sprintCost;
			if (stamina <= 0)
			{
				stamina = 0;
				changeStance(Movement::JOGGING);
				
			}
		}break;
		case Movement::WALKING:
		{
			GetCharacterMovement()->MaxWalkSpeed = currentSpeed = walkSpeed;
		}break;
		case Movement::JOGGING:
		{
			GetCharacterMovement()->MaxWalkSpeed = currentSpeed = jogSpeed;
			GetWorld()->GetTimerManager().ClearTimer(speedHandle);
				return;
		}break;
		case Movement::CROUCHING:
		{
			GetCharacterMovement()->MaxWalkSpeed = currentSpeed = (jogSpeed - walkSpeed) / 4 + walkSpeed;
		}break;
		case Movement::PRONING:
		{
			GetCharacterMovement()->MaxWalkSpeed = currentSpeed = walkSpeed;
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
	
	if (stamina >= maxStamina)
	{
		stamina = maxStamina;
		GetWorld()->GetTimerManager().ClearTimer(staminaHandle);
		return;
	}
	stamina += staminaRegen;
	
	
	
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
	FRotator Rot = GetActorRotation();
	FVector End = Loc + Rot.Vector() * 600; //make variable
	FCollisionQueryParams params = FCollisionQueryParams(FName(TEXT("Trace")), true, this);
	FCollisionResponseParams params2 = FCollisionResponseParams();
	FHitResult result(ForceInit);
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

void AStalkerGhostsCharacter::doDamage(DamageBodyPart BodyPart, ABullet* Cause)
{

}