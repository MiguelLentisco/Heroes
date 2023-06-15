#include "FHS_LaunchProjectile.h"

#include "GameFramework/Character.h"
#include "Heroes/GAS/FHS_AbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_LaunchProjectile::ReactOnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (!HasAuthority())
	{
		return;
	}
	
	// Damage heroes found
	TArray<AActor*> HeroesFound;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), ExplosionRadius, {ObjectTypeExplosion},
	                                          ACharacter::StaticClass(), {this}, HeroesFound);

	if (HeroesFound.IsEmpty())
	{
		Destroy();
		return;
	}

	for (AActor* Hero : HeroesFound)
	{
		auto* HeroChar = Cast<ACharacter>(Hero);
		const FVector LaunchVector = LaunchSpeed * (HeroChar->GetActorLocation() - Hit.Location).GetSafeNormal();
		HeroChar->LaunchCharacter(LaunchVector, true, true);
	}

	FGameplayCueParameters CueParameters;
	CueParameters.Location = Hit.ImpactPoint;
	CueParameters.Instigator = GetInstigator();
	CueParameters.EffectCauser = this;
	
	if (InstigatorUSC != nullptr)
	{
		InstigatorUSC->ExecuteGameplayCue(GCTag, CueParameters);
	}

	Destroy();
	
} // ReactOnHit

// ---------------------------------------------------------------------------------------------------------------------
