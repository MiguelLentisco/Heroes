// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "FHS_BaseHero.generated.h"

class AFHS_BaseWeapon;
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

DECLARE_DELEGATE(FHeroChangedStatus);

UCLASS(config=Game)
class HEROES_API AFHS_BaseHero : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AFHS_BaseHero();

	virtual void BeginPlay() override;

	FHeroChangedStatus OnHeroReady;
	FHeroChangedStatus OnHeroCleared;

	TArray<AFHS_BaseWeapon*> GetWeapons() const { return Weapons; }
	void GetWeaponUSCs(TArray<UAbilitySystemComponent*>& WeaponUSCs) const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	void SetHeroData(UFHS_HeroData* NewHeroData);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GAS)
	TObjectPtr<UFHS_AbilitySystemComponent> ASC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	TObjectPtr<UFHS_HeroData> HeroData;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Data)
	TArray<TObjectPtr<AFHS_BaseWeapon>> Weapons;
	
	bool bInputSet = false;
	
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	void SetupGASInput();
	void ClearGASInput();
	
	void SetupHero();
	void SetupWeapon();

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

}; // AFHS_BaseHero

