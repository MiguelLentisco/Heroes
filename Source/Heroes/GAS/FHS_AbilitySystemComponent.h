#pragma once

#include "AbilitySystemComponent.h"
#include "FHS_AbilitySystemComponent.generated.h"

class UFHS_AbilitySet;

UCLASS()
class HEROES_API UFHS_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void GiveAbilities(UFHS_AbilitySet* Abilities);
	UFUNCTION(BlueprintCallable, Category = "Input")
	void BindAbilityActivationToEnhancedInputComponent(UEnhancedInputComponent* Input);

	const FGameplayTag& GetNameTag() const { return NameTag; }
	void SetNameTag(const FGameplayTag& NewName) { NameTag = NewName; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags", Meta = (Categories = "Name"))
	FGameplayTag NameTag;
	
	
}; // UFHS_AbilitySystemComponent
