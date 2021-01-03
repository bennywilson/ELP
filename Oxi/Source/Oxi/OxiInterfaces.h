// ELP 2020

#pragma once

#include "UObject/Interface.h"
#include "OxiInterfaces.generated.h"

USTRUCT(BlueprintType)
struct FOxiDamageInfo
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DamageLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* DamageCauser;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName HitBoneName;
};

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
	float TakeDamage(const FOxiDamageInfo& DamageInfo);
	
	virtual float TakeDamage_Implementation(const FOxiDamageInfo& DamageInfo);

private:
	virtual float TakeDamage_Internal(const FOxiDamageInfo&) = 0;
};
