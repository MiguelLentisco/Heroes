#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "FHS_AT_Dash.generated.h"

class UCharacterMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDashEndedDlg);

UCLASS()
class HEROES_API UFHS_AT_Dash : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FDashEndedDlg DashEnded;
	
	UFUNCTION(BlueprintCallable, Category = Tasks, meta = (DefaultToSelf = "OwningAbility",
		BlueprintInternalUseOnly = "TRUE", DisplayName = "GT Dash"))
	static UFHS_AT_Dash* Dash(UGameplayAbility* OwningAbility, const FVector& Direction, float Speed, float Duration);

	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

protected:
	UPROPERTY()
	TWeakObjectPtr<UCharacterMovementComponent> CMC;
	
	FVector Direction;
	float Duration = 0.f;

	float Speed = 0.f;
	float OriginalSpeed = 0.f;

	FTimerHandle Timer;

	void RestoreCMC() const;
	void OnTimeEnded();
	
}; // UFHS_AT_Dash
