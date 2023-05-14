#pragma once

#include "Heroes/Data/FHS_AbilityMeshData.h"
#include "FHS_HeroData.generated.h"

class AFHS_BaseWeapon;

UCLASS()
class HEROES_API UFHS_HeroData : public UFHS_AbilityMeshData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	TSoftObjectPtr<USkeletalMesh> Hero1PMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TSoftClassPtr<AFHS_BaseWeapon> WeaponClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TSoftObjectPtr<UFHS_AbilityMeshData> WeaponData;
	
}; // UFHS_HeroData
