// ELP 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "OxiInterfaces.h"
#include "OxiCharacterComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OXI_API UOxiCharacterComponent : public USceneComponent, public IOxiDamageInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOxiCharacterComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	virtual void BeginPlay() override;

	// IOxiDamageInterface
protected:
	virtual float TakeDamage_Internal(const float DamageAmount, const AActor* DamageCauser) override { return 0.0f; }
};
