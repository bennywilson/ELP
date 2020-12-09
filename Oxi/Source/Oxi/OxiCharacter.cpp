// ELP 2020

#include "OxiCharacter.h"
#include "OxiProjectile.h"
#include "OxiWeaponAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AOxiCharacter

AOxiCharacter::AOxiCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	FP_HandsOutline = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_HandsOutline"));
	FP_HandsOutline->SetOnlyOwnerSee(true);
	FP_HandsOutline->SetupAttachment(FirstPersonCameraComponent);
	FP_HandsOutline->bCastDynamicShadow = false;
	FP_HandsOutline->CastShadow = false;
	FP_HandsOutline->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	FP_HandsOutline->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_GunOutline = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_GunOutline"));
	FP_GunOutline->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_GunOutline->bCastDynamicShadow = false;
	FP_GunOutline->CastShadow = false;
	FP_GunOutline->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_GunOutline->SetupAttachment(RootComponent);


	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);
}

void AOxiCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GripPoint"));
	FP_GunOutline->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GripPoint"));

	Mesh1P->SetHiddenInGame(false, true);
	FP_HandsOutline->SetHiddenInGame(false, true);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AOxiCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AOxiCharacter::OnFire);


	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AOxiCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AOxiCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AOxiCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AOxiCharacter::LookUpAtRate);
}

void AOxiCharacter::OnFire()
{
	UOxiWeaponAnimInstance* const HandsAnimInstance = Cast<UOxiWeaponAnimInstance>(Mesh1P->GetAnimInstance());
	if (HandsAnimInstance != nullptr)
	{
		HandsAnimInstance->StartFireWeapon(FirstPersonCameraComponent, false);
	}

	UOxiWeaponAnimInstance* const HandsOutlineAnimInstance = Cast<UOxiWeaponAnimInstance>(FP_HandsOutline->GetAnimInstance());
	if (HandsOutlineAnimInstance != nullptr)
	{
		HandsOutlineAnimInstance->StartFireWeapon(FirstPersonCameraComponent, true);
	}
}

void AOxiCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AOxiCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AOxiCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AOxiCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
