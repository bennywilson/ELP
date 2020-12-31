// ELP 2020

#include "OxiSentryCharacter.h"

float UOxiSentryCharacter::TakeDamage_Internal(const float DamageAmount, const AActor* DamageCauser)
{
	Super::TakeDamage_Internal(DamageAmount, DamageCauser);

	TArray<USceneComponent*> Children;
	GetChildrenComponents(true, Children);

	for (int i = 0; i < Children.Num(); i++)
	{
		USkeletalMeshComponent* const SkelMesh = Cast<USkeletalMeshComponent>(Children[i]);
		if (SkelMesh == nullptr)
		{
			continue;
		}

		SkelMesh->SetSimulatePhysics(true);
		SkelMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}
	return 0.f;
}