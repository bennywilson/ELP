// ELP 2020

#pragma once

#include "UObject/Interface.h"
#include "OxiInterfaces.generated.h"

UINTERFACE(BlueprintType)
class UOxiDamageInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IOxiDamageInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Oxi Damage")
	float TakeDamage(const float DamageAmount, const AActor* DamageCauser);
	
	virtual float TakeDamage_Implementation(const float DamageAmount, const AActor* DamageCauser);

private:
	virtual float TakeDamage_Internal(const float DamageAmount, const AActor* DamageCauser) = 0;
};
