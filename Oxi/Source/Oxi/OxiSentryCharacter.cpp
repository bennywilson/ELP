// ELP 2020

#include "OxiSentryCharacter.h"
#include "OxiSentryCharAnimInstance.h"
#include "Kismet/GameplayStatics.h"

void UOxiSentryCharacter::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (APlayerController* PC = Iterator->Get())
		{
			FVector VecTo = PC->GetPawn()->GetActorLocation() - GetOwner()->GetActorLocation();
			VecTo.Normalize();
			GetOwner()->SetActorRotation(VecTo.Rotation());
		}
	}
}


float UOxiSentryCharacter::TakeDamage_Internal(const float DamageAmount, const FVector DamageLocation, const AActor* DamageCauser)
{
	Super::TakeDamage_Internal(DamageAmount, DamageLocation, DamageCauser);

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

		UOxiSentryCharAnimInstance* const AnimInstance = Cast<UOxiSentryCharAnimInstance>(SkelMesh->GetAnimInstance());
		if (JustKilled)
		{
			SkelMesh->SetSimulatePhysics(true);
			SkelMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

			GetWorld()->GetTimerManager().SetTimer(DeleteTimer, this, &UOxiSentryCharacter::LifeSpanCallback, 5.0f, false);
			DeathStartTime = UGameplayStatics::GetTimeSeconds(GetWorld());

			if (AnimInstance != nullptr)
			{
				AnimInstance->PlayDeathReaction(DamageAmount, DamageLocation, DamageCauser);
			}
		}
		else
		{
			if (AnimInstance != nullptr)
			{
				AnimInstance->PlayHitReaction(DamageAmount, DamageLocation, DamageCauser);
			}
		}
	}
	return 0.f;
}

void UOxiSentryCharacter::LifeSpanCallback()
{
	DestroyComponent();
	if (AActor* Owner = GetOwner())
	{
		Owner->Destroy();
	}
}