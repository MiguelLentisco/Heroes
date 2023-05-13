#pragma once

#include "AbilitySystemComponent.h"
#include "FHS_AbilitySystemComponent.generated.h"

class UFHS_AbilitySet;
class UInputMappingContext;

UCLASS(Meta = (BlueprintSpawnableComponent))
class HEROES_API UFHS_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	UFHS_AbilitySystemComponent();
	
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void SetupAbilities(UFHS_AbilitySet* AbilitySet, APlayerController* PC = nullptr);
	
	const FGameplayTag& GetNameTag() const { return NameTag; }
	void SetNameTag(const FGameplayTag& NewName) { NameTag = NewName; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags", Meta = (Categories = "Name"))
	FGameplayTag NameTag;

	void GiveAbilities(UFHS_AbilitySet* AbilitySet);
	void BindAbilitiesToInput(UFHS_AbilitySet* AbilitySet, UInputComponent* Input);
	void BindAbilityActivationToEnhancedInputComponent(UFHS_AbilitySet* AbilitySet, UEnhancedInputComponent* Input);
	void AddInputMappingContext(UInputMappingContext* IMC, APlayerController* PC) const;
	
}; // UFHS_AbilitySystemComponent
