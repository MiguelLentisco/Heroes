#pragma once

#include "FHS_ApplyGEPickUp.generated.h"

class UGameplayEffect;
class UFHS_PickUpComponent;

UCLASS()
class HEROES_API AFHS_ApplyGEPickUp : public AActor
{
	GENERATED_BODY()
	
public:
	AFHS_ApplyGEPickUp();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UFHS_PickUpComponent> Trigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UGameplayEffect>> EffectsApplied;

	UFUNCTION()
	void TriggerEffects(ACharacter* Character);
	
}; // AFHS_ApplyGEPickUp
