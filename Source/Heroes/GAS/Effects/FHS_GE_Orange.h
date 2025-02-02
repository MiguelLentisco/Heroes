#pragma once

#include "GameplayEffect.h"
#include "FHS_GE_Orange.generated.h"

UCLASS()
class HEROES_API UFHS_GE_Orange : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	UFHS_GE_Orange();

#if WITH_EDITOR
	virtual void PostCDOCompiled(const FPostCDOCompiledContext& Context) override;
#endif // WITH_EDITOR
	
}; // UFHS_GE_Orange
