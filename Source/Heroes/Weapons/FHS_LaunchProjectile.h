#pragma once

#include "FHS_BaseProjectile.h"
#include "FHS_LaunchProjectile.generated.h"

UCLASS()
class HEROES_API AFHS_LaunchProjectile : public AFHS_BaseProjectile
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Explosion)
	TEnumAsByte<EObjectTypeQuery> ObjectTypeExplosion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Explosion)
	float LaunchSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Explosion)
	float ExplosionRadius = 800.f;
	
	virtual void ReactOnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                        FVector NormalImpulse, const FHitResult& Hit) override;
	
}; // FHS_LaunchProjectile
