#pragma once

#include "FHS_GASListener.h"
#include "GameFramework/HUD.h"
#include "FHS_HUD.generated.h"

class UFHS_UW_HUD;

UCLASS(Abstract)
class HEROES_API AFHS_HUD : public AHUD, public IFHS_GASListener
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFHS_HUD();

	virtual void SetupWithGAS_Implementation(UAbilitySystemComponent* ASC) override;
	virtual void CleanFromGAS_Implementation(UAbilitySystemComponent* ASC) override;

	void CreateHUD();
	bool IsHUDCreated() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (MustImplement = "FHS_GASListener", AllowAbstract = false))
	TSoftClassPtr<UFHS_UW_HUD> MainHUDClass;

	UPROPERTY()
	TObjectPtr<UFHS_UW_HUD> MainHUD;
	
}; // AFHS_HUD
