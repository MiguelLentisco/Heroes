// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "FHS_PickUpComponent.generated.h"

// Declaration of the delegate that will be called when someone picks this up
// The character picking this up is the parameter sent with the notification
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickUp, ACharacter*, PickUpCharacter);

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HEROES_API UFHS_PickUpComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	UFHS_PickUpComponent();

	UPROPERTY(Replicated, ReplicatedUsing = "OnRep_bPickedUp")
	bool bPickedUp = false;

	/** Delegate to whom anyone can subscribe to receive this event */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnPickUp OnPickUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHideAndRespawn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (EditCondition = "bHideAndRespawn", ClampMin = 0))
	float RespawnTime = 30.f;

	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void OnRep_bPickedUp();

protected:
	FTimerHandle RespawnTimer;
	
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                          const FHitResult& SweepResult);

	void RespawnPickup();
	
}; // UFHS_PickUpComponent