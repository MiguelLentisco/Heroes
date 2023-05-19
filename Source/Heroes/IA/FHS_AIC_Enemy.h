#pragma once

#include "AIController.h"
#include "GameplayEffectTypes.h"
#include "Perception/AIPerceptionTypes.h"
#include "GameplayTags.h"
#include "FHS_AIC_Enemy.generated.h"

UCLASS()
class HEROES_API AFHS_AIC_Enemy : public AAIController
{
	GENERATED_BODY()
	
public:
	AFHS_AIC_Enemy();

	AActor* GetHeroTarget() const { return HeroTarget; }
	virtual void OnPossess(APawn* InPawn) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBehaviorTree> BehaviorTree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = "0", Units = s))
	float SecondsUntilChangeTarget = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = "0", Units = s))
	float SecondsUntilStopChase = 8.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = "0"))
	float LowHealthThreshold = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = "0"))
	float TimeUntilDespawn = 5.f;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TSet<TObjectPtr<AActor>> HeroesDetected;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AActor> HeroTarget;

	FTimerHandle StopChaseTarget;
	FDateTime LastActorSet = FDateTime::MinValue();
	
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	AActor* FindNearestHero() const;
	
	void UpdateTargetSight(bool bTargetVision);
	void UpdateTarget(AActor* NewHeroTarget);
	void UpdateOutOfAmmo(bool bOutOfAmmo);
	void UpdateLowHealth(bool bLowHealth);

	void OnCurrentAmmoChanged(const FOnAttributeChangeData& Data);
	void OnCurrentHealthChanged(const FOnAttributeChangeData& Data);
	void OnPawnDead(FGameplayTag DeadTag, int32 NumCounts);
	void OnTargetDead(FGameplayTag DeadTag, int32 NumCounts);

	void DestroyWhenDead();
	
}; // AFHS_AIC_Enemy
