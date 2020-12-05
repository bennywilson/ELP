// ELP 2020

#include "OxiDestructibleActor.h"
#include "Components\LightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles\ParticleSystemComponent.h"

UOxiDamageInterface::UOxiDamageInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

float IOxiDamageInterface::TakeDamage_Implementation(const int DamageAmount, const AActor* DamageCauser)
{
	return TakeDamage_Internal(DamageAmount, DamageCauser);
}

AOxiDestructibleActor::AOxiDestructibleActor()
{
	DestructibleComponent = CreateDefaultSubobject<UOxiDestructibleComponent>(TEXT("FirstPersonCamera"));
}

UOxiDestructibleComponent::UOxiDestructibleComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UOxiDestructibleComponent::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

bool UOxiDestructibleComponent::InitDestructibleComponent(UStaticMeshComponent* InBaseMeshComponent, USkeletalMeshComponent* InDestructibleMeshComponent)
{
	BaseMeshComponent = InBaseMeshComponent;
	DestructibleMeshComponent = InDestructibleMeshComponent;

	if (BaseMeshComponent != nullptr)
	{
		BaseMeshComponent->SetHiddenInGame(false, true);
	}

	if (DestructibleMeshComponent != nullptr)
	{
		DestructibleMeshComponent->SetHiddenInGame(true, true);
		DestructibleMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	return true;
}

void UOxiDestructibleComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (DestructibleMeshComponent != nullptr && DestructibleMeshComponent->IsVisible())
	{
		UMaterialInstanceDynamic* const Mat = Cast<UMaterialInstanceDynamic>(DestructibleMeshComponent->GetMaterial(1));		// TODO: Unhardcode this
		if (Mat)
		{
			const float t = FMath::Clamp((GetWorld()->GetUnpausedTimeSeconds() - SmearStartTime) / SmearLengthSec, 0.0f, 1.0f);
			Mat->SetScalarParameterValue("SmearStrength", SmearStartStrength * (1.0f - t));
		}

		if (ExplosionLightComponent != nullptr)
		{
			const float t = FMath::Clamp((GetWorld()->GetUnpausedTimeSeconds() - SmearStartTime) / ExplosionLightDurationSec, 0.0f, 1.0f);
			const float lightMultiplier = 1.0f - t;	// Goes from off, on, off
			ExplosionLightComponent->SetIntensity(lightMultiplier * ExplosionLightTargetIntensity);
		}
	}
}

int UOxiDestructibleComponent::TakeDamage_Internal(const int DamageAmount, const AActor* DamageCauser)
{
	if (Health > 0.0f && Health - DamageAmount <= 0.0f)
	{
		BaseMeshComponent->SetHiddenInGame(true);
		BaseMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (ExplosionSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetComponentTransform().GetLocation());
		}
		if (DestructibleMeshComponent != nullptr)
		{
			SmearStartTime = GetWorld()->GetUnpausedTimeSeconds();
			DestructibleMeshComponent->SetHiddenInGame(false);
			DestructibleMeshComponent->SetSimulatePhysics(true);
			DestructibleMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
			
			FVector ExplosionLocation = DestructibleMeshComponent->GetSocketLocation("ExplosionLocation");

			for (FBodyInstance* BI : DestructibleMeshComponent->Bodies)
			{
				const float XYImpulse = FMath::RandRange(ExplosionXYImpulseMin, ExplosionXYImpulseMax);
				FVector XAmount = XYImpulse * DestructibleMeshComponent->GetComponentTransform().TransformFVector4(FVector4(1.0f, 0.0f, 0.0f, 1.0f));
				FVector YAmount = XYImpulse * DestructibleMeshComponent->GetComponentTransform().TransformFVector4(FVector4(0.0f, 1.0f, 0.0f));

				const FVector ImpulseDir = (BI->GetCOMPosition() - ExplosionLocation).GetSafeNormal() * ExplosionImpulseMagnitude;
				const FVector FinalImpulse = ImpulseDir + XAmount + YAmount;

				FTransform BodyTransform = BI->GetUnrealWorldTransform();
				BodyTransform.SetLocation(BodyTransform.GetLocation() + FinalImpulse.GetSafeNormal() * SmearInitialPopDistance);
				BI->SetBodyTransform(BodyTransform, ETeleportType::None);

				const FName BoneName = DestructibleMeshComponent->GetBoneName(BI->InstanceBoneIndex);
				DestructibleMeshComponent->AddImpulse(FinalImpulse, BoneName, true);
				
				FVector RotationAxis(FMath::RandRange(-1.0f, 1.0f), FMath::RandRange(-1.0f, 1.0f), FMath::RandRange(-1.0f, 1.0f));
				if (RotationAxis.SizeSquared() < 0.01f)
				{
					RotationAxis.Set(1.0f, 0.0f, 0.0f);
				}
				RotationAxis.Normalize();
				RotationAxis = RotationAxis * FMath::RandRange(ExplosionAngularImpulseMin, ExplosionAngularImpulseMax);
				DestructibleMeshComponent->AddAngularImpulseInRadians(RotationAxis, BoneName, true);
			}

			if (ExplosionParticleComponent != nullptr)
			{
				ExplosionParticleComponent->ResetParticles(true);
				ExplosionParticleComponent->SetVisibility(true);
 				ExplosionParticleComponent->ActivateSystem();
			}
		}

		if (ExplosionLightComponent != nullptr)
		{
			ExplosionLightComponent->SetVisibility(true);
			ExplosionLightTargetIntensity = ExplosionLightComponent->Intensity;
			ExplosionLightComponent->SetIntensity(0.0f);
		}
	}

	Health -= DamageAmount;
	return 1.0f;
}