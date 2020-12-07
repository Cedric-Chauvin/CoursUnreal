// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoursUnrealCppCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine.h"
#include "Pickable.h"
#include "Components/SkeletalMeshComponent.h"
#include "CoursUnrealCppGameMode.h"
#include "ItemActor.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetSystemLibrary.h"

//////////////////////////////////////////////////////////////////////////
// ACoursUnrealCppCharacter

ACoursUnrealCppCharacter::ACoursUnrealCppCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//Create SceneComponent
	ObjectPosition = CreateDefaultSubobject<USceneComponent>(TEXT("ObjectPos"));
	ObjectPosition->SetupAttachment(RootComponent);
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACoursUnrealCppCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Punch", IE_Pressed, this, &ACoursUnrealCppCharacter::Punch);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACoursUnrealCppCharacter::CrouchUnCrouch);
	PlayerInputComponent->BindAction("AIM", IE_Pressed, this, &ACoursUnrealCppCharacter::AimTrue);
	PlayerInputComponent->BindAction("AIM", IE_Released, this, &ACoursUnrealCppCharacter::AimFalse);
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &ACoursUnrealCppCharacter::Pause).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &ACoursUnrealCppCharacter::OpenInventory).bExecuteWhenPaused = true;

	PlayerInputComponent->BindAxis("MoveForward", this, &ACoursUnrealCppCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACoursUnrealCppCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACoursUnrealCppCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACoursUnrealCppCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACoursUnrealCppCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ACoursUnrealCppCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ACoursUnrealCppCharacter::OnResetVR);
}


bool ACoursUnrealCppCharacter::AddItem(FItemData* data)
{
	if (Items.Num() > 15)
		return false;
	Items.Add(data);
	return true;
}

void ACoursUnrealCppCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &ACoursUnrealCppCharacter::NotifyBegin);
}

void ACoursUnrealCppCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ACoursUnrealCppCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ACoursUnrealCppCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ACoursUnrealCppCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	if (IsAiming) {
		FRotator rotation = FRotator(0);
		rotation.Yaw = Controller->GetControlRotation().Yaw;
		SetActorRotation(rotation);
	}
}

void ACoursUnrealCppCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACoursUnrealCppCharacter::MoveForward(float Value)
{
	ForwardAxis = Value;
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACoursUnrealCppCharacter::MoveRight(float Value)
{
	SideAxis = Value;
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ACoursUnrealCppCharacter::Pick()
{
	if (IsAiming) {
		if(CurrentObject)
			CurrentObject->UseObject(this);
		return;
	}
	FVector start = GetActorLocation();
	FVector end = (FollowCamera->GetForwardVector() * PickDistance) + start;
	FHitResult hit;
	if (!GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility))
		return;
	DrawDebugLine(GetWorld(), start, end, FColor(255, 0, 0), true);
	if (CurrentObject == nullptr)
	{
		APickable* pick = Cast<APickable>(hit.Actor.Get());
		if (pick != nullptr) {
			CurrentObject = pick;
			pick->SetActorLocation(ObjectPosition->GetComponentLocation());
			pick->AttachToComponent(ObjectPosition,FAttachmentTransformRules(EAttachmentRule::SnapToTarget,true));
		}
	}
	else {
		CurrentObject->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
		CurrentObject->SetActorLocation(hit.Location,true);
		CurrentObject->SetActorRotation(FRotator(0));
		CurrentObject = nullptr;
	}
}

void ACoursUnrealCppCharacter::CrouchUnCrouch()
{
	if (bIsCrouched)
		UnCrouch();
	else
		Crouch();
}

void ACoursUnrealCppCharacter::AimTrue()
{
	IsAiming = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed /= 4;
	if (bIsCrouched)
		UnCrouch();
}

void ACoursUnrealCppCharacter::AimFalse()
{
	IsAiming = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed *= 4;
}

void ACoursUnrealCppCharacter::Pause()
{
	ACoursUnrealCppGameMode* gameMode = Cast<ACoursUnrealCppGameMode>(GetWorld()->GetAuthGameMode());
	if(gameMode)
		gameMode->GamePause();
}

void ACoursUnrealCppCharacter::OpenInventory()
{
	ACoursUnrealCppGameMode* gameMode = Cast<ACoursUnrealCppGameMode>(GetWorld()->GetAuthGameMode());
	if (!gameMode)
		return;
	TArray<UTexture2D*> textures;
	for (FItemData* item : Items)
	{
		textures.Add(item->Sprite);
	}
	gameMode->OpenInventory(textures);
}

void ACoursUnrealCppCharacter::Punch()
{
	GetMesh()->GetAnimInstance()->Montage_Play(Montage);
}

void ACoursUnrealCppCharacter::NotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& paylod) {
	FVector handLocation = GetMesh()->GetSocketLocation(FName("Punch"));
	TArray<TEnumAsByte<EObjectTypeQuery>> myArray;
	myArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> actorToIgnore;
	actorToIgnore.Add(this);
	TArray<AActor*> OverlapedActors;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), handLocation, 50, myArray, nullptr, actorToIgnore, OverlapedActors);
	for (AActor* actor : OverlapedActors) {
		actor->TakeDamage(AttackDamage, FDamageEvent(), Controller, this);
	}
}
