#include "FHS_ApplyGEPickUp.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "FHS_PickUpComponent.h"
#include "GameFramework/Character.h"

// ---------------------------------------------------------------------------------------------------------------------

AFHS_ApplyGEPickUp::AFHS_ApplyGEPickUp()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Trigger = CreateDefaultSubobject<UFHS_PickUpComponent>(TEXT("PickUp"));
	SetRootComponent(Trigger);
	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Trigger->bHideAndRespawn = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Trigger);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
} // AFHS_ApplyGEPickUp

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_ApplyGEPickUp::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
	{
		return;
	}
	
	Trigger->OnPickUp.AddDynamic(this, &AFHS_ApplyGEPickUp::TriggerEffects);
	
} // BeginPlay

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_ApplyGEPickUp::TriggerEffects(ACharacter* Character)
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
