// ELP 2020

#pragma once

#include "CoreMinimal.h"
#include "OxiCharacterComponent.h"
#include "OxiInterfaces.h"
#include "OxiSentryCharacter.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, editinlinenew, meta = (BlueprintSpawnableComponent))
class OXI_API UOxiSentryCharacter : public UOxiCharacterComponent
{
	GENERATED_BODY()

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction);

protected:

	void LifeSpanCallback();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	float BaseHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FX)
	TArray<TSubclassOf<AActor>> BloodDecals;

	FTimerHandle DeleteTimer;

	// IOxiDamageInterface
protected:
	virtual float TakeDamage_Internal(const float DamageAmount, const FVector DamageLocation, const AActor* DamageCauser) override;
};
