#pragma once

#include "FHS_Message.generated.h"

UCLASS(Abstract)
class HEROES_API UFHS_Message : public UObject
{
	GENERATED_BODY()
	
}; // UFHS_Message

UCLASS()
class HEROES_API UFHS_BasicMessage : public UFHS_Message
{
	GENERATED_BODY()
	
public:
	union
	{
		int32 Int;
		bool Bool;
		float Float;
		TWeakObjectPtr<UObject> Object;
	};
	
	static UFHS_BasicMessage* CreateBasicMessage(UObject* Owner, int32 IntValue);
	static UFHS_BasicMessage* CreateBasicMessage(UObject* Owner, bool BoolValue);
	static UFHS_BasicMessage* CreateBasicMessage(UObject* Owner, float FloatValue);
	static UFHS_BasicMessage* CreateBasicMessage(UObject* Owner, UObject* ObjectValue);
	
}; // UFHS_BasicMessage

UCLASS()
class HEROES_API UFHS_StringMessage : public UFHS_Message
{
	GENERATED_BODY()
	
public:
	FString String;
	
	static UFHS_StringMessage* CreateStringMessage(UObject* Owner, const FString& StringValue);
	
}; // UFHS_StringMessage