// Shoot Them Up Game. All rights reserved!

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "STUProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ASTUProjectile();

	void SetShotDirection(const FVector& Direction) { ShotDirection = Direction; }

protected:
	UPROPERTY(VisibleAnywhere, Category="Weapon")
	USphereComponent* CollisionComponent;
	
	UPROPERTY(VisibleAnywhere, Category="Weapon")
	UProjectileMovementComponent* MovementComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	float DamageRadius = 200.0f;
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	float DamageAmount = 50.0f;
	
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	//float ProjectileInitialSpeed = 2000.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	bool bDoFullDamage = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	float LifeSeconds = 5.0f;
	
	virtual void BeginPlay() override;
private:
	FVector ShotDirection;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );

	AController* GetController() const;
};
