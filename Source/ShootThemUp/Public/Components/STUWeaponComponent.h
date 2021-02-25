// Shoot Them Up Game. All rights reserved!

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUWeaponComponent.generated.h"

class ASTUBaseWeapon;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USTUWeaponComponent();

	void StartFire();
	void StopFire();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	TSubclassOf<ASTUBaseWeapon> WeaponClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FName WeaponAttachPointName = "WeaponSocket";
	
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	ASTUBaseWeapon* CurrentWeapon = nullptr;
	
	void SpawnWeapon();
};
