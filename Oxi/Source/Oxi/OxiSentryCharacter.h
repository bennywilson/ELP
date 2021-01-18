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
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction);

protected:

	void LifeSpanCallback();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	float BaseHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FX)
	TArray<TSubclassOf<AActor>> BloodDecals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FX)
	TArray<TSubclassOf<AActor>> BloodSprayFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FX)
	TArray<TSubclassOf<AActor>> GibList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FX)
	float GibChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FX)
	float DeathDurationSec;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=FX)
	float DeathStartTime;

	FTimerHandle DeleteTimer;

	struct FHitBoneInfo
	{
		FName BoneName;
		FVector HitLocation;
		float HitTime;
	};
	TArray<FHitBoneInfo> ClippedBones;
	
	UPROPERTY(Transient)
	float LastClipTime;

	// IOxiDamageInterface
protected:
	virtual float TakeDamage_Internal(const FOxiDamageInfo& DamageInfo) override;
};
