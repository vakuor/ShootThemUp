// Shoot Them Up Game. All rights reserved!

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "STUAIController.generated.h"

class USTUAIPerceptionComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUAIController : public AAIController
{
	GENERATED_BODY()

public:
	ASTUAIController();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USTUAIPerceptionComponent* STUAIPerceptionComponent;

	virtual void OnPossess(APawn* InPawn) override;
};
