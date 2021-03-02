// Shoot Them Up Game. All rights reserved!

#pragma once

#include "CoreMinimal.h"

#include "STUCoreTypes.h"
#include "Components/ActorComponent.h"
#include "STUWeaponFXComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUWeaponFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USTUWeaponFXComponent();

	void PlayImpactFX(const FHitResult& Hit);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FX")
	FImpactData DefaultImpactData;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FX")
	// ReSharper disable once UnrealHeaderToolParserError
	TMap<UPhysicalMaterial*, FImpactData> ImpactDataMap;

};
