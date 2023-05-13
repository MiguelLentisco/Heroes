#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTags.h"
#include "FHS_MessagingSubsystem.generated.h"

class UFHS_Message;

DECLARE_DELEGATE_OneParam(FOnMessageSent, UFHS_Message*)

UCLASS()
class HEROES_API UFHS_MessagingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	FDelegateHandle AddEventListener(const FGameplayTag& Name, const FOnMessageSent& Listener);
	void RemoveEventListener(const FGameplayTag& Name, const FDelegateHandle& Listener);
	void SendEvent(const FGameplayTag& Name, UFHS_Message* Message);

protected:
	TMap<FGameplayTag, TMap<FDelegateHandle, FOnMessageSent>> EventsListeners;
	
}; // UFHS_MessagingSubsystem