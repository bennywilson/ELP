// ELP 2020

#include "OxiSentryCharacter.h"
#include "OxiSentryCharAnimInstance.h"

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

			GetWorld()->GetTimerManager().SetTimer(DeleteTimer, this, &UOxiSentryCharacter::LifeSpanCallback, 5.0f, false);

		}
		else
		{
			UOxiSentryCharAnimInstance* const AnimInstance = Cast<UOxiSentryCharAnimInstance>(SkelMesh->GetAnimInstance());
			if (AnimInstance != nullptr)
			{
				AnimInstance->PlayHitReaction(DamageAmount, DamageCauser);
			}

		//	ECollisionChannel CollisionChannel = UEngineTypes::ConvertToCollisionChannel(TraceChannel);

	//		static const FName LineTraceSingleName(TEXT("LineTraceSingle"));
//			FCollisionQueryParams Params = ConfigureCollisionParams(LineTraceSingleName, bTraceComplex, ActorsToIgnore, bIgnoreSelf, WorldContextObject);
		//	FHitResult HitResult;
			//bool bHit = GetWorld()->LineTraceSingleByObjectType(Result, Start, End, FCollisionObjectQueryParams(ECC_WorldStatic), FCollisionQueryParams(NAME_None, FCollisionQueryParams::GetUnknownStatId(), true));

		//	const bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + FVector(10000.0f, 0.0f, 0.0f), FCollisionObjectQueryParams(ECC_WorldStatic));
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