#pragma once

#include "FHS_BaseWeapon.generated.h"

class UFHS_AbilityMeshData;
class UFHS_AbilitySystemComponent;
class AFHS_BaseHero;

UCLASS()
class AFHS_BaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	/** Sets default values for this component's properties */
	AFHS_BaseWeapon();
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void SetWeaponData(UFHS_AbilityMeshData* NewData);
	void AttachToHero(AFHS_BaseHero* Hero);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UFHS_AbilitySystemComponent> ASC;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TObjectPtr<USoundBase> FireSound;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TObjectPtr<UAnimMontage> FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AFHS_BaseHero> HeroOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UFHS_AbilityMeshData> WeaponData;
	
	void SetupInput();
	
}; // UFHS_BaseWeapon
