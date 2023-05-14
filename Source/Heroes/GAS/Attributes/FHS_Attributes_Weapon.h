#pragma once

#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "FHS_Attributes_Weapon.generated.h"

// "AttributeSet.h"
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class HEROES_API UFHS_Attributes_Weapon : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_CurrentAmmo)
	FGameplayAttributeData CurrentAmmo = FGameplayAttributeData(10.f);
	ATTRIBUTE_ACCESSORS(UFHS_Attributes_Weapon, CurrentAmmo)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_AmmoClip)
	FGameplayAttributeData AmmoClip = FGameplayAttributeData(10.f);
	ATTRIBUTE_ACCESSORS(UFHS_Attributes_Weapon, AmmoClip)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_FireRate)
	FGameplayAttributeData FireRate = FGameplayAttributeData(0.5f);
	ATTRIBUTE_ACCESSORS(UFHS_Attributes_Weapon, FireRate)
	
	// From https://github.com/tranek/GASDocumentation#concepts-as-attributes
	UFUNCTION()
	virtual void OnRep_CurrentAmmo(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_AmmoClip(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	virtual void OnRep_FireRate(const FGameplayAttributeData& OldValue);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
}; // FHS_Attributes_Weapon
