// ELP 2020

#pragma once

#include "OxiInterfaces.h"
#include "OxiDestructibleActor.generated.h"


UCLASS(config = Game)
class AOxiDestructibleActor : public AActor
{
	GENERATED_BODY()
	AOxiDestructibleActor();

protected:
	UPROPERTY(BlueprintReadWrite, Category = Mesh)
	class UOxiDestructibleComponent* DestructibleComponent;
};

UCLASS(Blueprintable, editinlinenew, meta = (BlueprintSpawnableComponent))
class OXI_API UOxiDestructibleComponent : public USceneComponent, public IOxiDamageInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UOxiDestructibleComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction);

	UFUNCTION(BlueprintCallable, Category = "Oxi Damage")
	bool InitDestructibleComponent(UStaticMeshComponent* InBaseMeshComponent, USkeletalMeshComponent* InDestructibleMeshComponent);

private:
	virtual float TakeDamage_Internal(const float DamageAmount, const AActor* DamageCauser) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Oxi Damage")
	float Health;

	UPROPERTY(EditAnywhere, Category = "Oxi Damage")
	float ExplosionSplashDamageRadius;

	UPROPERTY(EditAnywhere, Category = "Oxi Damage")
	float ExplosionSplashDamageAmount;

	UPROPERTY(EditAnywhere, Category = "Oxi Damage FX")
	float ExplosionImpulseMagnitude;

	UPROPERTY(EditAnywhere, Category = "Oxi Damage FX")
	float ExplosionXYImpulseMin;

	UPROPERTY(EditAnywhere, Category = "Oxi Damage FX")
	float ExplosionXYImpulseMax;

	UPROPERTY(EditAnywhere, Category = "Oxi Damage FX")
	float ExplosionAngularImpulseMin;

	UPROPERTY(EditAnywhere, Category = "Oxi Damage FX")
	float ExplosionAngularImpulseMax;

	UPROPERTY(EditAnywhere, Category = "Oxi Damage FX")
	class USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere, Category = "Oxi Damage FX")
	float SmearInitialPopDistance;

	UPROPERTY(EditAnywhere, Category = "Oxi Damage FX")
	float SmearLengthSec;

	UPROPERTY(EditAnywhere, Category = "Oxi Damage FX")
	float SmearStartStrength;

	UPROPERTY(EditAnywhere, Category = "Oxi Damage FX")
	float SmearStartTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Oxi Damage FX")
	class ULightComponent* ExplosionLightComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Oxi Damage FX")
	class UParticleSystemComponent* ExplosionParticleComponent;

	UPROPERTY(EditAnywhere, Category = "Oxi Damage FX")
	float ExplosionLightDurationSec;

	float ExplosionLightTargetIntensity;

	UStaticMeshComponent* BaseMeshComponent;
	USkeletalMeshComponent* DestructibleMeshComponent;
};
