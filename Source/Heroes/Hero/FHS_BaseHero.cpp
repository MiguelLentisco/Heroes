#include "FHS_BaseHero.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FHS_HeroData.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Heroes/Data/FHS_AbilityMeshData.h"
#include "Heroes/GAS/FHS_AbilitySet.h"
#include "Heroes/GAS/FHS_AbilitySystemComponent.h"
#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Attributes/FHS_Attributes_CharacterCore.h"
#include "Heroes/Weapons/FHS_BaseWeapon.h"
#include "Net/UnrealNetwork.h"

// ---------------------------------------------------------------------------------------------------------------------

AFHS_BaseHero::AFHS_BaseHero()
{
	bReplicates = true;
	SetupConstructor();

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(GetMesh());
	CameraSpringArm->SetUsingAbsoluteRotation(true);

	Camera3P = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera3P"));
	Camera3P->SetupAttachment(CameraSpringArm);
	Camera3P->bAutoActivate = false;

	ASC = CreateDefaultSubobject<UFHS_AbilitySystemComponent>(TEXT("GAS"));
	ASC->SetIsReplicated(true);

} // AFHS_BaseHero

// ---------------------------------------------------------------------------------------------------------------------

UAbilitySystemComponent* AFHS_BaseHero::GetAbilitySystemComponent() const
{
	return ASC;
	
} // GetAbilitySystemComponent

// ---------------------------------------------------------------------------------------------------------------------

bool AFHS_BaseHero::IsInputBlocked() const
{
	return bDead || bStunned;
	
} // IsInputBlocked

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(AFHS_BaseHero, HeroData, COND_None);
	DOREPLIFETIME_CONDITION(AFHS_BaseHero, CurrentWeapon, COND_None);
	DOREPLIFETIME_CONDITION(AFHS_BaseHero, bDead, COND_None);
	
} // GetLifetimeReplicatedProps

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	BindMovementActions(PlayerInputComponent);

	if (!HasActorBegunPlay())
	{
		return;
	}
		
	SetupGASInput(true);
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->SetupInput(true);
	}
	
} // SetupPlayerInputComponent

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::BeginPlay()
{
	Super::BeginPlay();

	MeshOriginalTransform = GetMesh()->GetRelativeTransform();

	if (HasAuthority())
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UFHS_Attributes_CharacterCore::GetSpeedAttribute()).AddUObject(
		this, &AFHS_BaseHero::SpeedUpdated);
		ASC->GetGameplayAttributeValueChangeDelegate(UFHS_Attributes_CharacterCore::GetCurrentHealthAttribute()).AddUObject(
			this, &AFHS_BaseHero::HealthUpdated);
		ASC->RegisterGameplayTagEvent(TAG_Status_Dead.GetTag()).AddUObject(this, &AFHS_BaseHero::OnPlayerDeadChanged);
		ASC->RegisterGameplayTagEvent(TAG_Status_Stunnned.GetTag()).AddUObject(this, &AFHS_BaseHero::OnPlayerStunChanged);

		if (HeroData != nullptr)
		{
			SetupGAS();
			SetupWeapons();
		}
	}

	OnRep_HeroData(nullptr);
	
} // BeginPlay

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::SpeedUpdated(const FOnAttributeChangeData& AttributeData)
{
	GetCharacterMovement()->MaxWalkSpeed = AttributeData.NewValue;
	
} // SetSpeed

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::HealthUpdated(const FOnAttributeChangeData& AttributeData)
{
	if (bDead || !FMath::IsNearlyZero(AttributeData.NewValue))
	{
		return;
	}

	KillHero();
	
} // HealthUpdated

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::KillHero()
{
	bDead = true;
	ASC->SetNumericAttributeBase(UFHS_Attributes_CharacterCore::GetCurrentHealthAttribute(), 0.f);
	ASC->SetTagMapCount(TAG_Status_Dead, 1);
	
} // KillHero

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::OnPlayerDeadChanged(const FGameplayTag DeadTag, int32 NumCount)
{
	bDead = NumCount > 0;
	if (bDead)
	{
		ASC->CancelAllAbilities();
		ASC->RemoveActiveEffects({});
	}
	else
	{
		InitStats();
	}

	OnRep_bDead();
	
} // OnPlayerDeadChanged

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::OnPlayerStunChanged(const FGameplayTag StunTag, int32 NumCount)
{
	bStunned = NumCount > 0;
	if (!bStunned)
	{
		return;
	}

	ASC->CancelAllAbilities();
	
} // OnPlayerStunChanged

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::SetHeroData_Implementation(UFHS_HeroData* NewHeroData)
{
	if (NewHeroData == nullptr || NewHeroData == HeroData)
	{
		return;
	}
	
	UFHS_HeroData* PreviousData = HeroData;
	
	HeroData = NewHeroData;
	SetupGAS();
	SetupWeapons();
	OnRep_HeroData(PreviousData);
	
} // SetHeroData

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::OnRep_bDead()
{
	GetMesh()->SetSimulatePhysics(bDead);
	GetMesh()->SetCollisionProfileName(bDead ? TEXT("Ragdoll") : TEXT("CharacterMesh"));
	GetCharacterMovement()->StopMovementImmediately();

	if (!bDead)
	{
		GetMesh()->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		GetMesh()->SetRelativeTransform(MeshOriginalTransform, false, nullptr, ETeleportType::ResetPhysics);
	}

	if (CurrentWeapon == nullptr)
	{
		return;
	}

	if (bDead)
	{
		const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		CurrentWeapon->AttachToComponent(GetMesh(), AttachmentRules, Hand3PSocket);
	}
	else
	{
		CurrentWeapon->AttachToHero();
	}
	
} // OnRep_bDead

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::OnRep_HeroData(UFHS_HeroData* PreviousData)
{
	if (PreviousData != nullptr)
	{
		SetupGASInput(false);
	}
	
	SetupMeshes();
	SetupGASInput(true);
	
} // OnRep_HeroData

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::OnRep_CurrentWeapon()
{
	OnMainWeaponChanged.Broadcast(CurrentWeapon);
	
} // OnRep_CurrentWeapons

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::InitStats()
{
	for (const FAttributeDefaults& Attribute : HeroData->Attributes)
	{
		ASC->InitStats(Attribute.Attributes, Attribute.DefaultStartingTable);
	}

	for (const TSoftClassPtr<UGameplayEffect>& GEClass : HeroData->InitialEffects)
	{
		ASC->BP_ApplyGameplayEffectToSelf(GEClass.LoadSynchronous(), 1, ASC->MakeEffectContext());
	}
	
} // InitStats

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::SetupGAS()
{
	if (HeroData == nullptr)
	{
		return;
	}

	// Reset all
	ASC->Clear();
	SetupGASInput(false);
	
	ASC->SetNameTag(HeroData->Name);
	InitStats();
	ASC->GiveAbilities(HeroData->AbilitySet.LoadSynchronous());
	
} // SetupGAS

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::SetupWeapons()
{
	if (HeroData == nullptr)
	{
		return;
	}

	const TArray<TSoftObjectPtr<UFHS_AbilityMeshData>> WeaponsData = HeroData->WeaponsData;

	// Already available weapons
	const int32 WeaponsToSetNum = FMath::Min(Weapons.Num(), WeaponsData.Num());
	for (int32 i = 0; i < WeaponsToSetNum; ++i)
	{
		Weapons[i]->SetWeaponData(WeaponsData[i].LoadSynchronous());
	}
	
	const int32 WeaponsToSpawnNum = WeaponsData.Num() - Weapons.Num();

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	// Need to spawn more
	for (int32 i = WeaponsToSetNum; i < WeaponsToSetNum + WeaponsToSpawnNum; ++i)
	{
		Weapons.Add(GetWorld()->SpawnActor<AFHS_BaseWeapon>(SpawnParameters));
		Weapons[i]->SetHeroOwner(this);
		Weapons[i]->SetWeaponData(WeaponsData[i].LoadSynchronous());
	}

	// Remove if we don't need them
	const int32 WeaponsToDeleteNum = -WeaponsToSpawnNum;
	for (int32 i = WeaponsToSetNum + WeaponsToDeleteNum; i > WeaponsToSetNum; --i)
	{
		Weapons[i]->Destroy();
		Weapons.RemoveAt(i);
	}

	// Update current weapon
	CurrentWeapon = nullptr;
	if (Weapons.IsValidIndex(0))
	{
		CurrentWeapon = Weapons[0];
		CurrentWeapon->SetMainWeapon(true);
	}

	OnRep_CurrentWeapon();
	
} // SetupWeaponMesh

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::SetupMeshes()
{
	if (HeroData == nullptr)
	{
		return;
	}
	
	Mesh1P->SetSkeletalMeshAsset(HeroData->Hero1PMesh.LoadSynchronous());
	GetMesh()->SetSkeletalMeshAsset(HeroData->Mesh.LoadSynchronous());
	
} // SetupMeshes

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseHero::SetupGASInput(bool bSetupInput)
{
	if (bSetupInput == bInputSet || HeroData == nullptr || InputComponent == nullptr)
	{
		return;
	}
	
	bInputSet = bSetupInput;
	if (bInputSet)
	{
		HeroData->SetupInput(ASC, this);
	}
	else
	{
		HeroData->ClearInput(ASC, this);
	}
	OnHeroInputChanged.Broadcast(bInputSet);
	
} // SetupGASInput

// ---------------------------------------------------------------------------------------------------------------------

#pragma region FPS_Template

bool AFHS_BaseHero::GetHasRifle() const
{
	return CurrentWeapon != nullptr;
	
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
	if (IsInputBlocked() || Controller == nullptr)
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
	if (IsInputBlocked() || Controller == nullptr)
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

