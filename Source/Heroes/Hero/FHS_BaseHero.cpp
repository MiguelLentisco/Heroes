#include "FHS_BaseHero.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FHS_HeroData.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Heroes/GAS/FHS_AbilitySystemComponent.h"
#include "Heroes/GAS/Attributes/FHS_Attributes_CharacterCore.h"
#include "Heroes/Weapons/FHS_BaseWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// ---------------------------------------------------------------------------------------------------------------------

AFHS_BaseHero::AFHS_BaseHero()
{
	bReplicates = true;
	SetupConstructor();

	ASC = CreateDefaultSubobject<UFHS_AbilitySystemComponent>(TEXT("GAS"));
	ASC->SetIsReplicated(true);

} // AFHS_BaseHero

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::BeginPlay()
{
	Super::BeginPlay();

	if (HeroData != nullptr)
	{
		SetupHero();
	}
	
} // BeginPlay

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::GetWeaponUSCs(TArray<UAbilitySystemComponent*>& WeaponUSCs) const
{
	WeaponUSCs.Empty();
	for (AFHS_BaseWeapon* Weapon : Weapons)
	{
		WeaponUSCs.Add(Weapon->GetAbilitySystemComponent());
	}
	
} // GetWeaponUSCs

// ---------------------------------------------------------------------------------------------------------------------


void AFHS_BaseHero::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(AFHS_BaseHero, Weapons, COND_None);
	
} // GetLifetimeReplicatedProps

// ---------------------------------------------------------------------------------------------------------------------

UAbilitySystemComponent* AFHS_BaseHero::GetAbilitySystemComponent() const
{
	return ASC;
	
} // GetAbilitySystemComponent

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::SetHeroData(UFHS_HeroData* NewHeroData)
{
	if (NewHeroData == nullptr || NewHeroData == HeroData)
	{
		return;
	}
	
	if (HeroData != nullptr)
	{
		OnHeroCleared.ExecuteIfBound();
		ClearGASInput();
	}
	
	HeroData = NewHeroData;
	SetupHero();
	
} // SetHeroData

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	BindMovementActions(PlayerInputComponent);
	SetupGASInput();
	
} // SetupPlayerInputComponent

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::SetupGASInput()
{
	if (bInputSet || InputComponent == nullptr)
	{
		return;
	}

	bInputSet = true;
	
	HeroData->SetupInput(ASC, this);
	for (AFHS_BaseWeapon* Weapon : Weapons)
	{
		Weapon->SetupInput();
	}
	
} // SetupGASInput

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::ClearGASInput()
{
	HeroData->ClearInput(this);
	for (AFHS_BaseWeapon* Weapon : Weapons)
	{
		Weapon->ClearInput();
	}
	
	bInputSet = false;
	
} // ClearGASInput

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::SetupHero()
{
	if (HeroData == nullptr)
	{
		return;
	}
	
	HeroData->SetupGAS(ASC);
	Mesh1P->SetSkeletalMeshAsset(HeroData->Hero1PMesh.LoadSynchronous());
	GetMesh()->SetSkeletalMeshAsset(HeroData->Mesh.LoadSynchronous());
	SetupGASInput();
	SetupWeapon();
	OnHeroReady.ExecuteIfBound();
	
} // SetupHero

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::SetupWeapon()
{
	// Only spawn on server, the weapons will set themselves on BeginPlay;
	if (!HasAuthority() || HeroData == nullptr)
	{
		return;
	}

	const TArray<TSoftObjectPtr<UFHS_AbilityMeshData>> WeaponsData = HeroData->WeaponsData;

	// Already available weapons
	const int32 WeaponsToSetNum = FMath::Min(Weapons.Num(), WeaponsData.Num());
	for (int32 i = 0; i < WeaponsToSetNum; ++i)
	{
		Weapons[i]->SetWeaponData(WeaponsData[i].LoadSynchronous()); // multinet
	}
	
	const int32 WeaponsToSpawnNum = WeaponsData.Num() - Weapons.Num();
	if (WeaponsToSpawnNum == 0)
	{
		return;
	}

	// Need to spawn more
	for (int32 i = WeaponsToSetNum; i < WeaponsToSetNum + WeaponsToSpawnNum; ++i)
	{
		Weapons.Add(GetWorld()->SpawnActorDeferred<AFHS_BaseWeapon>(AFHS_BaseWeapon::StaticClass(),
		                                                             FTransform::Identity, this));
		Weapons[i]->InitSpawnDeferred(this, WeaponsData[i].LoadSynchronous());
		UGameplayStatics::FinishSpawningActor(Weapons[i], FTransform::Identity);
	}

	// Remove if we don't need them
	const int32 WeaponsToDeleteNum = -WeaponsToSpawnNum;
	for (int32 i = WeaponsToSetNum + WeaponsToDeleteNum; i > WeaponsToSetNum; --i)
	{
		Weapons[i]->Destroy();
		Weapons.RemoveAt(i);
	}
	
} // SetupWeapon

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

	GetMesh()->SetOwnerNoSee(true);
	
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

	SetupMovementIMC();
	
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

