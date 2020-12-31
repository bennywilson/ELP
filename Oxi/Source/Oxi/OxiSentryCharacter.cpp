// ELP 2020

#include "OxiSentryCharacter.h"
#include "OxiSentryCharAnimInstance.h"

float UOxiSentryCharacter::TakeDamage_Internal(const float DamageAmount, const AActor* DamageCauser)
{
	Super::TakeDamage_Internal(DamageAmount, DamageCauser);

	const bool JustKilled = (BaseHealth > 0 && (BaseHealth - DamageAmount) <= 0.f);
	BaseHealth -= DamageAmount;

	TArray<USceneComponent*> Children;
	GetChildrenComponents(true, Children);

	for (int i = 0; i < Children.Num(); i++)
	{
		USkeletalMeshComponent* const SkelMesh = Cast<USkeletalMeshComponent>(Children[i]);
		if (SkelMesh == nullptr)
		{
			continue;
		}

		if (JustKilled)
		{
			SkelMesh->SetSimulatePhysics(true);
			SkelMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		}
		else
		{
			UOxiSentryCharAnimInstance* const AnimInstance = Cast<UOxiSentryCharAnimInstance>(SkelMesh->GetAnimInstance());
			if (AnimInstance != nullptr)
			{
				AnimInstance->PlayHitReaction(DamageAmount, DamageCauser);
			}
		}
	}
	return 0.f;
}