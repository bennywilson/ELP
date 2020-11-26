// ELP 2020

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "OxiWeaponAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class OXI_API UOxiWeaponAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Animations")
	bool StartFireWeapon();

	UFUNCTION(BlueprintImplementableEvent, Category = "Animations")
	bool StopFireWeapon();
};
