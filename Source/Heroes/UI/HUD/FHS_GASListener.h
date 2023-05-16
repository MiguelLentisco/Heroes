#pragma once

#include "AbilitySystemComponent.h"
#include "FHS_GASListener.generated.h"

class UAbilitySystemComponent;

UINTERFACE(MinimalAPI, Blueprintable)
class UFHS_GASListener : public UInterface
{
	GENERATED_BODY()
	
}; // UFHS_GASListener

class HEROES_API IFHS_GASListener
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void SetupWithGAS(UAbilitySystemComponent* ASC);
	UFUNCTION(BlueprintNativeEvent)
	void CleanFromGAS(UAbilitySystemComponent* ASC);
	
}; // IFHS_GASListener
