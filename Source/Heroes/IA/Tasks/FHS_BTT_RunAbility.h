#pragma once

#include "BehaviorTree/Tasks/BTTask_GameplayTaskBase.h"
#include "FHS_BTT_RunAbility.generated.h"

enum class EFHS_AbilityCommand : uint8;

UCLASS()
class HEROES_API UFHS_BTT_RunAbility : public UBTTask_GameplayTaskBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	EFHS_AbilityCommand AbilityCommand;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
}; // UFHS_BTT_SelectRandomPoint
