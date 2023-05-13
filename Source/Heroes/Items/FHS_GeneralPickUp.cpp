#include "FHS_GeneralPickUp.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "FHS_PickUpComponent.h"
#include "GameFramework/Character.h"

// ---------------------------------------------------------------------------------------------------------------------

AFHS_GeneralPickUp::AFHS_GeneralPickUp()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Trigger = CreateDefaultSubobject<UFHS_PickUpComponent>(TEXT("PickUp"));
	Trigger->SetupAttachment(Mesh);
	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Trigger->bHideAndRespawn = true;
	
} // AFHS_GeneralPickUp

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_GeneralPickUp::BeginPlay()
{
	Super::BeginPlay();

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
