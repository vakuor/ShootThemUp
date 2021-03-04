// Shoot Them Up Game. All rights reserved!

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_PickupPickable.generated.h"


UCLASS()
class SHOOTTHEMUP_API UEnvQueryTest_PickupPickable : public UEnvQueryTest
{
	GENERATED_BODY()

public:
	UEnvQueryTest_PickupPickable(const FObjectInitializer& ObjectInitializer);
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};
