// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "FHS_BaseHero.generated.h"

class UFHS_HeroData;
class UFHS_AbilitySystemComponent;
class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;
class UInputMappingContext;
class UInputAction;

UCLASS(config=Game)
class HEROES_API AFHS_BaseHero : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AFHS_BaseHero();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GAS)
	TObjectPtr<UFHS_AbilitySystemComponent> ASC;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void SetupHero(TObjectPtr<UFHS_HeroData> HeroData);

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	void SetupGAS(TObjectPtr<UFHS_HeroData> HeroData);
	void SetupMeshes(TObjectPtr<UFHS_HeroData> HeroData);

#pragma region FPS_Template

public:
	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle = false;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

	/** Returns Mesh1P subobject **/
	TObjectPtr<USkeletalMeshComponent> GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	TObjectPtr<UCameraComponent> GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

private:
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	TObjectPtr<USkeletalMeshComponent> Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, Meta =(AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> MovementMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	void SetupConstructor();
	void SetupMovementIMC();
	void BindMovementActions(UInputComponent* PlayerInputComponent);

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

#pragma endregion // FPS_Template

};

