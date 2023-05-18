#include "FHS_BTT_RunAbility.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"

// ---------------------------------------------------------------------------------------------------------------------

EBTNodeResult::Type UFHS_BTT_RunAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto* AbilityInterface = Cast<IAbilitySystemInterface>(OwnerComp.GetAIOwner()->GetPawn());
	if (AbilityInterface == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UAbilitySystemComponent* ASC = AbilityInterface->GetAbilitySystemComponent();
	if (ASC == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	
	ASC->PressInputID(static_cast<int32>(AbilityCommand));
	return EBTNodeResult::Succeeded;
	
} // ExecuteTask

// ---------------------------------------------------------------------------------------------------------------------

