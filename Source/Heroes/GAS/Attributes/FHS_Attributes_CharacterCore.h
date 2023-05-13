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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData CurrentHealth = FGameplayAttributeData(100.f);
	ATTRIBUTE_ACCESSORS(UFHS_Attributes_CharacterCore, CurrentHealth)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData MaxHealth = FGameplayAttributeData(100.f);
	ATTRIBUTE_ACCESSORS(UFHS_Attributes_CharacterCore, MaxHealth)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData CurrentUltimatePower = FGameplayAttributeData(0.f);
	ATTRIBUTE_ACCESSORS(UFHS_Attributes_CharacterCore, CurrentUltimatePower)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData MaxUltimatePower = FGameplayAttributeData(100.f);
	ATTRIBUTE_ACCESSORS(UFHS_Attributes_CharacterCore, MaxUltimatePower)
	
}; // UFHSAttributes_CharacterCore
