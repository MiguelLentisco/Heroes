#include "FHS_GeneralPickUp.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "FHS_PickUpComponent.h"
#include "GameFramework/Character.h"

// ---------------------------------------------------------------------------------------------------------------------

AFHS_GeneralPickUp::AFHS_GeneralPickUp()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Trigger = CreateDefaultSubobject<UFHS_PickUpComponent>(TEXT("PickUp"));
	SetRootComponent(Trigger);
	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Trigger->bHideAndRespawn = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Trigger);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
} // AFHS_GeneralPickUp

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_GeneralPickUp::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
	{
		return;
	}
	
	Trigger->OnPickUp.AddDynamic(this, &AFHS_GeneralPickUp::TriggerEffects);
	
} // BeginPlay

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_GeneralPickUp::TriggerEffects(ACharacter* Character)
{
	const auto* AbilityInterface = Cast<IAbilitySystemInterface>(Character);
	if (AbilityInterface == nullptr)
	{
		return;
	}

	UAbilitySystemComponent* USC = AbilityInterface->GetAbilitySystemComponent();
	if (USC == nullptr)
	{
		return;
	}
	
	for (const TSubclassOf<UGameplayEffect>& GE : EffectsApplied)
	{
		USC->ApplyGameplayEffectToSelf(Cast<UGameplayEffect>(GE->ClassDefaultObject), 1, USC->MakeEffectContext());
	}
	
} // TriggerEffects

// ---------------------------------------------------------------------------------------------------------------------
