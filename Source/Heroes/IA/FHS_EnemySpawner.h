#pragma once

#include "GameplayTagContainer.h"
#include "FHS_EnemySpawner.generated.h"

class AFHS_BaseHero;

UCLASS(Meta = (BlueprintSpawnableComponent))
class HEROES_API UFHS_EnemySpawner : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<AFHS_BaseHero> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeToRespawnSinceDeath = 40.f;

	UFUNCTION(BlueprintCallable)
	void Spawn();

	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	TWeakObjectPtr<AFHS_BaseHero> EnemySpawned;

	void EnemySpawnedDead(FGameplayTag DeadTag, int32 NumCounts);
	
}; // UFHS_EnemySpawner
