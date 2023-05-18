#pragma once

#include "BehaviorTree/Tasks/BTTask_GameplayTaskBase.h"
#include "FHS_BTT_SelectRandomPoint.generated.h"

UCLASS()
class HEROES_API UFHS_BTT_SelectRandomPoint : public UBTTask_GameplayTaskBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetLocationKey;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = "0"))
	float RadiusAroundPawn = 1000.0f;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
}; // UFHS_BTT_SelectRandomPoint
