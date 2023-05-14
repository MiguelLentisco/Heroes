#pragma once

#include "Heroes/Data/FHS_AbilityMeshData.h"
#include "FHS_HeroData.generated.h"

UCLASS()
class HEROES_API UFHS_HeroData : public UFHS_AbilityMeshData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Meshes)
	TSoftObjectPtr<USkeletalMesh> Hero1PMesh;
	
}; // UFHS_HeroData
