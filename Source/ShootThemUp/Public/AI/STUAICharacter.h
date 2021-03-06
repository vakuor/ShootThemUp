// Shoot Them Up Game. All rights reserved!

#pragma once

#include "CoreMinimal.h"
#include "STUBaseCharacter.h"
#include "STUAICharacter.generated.h"

class UBehaviorTree;
class UWidgetComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUAICharacter : public ASTUBaseCharacter
{
	GENERATED_BODY()

public:
	ASTUAICharacter(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="AI")
	UBehaviorTree* BehaviorTreeAsset;

	virtual void Tick(float DeltaTime) override; //todo: здесь можно переделать не через тик, а через таймер отображение полоски

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	float HealthVisibilityDistance = 1000.0f;
	
	virtual void BeginPlay() override;
	
	virtual void OnDeath() override;
	virtual void OnHealthChanged(float Health, float HealthDelta) override;

private:
	void UpdateHealthWidgetVisibility();
};
