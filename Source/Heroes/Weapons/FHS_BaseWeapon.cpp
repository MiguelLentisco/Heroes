#include "FHS_BaseWeapon.h"

#include "FHS_BaseProjectile.h"
#include "Heroes/Data/FHS_AbilityMeshData.h"
#include "Heroes/GAS/FHS_AbilitySet.h"
#include "Heroes/GAS/FHS_AbilitySystemComponent.h"
#include "Heroes/Hero/FHS_BaseHero.h"
#include "Heroes/IA/FHS_AIC_Enemy.h"
#include "Net/UnrealNetwork.h"

// ---------------------------------------------------------------------------------------------------------------------

AFHS_BaseWeapon::AFHS_BaseWeapon()
{
	bReplicates = true;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->CastShadow = false;
	SetRootComponent(Mesh);
	
} // AFHS_BaseWeapon

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AFHS_BaseWeapon, WeaponData, COND_None);
	DOREPLIFETIME_CONDITION(AFHS_BaseWeapon, HeroOwner, COND_None);
	DOREPLIFETIME_CONDITION(AFHS_BaseWeapon, bMainWeapon, COND_None);
	
} // GetLifetimeReplicatedProps

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::SetHeroOwner(AFHS_BaseHero* NewHero)
{
	if (!HasAuthority() || NewHero == nullptr)
	{
		return;
	}

	HeroOwner = NewHero;
	InitGAS();
	OnRep_HeroOwner();
	
} // SetHeroOwner_Implementation

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::SetWeaponData(UFHS_AbilityMeshData* NewData)
{
	if (!HasAuthority() || NewData == nullptr)
	{
		return;
	}

	// If data change then we are not longer the main weapon
	SetMainWeapon(false);
	
	UFHS_AbilityMeshData* OldData = NewData;
	WeaponData = NewData;
	InitGAS();
	OnRep_WeaponData(OldData);
	
} // SetWeaponData

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::SetMainWeapon(bool bNewMainWeapon)
{
	if (!HasAuthority())
	{
		return;
	}
	
	bMainWeapon = bNewMainWeapon;
	ChangeAbilityStatus();
	OnRep_bMainWeapon();
	
} // SetMainWeapon

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::InitStats()
{
	if (WeaponData == nullptr || HeroOwner == nullptr)
	{
		return;
	}

	UAbilitySystemComponent* ASC = HeroOwner->GetAbilitySystemComponent();
	for (const FAttributeDefaults& Attribute : WeaponData->Attributes)
	{
		ASC->InitStats(Attribute.Attributes, Attribute.DefaultStartingTable);
	}
	for (const TSoftClassPtr<UGameplayEffect>& GEClass : WeaponData->InitialEffects)
	{
		ASC->BP_ApplyGameplayEffectToSelf(GEClass.LoadSynchronous(), 1, ASC->MakeEffectContext());
	}
	
} // InitStats

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::SetupInput(bool bEnable)
{
	if (bInputSet == bEnable || WeaponData == nullptr || HeroOwner == nullptr || HeroOwner->InputComponent == nullptr)
	{
		return;
	}

	bInputSet = bEnable;
	if (bInputSet)
	{
		WeaponData->SetupInput(HeroOwner->GetFHSAbilitySystemComponent(), HeroOwner);
	}
	else
	{
		WeaponData->ClearInput(HeroOwner->GetFHSAbilitySystemComponent(), HeroOwner);
	}
	OnWeaponInputChanged.Broadcast(bInputSet);
	
} // SetupGAS

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::PrimaryFire(const TSubclassOf<AFHS_BaseProjectile>& ProjectileClass, const FVector& MuzzleOffset)
{
	if (!HasAuthority() || HeroOwner == nullptr || ProjectileClass == nullptr)
	{
		return;
	}

	const FVector MuzzlePoint = Mesh->GetSocketLocation(TEXT("Muzzle"));
	FRotator SpawnRotation = HeroOwner->GetControlRotation();
	
	// Auto aim for AIs since I can't deal with IKs rn
	if (auto* AIC = Cast<AFHS_AIC_Enemy>(HeroOwner->GetController()))
	{
		APawn* Target = Cast<APawn>(AIC->GetHeroTarget());
		const FVector Dir = (Target->GetActorLocation() + FVector(0.f, 0.f, 35.f) - MuzzlePoint).GetSafeNormal();
		SpawnRotation = FRotationMatrix::MakeFromX(Dir).Rotator();
	}

	const FVector SpawnLocation = MuzzlePoint + SpawnRotation.RotateVector(MuzzleOffset);
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	SpawnParameters.Instigator = HeroOwner;

	// Spawn the projectile at the muzzle
	auto* Bullet = GetWorld()->SpawnActor<AFHS_BaseProjectile>(ProjectileClass, SpawnLocation, SpawnRotation,
	                                                           SpawnParameters);
	if (Bullet == nullptr)
	{
		return;
	}
	
	Bullet->SetInstigator(HeroOwner);
	Bullet->SetInstigatorUSC(HeroOwner->GetAbilitySystemComponent());
	
} // PrimaryFire

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::PlayFireMontage_Implementation(UAnimMontage* ShootAnim)
{
	if (ShootAnim == nullptr || HeroOwner == nullptr)
	{
		return;
	}
	
	UAnimInstance* AnimInstance = HeroOwner->GetMesh1P()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return;
	}
	
	AnimInstance->Montage_Play(ShootAnim, 1.f);
	
} // PlayFireMontage

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::AttachToHero()
{
	if (HeroOwner == nullptr)
	{
		return;
	}
	
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(HeroOwner->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
	
} // AttachToHero

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::OnRep_WeaponData(UFHS_AbilityMeshData* OldWeaponData)
{
	if (OldWeaponData != nullptr)
	{
		SetupInput(false);
	}
	
	SetupWeaponMesh();
	SetupInput(true);
	
} // OnRep_WeaponData

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::OnRep_HeroOwner()
{
	AttachToHero();
	SetupInput(true);
	
} // OnRep_HeroOwner

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::OnRep_bMainWeapon()
{
	SetActorHiddenInGame(!bMainWeapon);
	SetupInput(bMainWeapon);
	
} // OnRep_bMainWeapon

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::InitGAS()
{
	if (HeroOwner == nullptr || WeaponData == nullptr)
	{
		return;
	}

	UAbilitySystemComponent* ASC = HeroOwner->GetAbilitySystemComponent();
	ASC->SetTagMapCount(WeaponData->Name, 1);
	InitStats();
	
} // InitGAS

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::ChangeAbilityStatus()
{
	if (HeroOwner == nullptr || WeaponData == nullptr)
	{
		return;
	}

	UFHS_AbilitySet* AbilitySet = WeaponData->AbilitySet.LoadSynchronous();
	if (bMainWeapon)
	{
		HeroOwner->GetFHSAbilitySystemComponent()->GiveAbilities(AbilitySet);
	}
	else
	{
		HeroOwner->GetFHSAbilitySystemComponent()->RemoveAbilities(AbilitySet);
	}
	
} // ChangeAbilityStatus

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseWeapon::SetupWeaponMesh()
{
	if (WeaponData == nullptr)
	{
		return;
	}
	
	Mesh->SetSkeletalMesh(WeaponData->Mesh.LoadSynchronous());
	
} // SetupWeaponMesh

// ---------------------------------------------------------------------------------------------------------------------


