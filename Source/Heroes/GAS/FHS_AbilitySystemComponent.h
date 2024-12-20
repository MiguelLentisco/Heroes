﻿#pragma once

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "FHS_AbilityCommand.h"
#include "FHS_AbilitySystemComponent.generated.h"

class UFHS_AbilitySet;

UCLASS(Meta = (BlueprintSpawnableComponent))
class HEROES_API UFHS_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	UFHS_AbilitySystemComponent();
	
	const FGameplayTag& GetNameTag() const { return NameTag; }
	void SetNameTag(const FGameplayTag& NewName) { NameTag = NewName; }
	const TMap<EFHS_AbilityCommand, FEnhancedInputActionEventBinding*>& GetAbilitiesBind() const { return AbilitiesBind; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void Clear();
	void GiveAbilities(UFHS_AbilitySet* AbilitySet);
	void RemoveAbilities(UFHS_AbilitySet* AbilitySet);
	void BindAbilitiesToInput(const UFHS_AbilitySet* AbilitySet, UInputComponent* Input);

	void ClearInputs(const UFHS_AbilitySet* AbilitySet, UEnhancedInputComponent* EnhancedInput);

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Tags", Meta = (Categories = "Name"))
	FGameplayTag NameTag;
	
	TMap<EFHS_AbilityCommand, FEnhancedInputActionEventBinding*> AbilitiesBind;
	
	void BindAbilityActivationToEnhancedInputComponent(const UFHS_AbilitySet* AbilitySet, UEnhancedInputComponent* Input);
	
}; // UFHS_AbilitySystemComponent
