#pragma once

#include "AttributeSet.h"
#include "FHSAttributes_CharacterCore.generated.h"

UCLASS()
class HEROES_API UFHSAttributes_CharacterCore : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData CurrentHealth = FGameplayAttributeData(100.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData MaxHealth = FGameplayAttributeData(100.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayAttributeData Speed = FGameplayAttributeData(150.f);
	
}; // UFHSAttributes_CharacterCore
