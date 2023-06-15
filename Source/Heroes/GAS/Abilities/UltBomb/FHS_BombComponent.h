#pragma once

#include "GameplayTagContainer.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "FHS_BombComponent.generated.h"

class UGameplayEffect;

UCLASS(Meta = (BlueprintSpawnableComponent))
class UFHS_BombComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()
	
public:
	UFHS_BombComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bImplodeOnHit = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EObjectTypeQuery> ObjectTypeExplosion;

	UPROPERTY()
	TObjectPtr<UCurveFloat> DamageByDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GEClass;
	
	FGameplayTag GCTag;
	
	float ExplosionRadius = 500.f;
	float MaxDamage = 100.f;
	float ImplodeTime = 2.f;
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void Implode();

	UFUNCTION()
	void OnBombHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

protected:
	FTimerHandle Timer;
	
	float GetDamage(AActor* Actor) const;
	
}; // UFHS_BombComponent
