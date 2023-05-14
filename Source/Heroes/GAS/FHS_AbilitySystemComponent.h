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
	
	const FGameplayTag& GetNameTag() const { return NameTag; }
	void SetNameTag(const FGameplayTag& NewName) { NameTag = NewName; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void Clear();
	
	void GiveAbilities(UFHS_AbilitySet* AbilitySet);
	void BindAbilitiesToInput(UFHS_AbilitySet* AbilitySet, UInputComponent* Input);

protected:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Tags", Meta = (Categories = "Name"))
	FGameplayTag NameTag;
	
	void BindAbilityActivationToEnhancedInputComponent(UFHS_AbilitySet* AbilitySet, UEnhancedInputComponent* Input);
	
}; // UFHS_AbilitySystemComponent
