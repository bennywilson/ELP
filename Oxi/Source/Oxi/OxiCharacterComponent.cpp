// ELP 2020

#include "OxiCharacterComponent.h"

UOxiCharacterComponent::UOxiCharacterComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOxiCharacterComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void UOxiCharacterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
