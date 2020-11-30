// ELP 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/Interface.h"
#include "OxiDestructibleActor.generated.h"


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
	virtual float TakeDamage_Implementation(const int DamageAmount, const AActor* DamageCauser);

private:
	virtual int TakeDamage_Internal(const int DamageAmount, const AActor* DamageCauser) = 0;
};

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

	UStaticMeshComponent* BaseMeshComponent;
	USkeletalMeshComponent* DestructibleMeshComponent;
};
