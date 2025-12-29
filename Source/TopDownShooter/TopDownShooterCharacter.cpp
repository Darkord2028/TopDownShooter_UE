// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDownShooterCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DrawDebugHelpers.h"

//////////////////////////////////////////////////////////////////////////
// ATopDownShooterCharacter

ATopDownShooterCharacter::ATopDownShooterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

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

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATopDownShooterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (!EnhancedInputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cast failed for EnhancedInputComponent"));
		return;
	}

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopDownShooterCharacter::Move);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ATopDownShooterCharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ATopDownShooterCharacter::StopJumping);

	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ATopDownShooterCharacter::Aim);
	EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ATopDownShooterCharacter::Aim);
}


void ATopDownShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot get player controller"));
		return;
	}

	PlayerController->bShowMouseCursor = true;
	PlayerController->bEnableClickEvents = true;
	PlayerController->bEnableMouseOverEvents = true;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (Subsystem && DefaultMappingContext)
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

}

void ATopDownShooterCharacter::Tick(float DeltaTime)
{
	DrawDebug();
	HandleAim(DeltaTime);
}

void ATopDownShooterCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementInput = Value.Get<FVector2D>();

	if (!Controller || MovementInput.IsNearlyZero())
	{
		return;
	}

	// Use controller yaw only
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// X = Forward/Backward, Y = Right/Left
	AddMovementInput(ForwardDirection, MovementInput.X);
	AddMovementInput(RightDirection, MovementInput.Y);
}

void ATopDownShooterCharacter::Aim(const FInputActionValue& Value)
{
	bIsAiming = Value.Get<bool>();
	GetCharacterMovement()->bOrientRotationToMovement = !bIsAiming;
}

void ATopDownShooterCharacter::HandleAim(float DeltaTime)
{
	if (!bIsAiming) return;

	FVector WorldOrigin, WorldDirection;
	PlayerController->DeprojectMousePositionToWorld(WorldOrigin, WorldDirection);

	float T = (GetActorLocation().Z - WorldOrigin.Z) / WorldDirection.Z;
	FVector MouseWorldPosition = WorldOrigin + WorldDirection * T;

	FVector ToMouse = MouseWorldPosition - GetActorLocation();
	ToMouse.Z = 0.0f;
	
	if (ToMouse.IsNearlyZero()) return;

	FRotator TargetRotation = ToMouse.Rotation();

	SetActorRotation(FMath::RInterpTo(
		GetActorRotation(),
		TargetRotation,
		DeltaTime,
		12.0f
	));

	
}

void ATopDownShooterCharacter::DrawDebug()
{
	FVector FirePointLocation = GetMesh()->GetSocketLocation(TEXT("FirePoint"));
	FRotator FirePointRotation = GetMesh()->GetSocketRotation(TEXT("FirePoint"));

	FVector ActorDir = GetActorLocation() + GetActorForwardVector() * 400.0f;
	FVector FirePointDir = FirePointLocation + FirePointRotation.Vector() * 400.f;
	FVector FirePointForward =
		GetMesh()->GetSocketTransform(TEXT("FirePoint"), RTS_World)
		.GetUnitAxis(EAxis::X);

	FVector WorldOrigin, WorldDirection;
	PlayerController->DeprojectMousePositionToWorld(WorldOrigin, WorldDirection);

	float T = (GetActorLocation().Z - WorldOrigin.Z) / WorldDirection.Z;
	FVector MouseWorldPosition = WorldOrigin + WorldDirection * T;

	DrawDebugLine(
		GetWorld(),
		FirePointLocation,
		FirePointDir,
		FColor::Red,
		false,
		0.f,
		0,
		2.f
	);

	DrawDebugLine(
		GetWorld(),
		GetActorLocation(),
		ActorDir,
		FColor::Yellow,
		false,
		0.f,
		0,
		2.f
	);

	DrawDebugLine(
		GetWorld(),
		FirePointLocation,
		MouseWorldPosition,
		FColor::Blue,
		false,
		0.f,
		0,
		2.f
	);

	FVector FireToCursor =
		(MouseWorldPosition - FirePointLocation).GetSafeNormal();

	float Dot =
		FVector::DotProduct(FirePointForward, FireToCursor);

	Dot = FMath::Clamp(Dot, -1.f, 1.f);

	float AngleDegreeError =
		FMath::RadiansToDegrees(FMath::Acos(Dot));

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("AimError = %.2f deg | Dot = %.3f"),
		AngleDegreeError,
		Dot
	);

}
