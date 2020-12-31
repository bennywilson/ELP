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
	
	// IOxiDamageInterface
protected:
	virtual float TakeDamage_Internal(const float DamageAmount, const AActor* DamageCauser) override;
};
