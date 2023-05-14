#pragma once

#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "FHS_Attributes_CharacterCore.generated.h"

// "AttributeSet.h"
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class HEROES_API UFHS_Attributes_CharacterCore : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_CurrentHealth)
	FGameplayAttributeData CurrentHealth = FGameplayAttributeData(100.f);
	ATTRIBUTE_ACCESSORS(UFHS_Attributes_CharacterCore, CurrentHealth)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth = FGameplayAttributeData(100.f);
	ATTRIBUTE_ACCESSORS(UFHS_Attributes_CharacterCore, MaxHealth)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_CurrentUltPower)
	FGameplayAttributeData CurrentUltimatePower = FGameplayAttributeData(0.f);
	ATTRIBUTE_ACCESSORS(UFHS_Attributes_CharacterCore, CurrentUltimatePower)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,ReplicatedUsing = OnRep_MaxUltPower)
	FGameplayAttributeData MaxUltimatePower = FGameplayAttributeData(100.f);
	ATTRIBUTE_ACCESSORS(UFHS_Attributes_CharacterCore, MaxUltimatePower)

	// From https://github.com/tranek/GASDocumentation#concepts-as-attributes
	UFUNCTION()
	virtual void OnRep_CurrentHealth(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_CurrentUltPower(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_MaxUltPower(const FGameplayAttributeData& OldValue);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
}; // UFHSAttributes_CharacterCore
