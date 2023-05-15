#include "FHS_BaseWeapon.h"

#include "FHS_BaseProjectile.h"
#include "Heroes/Data/FHS_AbilityMeshData.h"
#include "Heroes/GAS/FHS_AbilitySystemComponent.h"
#include "Heroes/Hero/FHS_BaseHero.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// ---------------------------------------------------------------------------------------------------------------------

AFHS_BaseWeapon::AFHS_BaseWeapon() : MuzzleOffset(FVector(100.f, .0f, 10.f))
{
	bReplicates = true;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->CastShadow = false;
	SetRootComponent(Mesh);
	
	ASC = CreateDefaultSubobject<UFHS_AbilitySystemComponent>(TEXT("GAS"));
	ASC->SetIsReplicated(true);
	
} // AFHS_BaseWeapon

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AFHS_BaseWeapon, WeaponData, COND_None);
	DOREPLIFETIME_CONDITION(AFHS_BaseWeapon, HeroOwner, COND_None);
	
} // GetLifetimeReplicatedProps

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	AttachToHero();
	SetupWeapon();
	
} // BeginPlay

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::InitSpawnDeferred(AFHS_BaseHero* NewHeroOwner, UFHS_AbilityMeshData* NewData)
{
	HeroOwner = NewHeroOwner;
	WeaponData = NewData;
		
} // InitSpawnDeferred

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::SetWeaponData_Implementation(UFHS_AbilityMeshData* NewData)
{
	if (NewData == nullptr || NewData == WeaponData)
	{
		return;
	}

	if (WeaponData != nullptr)
	{
		ClearInput();
	}
	
	WeaponData = NewData;
	SetupWeapon();
	
} // SetWeaponData

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::SetupInput()
{
	if (bInputSet || WeaponData == nullptr || HeroOwner == nullptr || HeroOwner->InputComponent == nullptr)
	{
		return;
	}

	bInputSet = true;
	WeaponData->SetupInput(ASC, HeroOwner);
	
} // SetupGAS

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::ClearInput()
{
	WeaponData->ClearInput(HeroOwner);
	bInputSet = false;
	
} // ClearInput

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::PrimaryFire()
{
	if (HeroOwner == nullptr)
	{
		return;
	}
	
	const APlayerController* PC = Cast<APlayerController>(HeroOwner->GetController());
	if (PC == nullptr)
	{
		return;
	}

	UClass* ProjectileClassLoaded = ProjectileClass.LoadSynchronous();
	if (ProjectileClassLoaded == nullptr)
	{
		return;
	}
	
	const FRotator SpawnRotation = PC->PlayerCameraManager->GetCameraRotation();
	const FVector SpawnLocation = HeroOwner->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	// Spawn the projectile at the muzzle
	auto* Bullet = GetWorld()->SpawnActor<AFHS_BaseProjectile>(ProjectileClassLoaded, SpawnLocation, SpawnRotation,
	                                                           ActorSpawnParams);
	Bullet->SetInstigatorUSC(ASC);

	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, HeroOwner->GetActorLocation());
	}
	
	
} // PrimaryFire

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::PlayFireMontage_Implementation()
{
	if (FireAnimation == nullptr)
	{
		return;
	}
	
	UAnimInstance* AnimInstance = HeroOwner->GetMesh1P()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return;
	}
	
	AnimInstance->Montage_Play(FireAnimation, 1.f);
	
} // PlayFireMontage

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::SetupWeapon()
{
	if (WeaponData == nullptr)
	{
		return;
	}
	
	WeaponData->SetupGAS(ASC);
	Mesh->SetSkeletalMesh(WeaponData->Mesh.LoadSynchronous());
	SetupInput();
	
} // SetupWeapon

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::AttachToHero()
{
	if (HeroOwner == nullptr)
	{
		return;
	}
	
	HeroOwner->SetHasRifle(true);
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(HeroOwner->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// ASC needs autonomous proxy
	if (GetNetMode() != NM_Standalone)
	{
		SetAutonomousProxy(true);
	}
	
} // AttachToHero

// ---------------------------------------------------------------------------------------------------------------------

