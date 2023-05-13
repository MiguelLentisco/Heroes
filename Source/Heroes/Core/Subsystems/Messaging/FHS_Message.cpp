#include "FHS_Message.h"

// ---------------------------------------------------------------------------------------------------------------------

/*static*/ UFHS_BasicMessage* UFHS_BasicMessage::CreateBasicMessage(UObject* Owner, int32 IntValue)
{
	UFHS_BasicMessage* Message = NewObject<UFHS_BasicMessage>(Owner);
	Message->Int = IntValue;
	return Message;
	
} // CreateBasicMessage

// ---------------------------------------------------------------------------------------------------------------------

/*static*/ UFHS_BasicMessage* UFHS_BasicMessage::CreateBasicMessage(UObject* Owner, bool BoolValue)
{
	UFHS_BasicMessage* Message = NewObject<UFHS_BasicMessage>(Owner);
	Message->Bool = BoolValue;
	return Message;
	
} // CreateBasicMessage

// ---------------------------------------------------------------------------------------------------------------------

/*static*/ UFHS_BasicMessage* UFHS_BasicMessage::CreateBasicMessage(UObject* Owner, float FloatValue)
{
	UFHS_BasicMessage* Message = NewObject<UFHS_BasicMessage>(Owner);
	Message->Float = FloatValue;
	return Message;
	
} // CreateBasicMessage

// ---------------------------------------------------------------------------------------------------------------------

/*static*/ UFHS_BasicMessage* UFHS_BasicMessage::CreateBasicMessage(UObject* Owner, UObject* ObjectValue)
{
	UFHS_BasicMessage* Message = NewObject<UFHS_BasicMessage>(Owner);
	Message->Object = ObjectValue;
	return Message;
	
} // CreateBasicMessage

// ---------------------------------------------------------------------------------------------------------------------

UFHS_StringMessage* UFHS_StringMessage::CreateStringMessage(UObject* Owner, const FString& StringValue)
{
	UFHS_StringMessage* Message = NewObject<UFHS_StringMessage>(Owner);
	Message->String = StringValue;
	return Message;
	
} // CreateStringMessage

// ---------------------------------------------------------------------------------------------------------------------
