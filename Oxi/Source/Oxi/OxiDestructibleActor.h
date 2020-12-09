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
	virtual int TakeDamage_Internal(const int DamageAmount, const AActor* DamageCauser) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Oxi Damage")
	int Health;

	UPROPERTY(EditAnywhere, Category = "Oxi Damage")
	float ExplosionImpulseMagnitude;

	UPROPERTY(EditAnywhere, Category = "Oxi Damage")
	float ExplosionXYImpulseMin;

	UPROPERTY(EditAnywhere, Category = "Oxi Damage")
	float ExplosionXYImpulseMax;

	UPROPERTY(EditAnywhere, Category = "Oxi Damage")
	float ExplosionAngularImpulseMin;

	UPROPERTY(EditAnywhere, Category = "Oxi Damage")
	float ExplosionAngularImpulseMax;

	UPROPERTY(EditAnywhere, Category = "Oxi Damage")
	class USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere, Category = "Oxi Damage")
	float SmearInitialPopDistance;

	UPROPERTY(EditAnywhere, Category = "Oxi Damage")
	float SmearLengthSec;

	UPROPERTY(EditAnywhere, Category = "Oxi Damage")
	float SmearStartStrength;

	UPROPERTY(EditAnywhere, Category = "Oxi Damage")
	float SmearStartTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Oxi Damage")
	class ULightComponent* ExplosionLightComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Oxi Damage")
	class UParticleSystemComponent* ExplosionParticleComponent;

	UPROPERTY(EditAnywhere, Category = "Oxi Damage")
	float ExplosionLightDurationSec;

	float ExplosionLightTargetIntensity;

	UStaticMeshComponent* BaseMeshComponent;
	USkeletalMeshComponent* DestructibleMeshComponent;
};
