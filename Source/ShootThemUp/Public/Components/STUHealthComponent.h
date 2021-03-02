// Shoot Them Up Game. All rights reserved!

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUHealthComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USTUHealthComponent();

	FOnDeathSignature OnDeath;
	FOnHealthChangedSignature OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category="Health")
	bool IsDead() const { return FMath::IsNearlyZero(Health); }

	UFUNCTION(BlueprintCallable, Category="Health")
	float GetHealthPercent() const { return GetHealth() / MaxHealth; }

	float GetHealth() const { return Health; }

	bool TryToAddHealth(float HealthAmount);
	bool IsHealthFull() const;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Health", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Heal")
	bool bAutoHeal = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Heal", meta = (EditCondition = "bAutoHeal", ClampMin = "0.0"))
	float HealUpdateTime = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Heal", meta = (EditCondition = "bAutoHeal", ClampMin = "0.0"))
	float HealDelay = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Heal", meta = (EditCondition = "bAutoHeal", ClampMin = "0.0"))
	float HealModifier = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="VFX")
	TSubclassOf<UCameraShakeBase> CameraShake;

	virtual void BeginPlay() override;
private:
	float Health = 0.0f;
	FTimerHandle HealTimerHandle;

	UFUNCTION()
	void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	                     class AController* InstigatedBy, AActor* DamageCauser);

	void HealUpdate();
	void SetHealth(float NewHealth);

	void PlayCameraShake();
};
