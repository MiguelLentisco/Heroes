#include "FHS_BaseHero.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FHS_HeroData.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Heroes/GAS/FHS_AbilitySystemComponent.h"
#include "Heroes/GAS/Attributes/FHS_Attributes_CharacterCore.h"
#include "Heroes/GAS/FHS_AbilitySet.h"

// ---------------------------------------------------------------------------------------------------------------------

AFHS_BaseHero::AFHS_BaseHero()
{
	SetupConstructor();

	ASC = CreateDefaultSubobject<UFHS_AbilitySystemComponent>(TEXT("GAS"));

} // AFHS_BaseHero

// ---------------------------------------------------------------------------------------------------------------------

UAbilitySystemComponent* AFHS_BaseHero::GetAbilitySystemComponent() const
{
	return ASC;
	
} // GetAbilitySystemComponent

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::SetupHero(TObjectPtr<UFHS_HeroData> HeroData)
{
	if (HeroData == nullptr)
	{
		return;
	}

	SetupGAS(HeroData);
	SetupMeshes(HeroData);
	
} // SetupHero

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	SetupMovementIMC();

} // BeginPlay

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	BindMovementActions(PlayerInputComponent);
	
} // SetupPlayerInputComponent

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::SetupGAS(TObjectPtr<UFHS_HeroData> HeroData)
{
	// Hard reset
	ASC->DestroyComponent();
	ASC = NewObject<UFHS_AbilitySystemComponent>(this);
	ASC->RegisterComponent();

	ASC->SetNameTag(HeroData->HeroName);
	for (const FAttributeDefaults& Attribute : HeroData->Attributes)
	{
		ASC->InitStats(Attribute.Attributes, Attribute.DefaultStartingTable);
	}
	ASC->SetupAbilities(HeroData->AbilitySet.LoadSynchronous(), Cast<APlayerController>(GetController()));
	
} // SetupGAS

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::SetupMeshes(TObjectPtr<UFHS_HeroData> HeroData)
{
	Mesh1P->SetSkeletalMeshAsset(HeroData->Hero1PMesh.LoadSynchronous());
	GetMesh()->SetSkeletalMeshAsset(HeroData->Hero3PMesh.LoadSynchronous());
	
} // SetupMeshes

// ---------------------------------------------------------------------------------------------------------------------

#pragma region FPS_Template

void AFHS_BaseHero::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
	
} // SetHasRifle

// ---------------------------------------------------------------------------------------------------------------------

bool AFHS_BaseHero::GetHasRifle()
{
	return bHasRifle;
	
} // GetHasRifle

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::SetupConstructor()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	GetMesh()->SetVisibility(false);
	
} // SetupConstructor

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::SetupMovementIMC()
{
	const APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController == nullptr)
	{
		return;
	}

	auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (Subsystem == nullptr)
	{
		return;
	}

	Subsystem->AddMappingContext(MovementMappingContext, 0);
	
} // SetupMovementIMC

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::BindMovementActions(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent == nullptr)
	{
		return;
	}
	
	//Moving
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFHS_BaseHero::Move);
	//Looking
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFHS_BaseHero::Look);
	
} // BindMovementActions

// ---------------------------------------------------------------------------------------------------------------------


void AFHS_BaseHero::Move(const FInputActionValue& Value)
{
	if (Controller == nullptr)
	{
		return;
	}
	
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();
	
	// add movement 
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
	
} // Move

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::Look(const FInputActionValue& Value)
{
	if (Controller == nullptr)
	{
		return;
	}
	
	// input is a Vector2D
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	// add yaw and pitch input to controller
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
	
} // Look

#pragma endregion // FPS_Template

// ---------------------------------------------------------------------------------------------------------------------
