#include "FHS_AIC_Enemy.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Heroes/GAS/Attributes/FHS_Attributes_Weapon.h"
#include "Perception/AIPerceptionComponent.h"

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
	
	if (const auto* AbilityInterface = Cast<IAbilitySystemInterface>(InPawn))
	{
		if (UAbilitySystemComponent* ASC = AbilityInterface->GetAbilitySystemComponent())
		{
			ASC->GetGameplayAttributeValueChangeDelegate(UFHS_Attributes_Weapon::GetCurrentAmmoAttribute()).AddUObject(
				this, &AFHS_AIC_Enemy::OnCurrentAmmoChanged);
		}
	}

	if (BehaviorTree == nullptr)
	{
		return;
	}
	RunBehaviorTree(BehaviorTree);
	
} // OnPossess

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

void AFHS_AIC_Enemy::UpdateTarget(AActor* NewHeroTarget)
{
	if (NewHeroTarget != nullptr)
	{
		LastActorSet = FDateTime::Now();
		UpdateTargetSight(true);
	}
	
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
	
} // OnCurrentHealthChanged

// ---------------------------------------------------------------------------------------------------------------------
