// Fill out your copyright notice in the Description page of Project Settings.

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
	void PlayHitReaction(const float DamageAmount, const FVector DamageLocation, const AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void PlayDeathReaction(const float DamageAmount, const FVector DamageLocation, const AActor* DamageCauser);
};
