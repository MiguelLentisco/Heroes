#include "FHS_BombComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Effects/FHS_GE_MakeDamage.h"
#include "Kismet/KismetSystemLibrary.h"

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_BombComponent::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &UFHS_BombComponent::Implode, ImplodeTime);
	
} // BeginPlay

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_BombComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(Timer);
	
	Super::EndPlay(EndPlayReason);
	
} // EndPlay

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_BombComponent::Implode()
{
	auto* Instigator = GetOwner()->GetInstigator<IAbilitySystemInterface>();
	if (Instigator == nullptr)
	{
		GetOwner()->Destroy();
		return;
	}
	
	UAbilitySystemComponent* InstigatorASC = Instigator->GetAbilitySystemComponent();

	// Play sound
	FGameplayCueParameters CueParameters;
	CueParameters.Instigator = InstigatorASC->GetOwner();
	CueParameters.Location = GetOwner()->GetActorLocation();
	CueParameters.EffectCauser = GetOwner();
	InstigatorASC->ExecuteGameplayCue(GCTag, CueParameters);

	// Damage heroes found
	TArray<AActor*> HeroesFound;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetOwner()->GetActorLocation(), ExplosionRadius,
											  {ObjectTypeExplosion}, nullptr, {GetOwner()}, HeroesFound);
	if (HeroesFound.IsEmpty())
	{
		GetOwner()->Destroy();
		return;
	}
	
	for (AActor* Hero : HeroesFound)
	{
		const auto* HeroInterface = Cast<IAbilitySystemInterface>(Hero);
		if (HeroInterface == nullptr)
		{
			continue;
		}

		FGameplayEffectSpecHandle GEHandle;
		UAbilitySystemComponent* ASC = HeroInterface->GetAbilitySystemComponent();
		if (InstigatorASC != nullptr)
		{
			GEHandle = InstigatorASC->MakeOutgoingSpec(UFHS_GE_MakeDamage::StaticClass(), 1, InstigatorASC->MakeEffectContext());
		}
		else
		{
			GEHandle = ASC->MakeOutgoingSpec(UFHS_GE_MakeDamage::StaticClass(), 1, ASC->MakeEffectContext());
		}
		
		GEHandle.Data->SetSetByCallerMagnitude(TAG_Data_Damage, -GetDamage(Hero));
		ASC->ApplyGameplayEffectSpecToSelf(*GEHandle.Data.Get());
	}

	GetOwner()->Destroy();
	
} // Implode

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_BombComponent::OnBombHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == nullptr)
	{
		return;
	}

	GetOwner()->SetActorEnableCollision(false);
	GetOwner()->SetActorLocation(Hit.ImpactPoint);
	GetOwner()->AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
	
} // OnBombHit

// ---------------------------------------------------------------------------------------------------------------------

float UFHS_BombComponent::GetDamage(AActor* Actor) const
{
	const float Distance = (Actor->GetActorLocation() - GetOwner()->GetActorLocation()).Size();
	return DamageByDistance->GetFloatValue(Distance / ExplosionRadius) * MaxDamage;
	
} // GetDamage

// ---------------------------------------------------------------------------------------------------------------------

