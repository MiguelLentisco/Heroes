#include "FHS_AIC_Enemy.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "BrainComponent.h"
#include "GameplayEffectExtension.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Heroes/GAS/FHS_GameplayTags.h"
#include "Heroes/GAS/Attributes/FHS_Attributes_CharacterCore.h"
#include "Heroes/GAS/Attributes/FHS_Attributes_Weapon.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

// ---------------------------------------------------------------------------------------------------------------------

AFHS_AIC_Enemy::AFHS_AIC_Enemy()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AFHS_AIC_Enemy::OnTargetPerceptionUpdated);
	
} // AFHS_AIC_Enemy

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_AIC_Enemy::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTree == nullptr)
	{
		return;
	}
	
	if (const auto* AbilityInterface = Cast<IAbilitySystemInterface>(InPawn))
	{
		if (UAbilitySystemComponent* ASC = AbilityInterface->GetAbilitySystemComponent())
		{
			ASC->GetGameplayAttributeValueChangeDelegate(UFHS_Attributes_Weapon::GetCurrentAmmoAttribute()).AddUObject(
				this, &AFHS_AIC_Enemy::OnCurrentAmmoChanged);
			ASC->GetGameplayAttributeValueChangeDelegate(UFHS_Attributes_CharacterCore::GetCurrentHealthAttribute()).AddUObject(
				this, &AFHS_AIC_Enemy::OnCurrentHealthChanged);
			ASC->RegisterGameplayTagEvent(TAG_Status_Dead.GetTag()).AddUObject(this, &AFHS_AIC_Enemy::OnPawnDead);
		}
	}
	
	RunBehaviorTree(BehaviorTree);
	
} // OnPossess

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_AIC_Enemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (const auto* AbilityInterface = Cast<IAbilitySystemInterface>(GetPawn()))
	{
		if (UAbilitySystemComponent* ASC = AbilityInterface->GetAbilitySystemComponent())
		{
			ASC->GetGameplayAttributeValueChangeDelegate(UFHS_Attributes_Weapon::GetCurrentAmmoAttribute()).RemoveAll(this);
			ASC->GetGameplayAttributeValueChangeDelegate(UFHS_Attributes_CharacterCore::GetCurrentHealthAttribute()).
				 RemoveAll(this);
		}
	}

	GetWorldTimerManager().ClearTimer(StopChaseTarget);
	
	Super::EndPlay(EndPlayReason);
	
} // EndPlay

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_AIC_Enemy::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Actor == nullptr || !Actor->ActorHasTag(TEXT("Player")))
	{
		return;
	}

	// Update the list of targets
	const bool bTargetSensed = Stimulus.WasSuccessfullySensed();
	if (bTargetSensed)
	{
		HeroesDetected.Add(Actor);
	}
	else
	{
		HeroesDetected.Remove(Actor);
	}
	HeroesDetected.Remove(nullptr);

	// We have vision of nobody, update and start the countdown to patrol
	if (HeroesDetected.IsEmpty())
	{
		UpdateTargetSight(false);

		// On stealth force 
		if (HeroTarget != nullptr && HeroTarget->ActorHasTag(TEXT("OnStealth")))
		{
			UpdateTarget(nullptr);
		}
		
		return;
	}

	// We can't select another target until some time (avoid changing too much)
	const FDateTime Now = FDateTime::Now();
	if ((Now - LastActorSet).GetTotalSeconds() < SecondsUntilChangeTarget)
	{
		// Update if our target changed
		if (Actor == HeroTarget)
		{
			UpdateTargetSight(bTargetSensed);
		}
		return;
	}
	
	// We select another enemy based on proximity
	UpdateTarget(FindNearestHero());
	
} // OnTargetPerceptionUpdated

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_AIC_Enemy::UpdateTargetSight(bool bTargetVision)
{
	if (bTargetVision)
	{
		GetWorldTimerManager().ClearTimer(StopChaseTarget);
	}
	else
	{
		const FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &AFHS_AIC_Enemy::UpdateTarget,
																	  static_cast<AActor*>(nullptr));
		GetWorldTimerManager().SetTimer(StopChaseTarget, Delegate, SecondsUntilStopChase, false);
	}

	GetBlackboardComponent()->SetValueAsBool(TEXT("bTargetOnSight"), bTargetVision);
	
} // UpdateTargetSight

// ---------------------------------------------------------------------------------------------------------------------

AActor* AFHS_AIC_Enemy::FindNearestHero() const
{
	const FVector Origin = GetPawn()->GetActorLocation();
	AActor* NearestActor = nullptr;
	float NearestDistance = TNumericLimits<float>::Max();

	for (AActor* Hero : HeroesDetected)
	{
		const float CheckDistance = (Origin - Hero->GetActorLocation()).SizeSquared();
		if (CheckDistance >= NearestDistance)
		{
			continue;
		}
		
		NearestActor = Hero;
		NearestDistance = CheckDistance;
	}

	return NearestActor;
	
} // FindNearestHero


// ---------------------------------------------------------------------------------------------------------------------

void AFHS_AIC_Enemy::UpdateTarget(AActor* NewHeroTarget)
{
	if (HeroTarget != nullptr)
	{
		UAbilitySystemComponent* ASC = Cast<IAbilitySystemInterface>(HeroTarget)->GetAbilitySystemComponent();
		ASC->RegisterGameplayTagEvent(TAG_Status_Dead.GetTag()).RemoveAll(this);
	}
	
	if (NewHeroTarget != nullptr)
	{
		LastActorSet = FDateTime::Now();
		UpdateTargetSight(true);
		UAbilitySystemComponent* ASC = Cast<IAbilitySystemInterface>(NewHeroTarget)->GetAbilitySystemComponent();
		ASC->RegisterGameplayTagEvent(TAG_Status_Dead.GetTag()).AddUObject(this, &AFHS_AIC_Enemy::OnTargetDead);
	}

	HeroTarget = NewHeroTarget;
	GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), NewHeroTarget);
	
} // UpdateTarget

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_AIC_Enemy::UpdateOutOfAmmo(bool bOutOfAmmo)
{
	GetBlackboardComponent()->SetValueAsBool(TEXT("bOutOfAmmo"), bOutOfAmmo);
	
} // UpdateOutOfAmmo

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_AIC_Enemy::UpdateLowHealth(bool bLowHealth)
{
	GetBlackboardComponent()->SetValueAsBool(TEXT("bLowHealth"), bLowHealth);
	
} // UpdateLowHealth

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_AIC_Enemy::OnCurrentAmmoChanged(const FOnAttributeChangeData& Data)
{
	UpdateOutOfAmmo(FMath::IsNearlyZero(Data.NewValue, .01f));
	
} // OnCurrentAmmoChanged

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_AIC_Enemy::OnCurrentHealthChanged(const FOnAttributeChangeData& Data)
{
	UpdateLowHealth(Data.NewValue <= LowHealthThreshold);

	const FGameplayEffectModCallbackData* CallbackData = Data.GEModData;
	if (GetBlackboardComponent()->GetValueAsBool(TEXT("bTargetOnSight")) || CallbackData == nullptr)
	{
		return;
	}

	AActor* DamageInstigator = CallbackData->EffectSpec.GetEffectContext().GetInstigator();
	if (DamageInstigator == nullptr || !DamageInstigator->ActorHasTag(TEXT("Player")))
	{
		return;
	}

	UpdateTarget(DamageInstigator);
	
} // OnCurrentHealthChanged

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_AIC_Enemy::OnPawnDead(FGameplayTag DeadTag, int32 NumCounts)
{
	if (NumCounts <= 0)
	{
		return;
	}
	
	GetBrainComponent()->StopLogic(TEXT("Dead"));
	GetPerceptionComponent()->SetSenseEnabled(UAISense_Sight::StaticClass(), false);
	
	FTimerHandle AuxTimer;
	GetWorldTimerManager().SetTimer(AuxTimer, this, &AFHS_AIC_Enemy::DestroyWhenDead, TimeUntilDespawn, false);
	
} // OnPawnDead

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_AIC_Enemy::OnTargetDead(FGameplayTag DeadTag, int32 NumCounts)
{
	HeroesDetected.Remove(HeroTarget);
	UpdateTarget(FindNearestHero());
	UpdateTargetSight(!HeroesDetected.IsEmpty());
	
} // OnTargetDead

// ---------------------------------------------------------------------------------------------------------------------

void AFHS_AIC_Enemy::DestroyWhenDead()
{
	GetPawn()->Destroy();
	Destroy();
	
} // DestroyWhenDead

// ---------------------------------------------------------------------------------------------------------------------
