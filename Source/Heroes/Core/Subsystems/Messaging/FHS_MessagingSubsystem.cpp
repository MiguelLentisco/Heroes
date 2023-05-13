#include "FHS_MessagingSubsystem.h"

// ---------------------------------------------------------------------------------------------------------------------

FDelegateHandle UFHS_MessagingSubsystem::AddEventListener(const FGameplayTag& Name, const FOnMessageSent& Listener)
{
	if (!Name.IsValid())
	{
		return FDelegateHandle();
	}
	
	if (!EventsListeners.Contains(Name))
	{
		EventsListeners.Add(Name, {});
	}

	const FDelegateHandle ListenerHandle = Listener.GetHandle();
	EventsListeners[Name].Add(ListenerHandle, Listener);
	return ListenerHandle;
	
} // AddEvent

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_MessagingSubsystem::RemoveEventListener(const FGameplayTag& Name, const FDelegateHandle& Listener)
{
	if (!EventsListeners.Contains(Name))
	{
		return;
	}
	
	EventsListeners[Name].Remove(Listener);
	
} // RemoveEventListener

// ---------------------------------------------------------------------------------------------------------------------

void UFHS_MessagingSubsystem::SendEvent(const FGameplayTag& Name, UFHS_Message* Message)
{
	if (!EventsListeners.Contains(Name))
	{
		return;
	}

	for (const TPair<FDelegateHandle, FOnMessageSent>& Listener : EventsListeners[Name])
	{
		Listener.Value.ExecuteIfBound(Message);
	}
	
} // SendEvent

// ---------------------------------------------------------------------------------------------------------------------
