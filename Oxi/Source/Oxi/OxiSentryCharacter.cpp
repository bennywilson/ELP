// ELP 2020

#include "OxiSentryCharacter.h"
#include "OxiSentryCharAnimInstance.h"
#include "Kismet/GameplayStatics.h"

void UOxiSentryCharacter::BeginPlay()
{
	Super::BeginPlay();

	TArray<USceneComponent*> Children;
	GetChildrenComponents(true, Children);

	for (int i = 0; i < Children.Num(); i++)
	{
		USkeletalMeshComponent* const SkelMesh = Cast<USkeletalMeshComponent>(Children[i]);
		if (SkelMesh == nullptr)
		{
			continue;
		}


SkelMesh->SetSimulatePhysics(false);
		SkelMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SkelMesh->SetAllBodiesPhysicsBlendWeight(0.f);
	}

	LastClipTime = -999999.0f;
}


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

	TArray<USceneComponent*> Children;
	GetChildrenComponents(true, Children);

	for (int i = 0; i < Children.Num(); i++)
	{
		USkeletalMeshComponent* const SkelMesh = Cast<USkeletalMeshComponent>(Children[i]);
		if (SkelMesh == nullptr)
		{
			continue;
		}

		for (int MatIdx = 0; MatIdx < SkelMesh->GetNumMaterials(); MatIdx++)
		{
			UMaterialInstanceDynamic* const DynMat = Cast<UMaterialInstanceDynamic>(SkelMesh->GetMaterial(MatIdx));
			if (DynMat == nullptr)
			{
				continue;
			}

			static FName ClipBoneParams[] = { "ClipBone1Params", "ClipBone2Params", "ClipBone3Params" };
			for (int ClipBoneIdx = 0; ClipBoneIdx < ClippedBones.Num(); ClipBoneIdx++)
			{
				FLinearColor ClipBoneParam;
				ClipBoneParam.R = ClippedBones[ClipBoneIdx].HitLocation.X;
				ClipBoneParam.G = ClippedBones[ClipBoneIdx].HitLocation.Y;
				ClipBoneParam.B = ClippedBones[ClipBoneIdx].HitLocation.Z;
				ClipBoneParam.A = 5.0f;
				DynMat->SetVectorParameterValue(ClipBoneParams[ClipBoneIdx], ClipBoneParam);
			}
		}
	}

}


float UOxiSentryCharacter::TakeDamage_Internal(const FOxiDamageInfo& DamageInfo)
{
	Super::TakeDamage_Internal(DamageInfo);
	
	float DamageAmount = DamageInfo.DamageAmount;
	bool KillShot = true;

	TArray<USceneComponent*> Children;
	GetChildrenComponents(true, Children);

	for (int i = 0; i < Children.Num(); i++)
	{
		USkeletalMeshComponent* const SkelMesh = Cast<USkeletalMeshComponent>(Children[i]);
		if (SkelMesh == nullptr)
		{
			continue;
		}

		static FName RootBone("Bone");

		bool bCanClipBone = DamageInfo.HitBoneName != RootBone;
		bool bBoneClipped = false;
		if (bCanClipBone)
		{
			for (int ClipBoneIdx = 0; ClipBoneIdx < ClippedBones.Num(); ClipBoneIdx++)
			{
				if (SkelMesh->BoneIsChildOf(ClippedBones[ClipBoneIdx].BoneName, DamageInfo.HitBoneName))
				{
					bCanClipBone = false;
					break;
				}
			}
		}

		if (bCanClipBone && ClippedBones.Num() < 3 && GetWorld()->GetUnpausedTimeSeconds() > LastClipTime + 0.5f)
		{
			LastClipTime = GetWorld()->GetUnpausedTimeSeconds();
			const int32 HitBoneIdx = SkelMesh->GetBoneIndex(DamageInfo.HitBoneName);

			if (HitBoneIdx != INDEX_NONE)
			{
				//const FVector RefBonePos = SkelMesh->GetRefPosePosition(HitBoneIdx);
				FHitBoneInfo HitInfo;
				HitInfo.HitLocation = SkelMesh->GetBoneLocation(DamageInfo.HitBoneName, EBoneSpaces::ComponentSpace);
				HitInfo.BoneName = DamageInfo.HitBoneName;
				ClippedBones.Add(HitInfo);

				DamageAmount *= 0.5f;
				SkelMesh->SetAllBodiesBelowSimulatePhysics(DamageInfo.HitBoneName, true);
				SkelMesh->SetAllBodiesBelowPhysicsBlendWeight(DamageInfo.HitBoneName, 1.0f);
				// Break the constraint
				int32 ConstraintIndex = SkelMesh->FindConstraintIndex(DamageInfo.HitBoneName);
				if (ConstraintIndex != INDEX_NONE && ConstraintIndex < SkelMesh->Constraints.Num())
				{
					FConstraintInstance* Constraint = SkelMesh->Constraints[ConstraintIndex];
					// If already broken, our job has already been done. Bail!
					if (Constraint->IsTerminated() == false)
					{
						UPhysicsAsset* const PhysicsAsset = SkelMesh->GetPhysicsAsset();
						FBodyInstance* Body = SkelMesh->GetBodyInstance(Constraint->JointName);

						if (Body != NULL && !Body->IsInstanceSimulatingPhysics())
						{
							Body->SetInstanceSimulatePhysics(true);
						}

						// Break Constraint
						Constraint->TermConstraint();
						bBoneClipped = true;
					}
				}
				// Make sure child bodies and constraints are released and turned to physics.
				//UpdateMeshForBrokenConstraints();
				// Add impulse to broken limb

				FVector Impulse = (DamageInfo.DamageLocation - DamageInfo.DamageCauser->GetActorLocation()).GetSafeNormal();
				static float Scalar = 1000.0f;
				Impulse *= Scalar;
				SkelMesh->AddImpulse(Impulse, DamageInfo.HitBoneName, true);
				//SkelMesh->BreakConstraint(FVector::ZeroVector, DamageInfo.DamageLocation, DamageInfo.HitBoneName);
			}

			for (int MatIdx = 0; MatIdx < SkelMesh->GetNumMaterials(); MatIdx++)
			{
				UMaterialInstanceDynamic* const DynMat = Cast<UMaterialInstanceDynamic>(SkelMesh->GetMaterial(MatIdx));
				if (DynMat == nullptr)
				{
					continue;
				}

				static FName ClipBoneParams[] = {"ClipBone1Params", "ClipBone2Params", "ClipBone3Params"};
				for (int ClipBoneIdx = 0; ClipBoneIdx < ClippedBones.Num(); ClipBoneIdx++)
				{
					FLinearColor ClipBoneParam;
					ClipBoneParam.R = ClippedBones[ClipBoneIdx].HitLocation.X;
					ClipBoneParam.G = ClippedBones[ClipBoneIdx].HitLocation.Y;
					ClipBoneParam.B = ClippedBones[ClipBoneIdx].HitLocation.Z;
					ClipBoneParam.A = 5.0f;
					DynMat->SetVectorParameterValue(ClipBoneParams[ClipBoneIdx], ClipBoneParam);
				}
			}
		}

		if (BaseHealth > 1 || (DamageInfo.HitBoneName != NAME_None && DamageInfo.HitBoneName != RootBone))
		{
			KillShot = false;
		}

		if (BaseHealth <= 0)
		{
			return 0.f;
		}

		const bool JustKilled = KillShot && (BaseHealth > 0 && (BaseHealth - DamageInfo.DamageAmount) <= 0.f);

		BaseHealth -= DamageAmount;

		if (BaseHealth <= 0 && KillShot == false)
		{
			BaseHealth = 1.0f;
		}

		UOxiSentryCharAnimInstance* const AnimInstance = Cast<UOxiSentryCharAnimInstance>(SkelMesh->GetAnimInstance());
		if (JustKilled)
		{
			SkelMesh->SetSimulatePhysics(true);
			SkelMesh->SetAllBodiesPhysicsBlendWeight(1.f);

			GetWorld()->GetTimerManager().SetTimer(DeleteTimer, this, &UOxiSentryCharacter::LifeSpanCallback, 5.0f, false);
			DeathStartTime = UGameplayStatics::GetTimeSeconds(GetWorld());

			if (AnimInstance != nullptr)
			{
				AnimInstance->PlayHitReaction(DamageInfo.DamageAmount, DamageInfo.DamageLocation, DamageInfo.DamageCauser, false, false);
				AnimInstance->PlayDeathReaction(DamageInfo.DamageAmount, DamageInfo.DamageLocation, DamageInfo.DamageCauser);
			}

			FVector Impulse = (GetComponentTransform().GetLocation() - DamageInfo.DamageLocation).GetSafeNormal();
			static float Scalar = 1000.0f;
			Impulse *= Scalar;
			SkelMesh->AddImpulse(Impulse, NAME_None, true);
		}
		else
		{
			if (AnimInstance != nullptr)
			{
				AnimInstance->PlayHitReaction(DamageInfo.DamageAmount, DamageInfo.DamageLocation, DamageInfo.DamageCauser, bCanClipBone, bBoneClipped);
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