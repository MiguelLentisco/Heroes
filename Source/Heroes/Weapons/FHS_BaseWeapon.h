#pragma once
#include "AbilitySystemInterface.h"

#include "FHS_BaseWeapon.generated.h"

class UFHS_AbilityMeshData;
class UFHS_AbilitySystemComponent;
class AFHS_BaseHero;
class AFHS_BaseProjectile;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeaponInputChanged, bool);

UCLASS()
class HEROES_API AFHS_BaseWeapon : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AFHS_BaseWeapon();

	FOnWeaponInputChanged OnWeaponInputChanged;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	bool IsInputSet() const { return bInputSet; }
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Do not call from clients
	void SetHeroOwner(AFHS_BaseHero* NewHero);
	void SetWeaponData(UFHS_AbilityMeshData* NewData);
	void SetMainWeapon(bool bNewMainWeapon);
	
	void SetupInput(bool bWantsToSet);

	void PrimaryFire(const TSubclassOf<AFHS_BaseProjectile>& ProjectileClass, const FVector& MuzzleOffset);
	UFUNCTION(NetMulticast, Unreliable)
	void PlayFireMontage(UAnimMontage* ShootAnim);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UFHS_AbilitySystemComponent> ASC;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, ReplicatedUsing = "OnRep_WeaponData")
	TObjectPtr<UFHS_AbilityMeshData> WeaponData;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = "OnRep_HeroOwner")
	TObjectPtr<AFHS_BaseHero> HeroOwner;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = "OnRep_bMainWeapon")
	bool bMainWeapon = false;

	bool bInputSet = false;

	UFUNCTION()
	void OnRep_WeaponData(UFHS_AbilityMeshData* OldWeaponData);
	UFUNCTION()
	void OnRep_HeroOwner();
	UFUNCTION()
	void OnRep_bMainWeapon();

	void SetupWeaponMesh();
	void AttachToHero();
	
}; // UFHS_BaseWeapon
