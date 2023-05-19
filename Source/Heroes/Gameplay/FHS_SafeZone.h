#pragma once

#include "Engine/TriggerBox.h"
#include "ActiveGameplayEffectHandle.h"
#include "FHS_SafeZone.generated.h"

class AFHS_BaseHero;

UCLASS()
class AFHS_SafeZone : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	AFHS_SafeZone();

protected:
	UPROPERTY()
	TMap<TWeakObjectPtr<AFHS_BaseHero>, FActiveGameplayEffectHandle> ImmunityGEActives;
	
	UFUNCTION()
	void OnPawnEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnPawnExited(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                  int32 OtherBodyIndex);
	
	
}; // AFHS_SafeZone
