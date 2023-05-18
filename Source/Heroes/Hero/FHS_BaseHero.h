// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "FHS_BaseHero.generated.h"

class USpringArmComponent;
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
struct FOnAttributeChangeData;
struct FGameplayTag;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMainWeaponChanged, AFHS_BaseWeapon*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHeroInputChanged, bool);

UCLASS()
class HEROES_API AFHS_BaseHero : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AFHS_BaseHero();

	FOnHeroInputChanged OnHeroInputChanged;
	FOnMainWeaponChanged OnMainWeaponChanged;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UFHS_AbilitySystemComponent* GetFHSAbilitySystemComponent() const { return ASC; }
	UCameraComponent* GetCamera3P() const { return Camera3P; }
	AFHS_BaseWeapon* GetCurrentWeapon() const { return CurrentWeapon; }
	bool IsInputSet() const { return bInputSet; }
	bool IsInputBlocked() const;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	virtual void BeginPlay() override;
	
	void SpeedUpdated(const FOnAttributeChangeData& AttributeData);
	void HealthUpdated(const FOnAttributeChangeData& AttributeData);

	void KillHero();
	
	UFUNCTION(Server, Reliable)
	void SetHeroData(UFHS_HeroData* NewHeroData);

	void OnPlayerDeadChanged(const FGameplayTag DeadTag, int32 NumCount);
	void OnPlayerStunChanged(const FGameplayTag StunTag, int32 NumCount);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GAS)
	TObjectPtr<UFHS_AbilitySystemComponent> ASC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<USpringArmComponent> CameraSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<UCameraComponent> Camera3P;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Hero, ReplicatedUsing = "OnRep_HeroData")
	TObjectPtr<UFHS_HeroData> HeroData;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Hero, ReplicatedUsing = "OnRep_CurrentWeapon")
	TObjectPtr<AFHS_BaseWeapon> CurrentWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hero)
	TArray<TObjectPtr<AFHS_BaseWeapon>> Weapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hero)
	FName Hand3PSocket = TEXT("middle_01_rSocket");

	UPROPERTY(Replicated, VisibleAnywhere, ReplicatedUsing = "OnRep_bDead")
	bool bDead = false;

	FTransform MeshOriginalTransform;

	bool bInputSet = false;
	bool bStunned = false;

	UFUNCTION()
	void OnRep_bDead();
	UFUNCTION()
	void OnRep_HeroData(UFHS_HeroData* PreviousData);
	UFUNCTION()
	void OnRep_CurrentWeapon();

	void InitStats();
	void SetupGAS();
	void SetupWeapons();
	void SetupMeshes();
	void SetupGASInput(bool bSetupInput);

#pragma region FPS_Template

public:
	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle() const;

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

