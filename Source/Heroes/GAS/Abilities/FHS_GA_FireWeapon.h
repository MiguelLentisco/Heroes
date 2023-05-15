#pragma once

#include "FHS_GameplayAbility.h"
#include "FHS_GA_FireWeapon.generated.h"

class AFHS_BaseWeapon;

UCLASS()
class UFHS_GA_FireWeapon : public UFHS_GameplayAbility
{
	GENERATED_BODY()
	
public:
	UFHS_GA_FireWeapon();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	
}; // UFHS_GA_FireWeapon
