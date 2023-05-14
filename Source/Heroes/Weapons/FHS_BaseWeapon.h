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
	AFHS_BaseWeapon();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

	void InitSpawnDeferred(AFHS_BaseHero* NewHeroOwner, UFHS_AbilityMeshData* NewData);
	void SetWeaponData(UFHS_AbilityMeshData* NewData);
	
	void SetupInput();
	void ClearInput();

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
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UFHS_AbilityMeshData> WeaponData;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AFHS_BaseHero> HeroOwner;

	bool bInputSet = false;

	void SetupWeapon();
	void AttachToHero();
	
}; // UFHS_BaseWeapon
