#pragma once

#include "AttributeSet.h"
#include "GameplayTags.h"
#include "FHS_RewindComponent.generated.h"

class UAbilitySystemComponent;

struct HEROES_API FFHS_RewindData
{
	float Timestamp = 0.f;

	FVector Velocity = FVector::ZeroVector;
	FVector Position = FVector::ZeroVector;
	FRotator ControlRotation = FRotator::ZeroRotator;

	TMap<FGameplayAttribute, int32> AttributesCurves;
	
}; // FFHS_RewindData

UCLASS(Meta = (BlueprintSpawnableComponent))
class HEROES_API UFHS_RewindComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UFHS_RewindComponent();

	void SetRewindTime(float NewRewindTime) { RewindTime = NewRewindTime; }

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void StartRecording();
	void Rewind();

	void OnHeroDead(const FGameplayTag DeadTag, int32 NumTag);

protected:
	UPROPERTY()
	TWeakObjectPtr<UAbilitySystemComponent> HeroASC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RewindTime = 3.f;

	float CurrentTime = 0.f;

	TQueue<FFHS_RewindData> RewindData;
	
	void RecordData();
	
}; // UFHS_RewindComponent
