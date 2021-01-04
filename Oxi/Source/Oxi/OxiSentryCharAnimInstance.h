// ELP 2020

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "OxiSentryCharAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class OXI_API UOxiSentryCharAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void PlayHitReaction(const float DamageAmount, const FVector DamageLocation, const AActor* DamageCauser, const bool bHitTentacle, const bool bDismemberedTentacle);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void PlayDeathReaction(const float DamageAmount, const FVector DamageLocation, const AActor* DamageCauser);
};
