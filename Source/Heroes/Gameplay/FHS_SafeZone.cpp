#include "FHS_SafeZone.h"

#include "AbilitySystemComponent.h"
#include "Components/ShapeComponent.h"
#include "Heroes/GAS/Effects/FHS_GE_FullHealth.h"
#include "Heroes/GAS/Effects/FHS_GE_GrantImmunity.h"
#include "Heroes/Hero/FHS_BaseHero.h"

// ---------------------------------------------------------------------------------------------------------------------

AFHS_SafeZone::AFHS_SafeZone()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCollisionComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCollisionComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &AFHS_SafeZone::OnPawnEntered);
	GetCollisionComponent()->OnComponentEndOverlap.AddDynamic(this, &AFHS_SafeZone::OnPawnExited);
	
} // AFHS_SafeZone

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_SafeZone::OnPawnEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority() || OtherActor == nullptr)
	{
		return;
	}

	auto* Hero = Cast<AFHS_BaseHero>(OtherActor);
	if (Hero == nullptr)
	{
		return;
	}

	UAbilitySystemComponent* ASC = Hero->GetAbilitySystemComponent();
	const FActiveGameplayEffectHandle Handle = ASC->BP_ApplyGameplayEffectToSelf(
		UFHS_GE_GrantImmunity::StaticClass(), 1, ASC->MakeEffectContext());
	ASC->BP_ApplyGameplayEffectToSelf(UFHS_GE_FullHealth::StaticClass(), 1, ASC->MakeEffectContext());
	ImmunityGEActives.Add(Hero, Handle);
	
} // OnPawnEntered

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_SafeZone::OnPawnExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!HasAuthority() || OtherActor == nullptr)
	{
		return;
	}

	auto* Hero = Cast<AFHS_BaseHero>(OtherActor);
	if (Hero == nullptr || !ImmunityGEActives.Contains(Hero))
	{
		return;
	}

	UAbilitySystemComponent* ASC = Hero->GetAbilitySystemComponent();
	ASC->RemoveActiveGameplayEffect(ImmunityGEActives[Hero]);
	
} // OnPawnExited

// ---------------------------------------------------------------------------------------------------------------------
