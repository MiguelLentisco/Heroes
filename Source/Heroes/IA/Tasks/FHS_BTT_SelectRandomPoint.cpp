#include "FHS_BTT_SelectRandomPoint.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

// ---------------------------------------------------------------------------------------------------------------------

EBTNodeResult::Type UFHS_BTT_SelectRandomPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if (Pawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FVector TargetLocation = Pawn->GetActorLocation();
	FVector RandomLocation;
	if (UNavigationSystemV1::K2_GetRandomReachablePointInRadius(Pawn, TargetLocation, RandomLocation, RadiusAroundPawn))
	{
		TargetLocation = RandomLocation;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLocationKey.SelectedKeyName, TargetLocation);
	return EBTNodeResult::Succeeded;
	
} // UFHS_BTT_SelectRandomPoint

// ---------------------------------------------------------------------------------------------------------------------

