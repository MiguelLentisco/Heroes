// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "GameFramework/Actor.h"
#include "FHS_BaseProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UAbilitySystemComponent;

UCLASS()
class HEROES_API AFHS_BaseProjectile : public AActor
{
	GENERATED_BODY()

public:
	AFHS_BaseProjectile();
	
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	void SetInstigatorUSC(UAbilitySystemComponent* WeaponUSC) { InstigatorUSC = WeaponUSC; }

protected:
	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FScalableFloat DamageValue = -10.f;

	UPROPERTY()
	TWeakObjectPtr<UAbilitySystemComponent> InstigatorUSC;

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	           const FHitResult& Hit);

}; // AFHS_BaseProjectile

