// Copyright Epic Games, Inc. All Rights Reserved.

#include "FHS_BaseProjectile.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Effects/FHS_GE_MakeDamage.h"

// ---------------------------------------------------------------------------------------------------------------------

AFHS_BaseProjectile::AFHS_BaseProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFHS_BaseProjectile::OnHit);

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	bReplicates = true;

	DamageGE = UFHS_GE_MakeDamage::StaticClass();
	
} // AFHS_BaseProjectile

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_BaseProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                FVector NormalImpulse, const FHitResult& Hit)
{
	if (!HasAuthority() || OtherActor == nullptr || OtherActor == this || (GetInstigator() != nullptr && GetInstigator()
		== OtherActor))
	{
		Destroy();
		return;
	}

	auto* ASI = Cast<IAbilitySystemInterface>(OtherActor);
	if (ASI == nullptr)
	{
		Destroy();
		return;
	}

	UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
	if (ASC == nullptr)
	{
		Destroy();
		return;
	}

	FGameplayEffectSpecHandle GEHandle;
	if (InstigatorUSC != nullptr)
	{
		GEHandle = InstigatorUSC->MakeOutgoingSpec(UFHS_GE_MakeDamage::StaticClass(), 1,
		                                           InstigatorUSC->MakeEffectContext());
	}
	else
	{
		GEHandle = ASC->MakeOutgoingSpec(DamageGE, 1, ASC->MakeEffectContext());
	}

	GEHandle.Data->SetSetByCallerMagnitude(TAG_Data_Damage, DamageValue.GetValueAtLevel(1));
	ASC->ApplyGameplayEffectSpecToSelf(*GEHandle.Data.Get());

	FGameplayCueParameters CueParameters;
	CueParameters.Location = Hit.ImpactPoint;
	CueParameters.Instigator = GetInstigator();
	CueParameters.EffectCauser = this;
	ASC->ExecuteGameplayCue(GCTag, CueParameters);
	
	Destroy();
	
} // OnHit

// ---------------------------------------------------------------------------------------------------------------------
