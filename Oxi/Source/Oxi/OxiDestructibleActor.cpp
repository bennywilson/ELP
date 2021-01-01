// ELP 2020

#include "OxiDestructibleActor.h"
#include "Components\LightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles\ParticleSystemComponent.h"

UOxiDamageInterface::UOxiDamageInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

float IOxiDamageInterface::TakeDamage_Implementation(const float DamageAmount, const FVector DamageLocation, const AActor* DamageCauser)
{
	return TakeDamage_Internal(DamageAmount, DamageLocation, DamageCauser);
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

float UOxiDestructibleComponent::TakeDamage_Internal(const float DamageAmount, const FVector DamageLocation, const AActor* DamageCauser)
{
	if (Health > 0.0f && Health - DamageAmount <= 0.0f)
	{
		BaseMeshComponent->SetHiddenInGame(true);
		BaseMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BaseMeshComponent->SetGenerateOverlapEvents(false);

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
			
			const FVector ExplosionLocation = DestructibleMeshComponent->GetSocketLocation("ExplosionLocation");

			for (FBodyInstance* BI : DestructibleMeshComponent->Bodies)
			{
				const float XYImpulse = FMath::RandRange(ExplosionXYImpulseMin, ExplosionXYImpulseMax);
				const FVector XAmount = XYImpulse * DestructibleMeshComponent->GetComponentTransform().TransformFVector4(FVector4(1.0f, 0.0f, 0.0f, 1.0f));
				const FVector YAmount = XYImpulse * DestructibleMeshComponent->GetComponentTransform().TransformFVector4(FVector4(0.0f, 1.0f, 0.0f));

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
		}

		if (ExplosionParticleComponent != nullptr)
		{
			ExplosionParticleComponent->ResetParticles(true);
			ExplosionParticleComponent->SetVisibility(true);
			ExplosionParticleComponent->ActivateSystem();
		}

		if (ExplosionLightComponent != nullptr)
		{
			ExplosionLightComponent->SetVisibility(true);
			ExplosionLightTargetIntensity = ExplosionLightComponent->Intensity;
			ExplosionLightComponent->SetIntensity(0.0f);
		}

		if (ExplosionSplashDamageRadius > 0 && ExplosionSplashDamageAmount > 0)
		{
			TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
			TArray<AActor*> ActorsToIgnore;
			TArray<class UPrimitiveComponent*> OutComponents;
			ActorsToIgnore.Add(GetOwner());
			UKismetSystemLibrary::SphereOverlapComponents(this, GetOwner()->GetActorLocation(), ExplosionSplashDamageRadius, ObjectTypes, nullptr, ActorsToIgnore, OutComponents);

			const FVector OwnerLocation = GetOwner()->GetActorLocation();
			for (int i = 0; i < OutComponents.Num(); i++)
			{
				UPrimitiveComponent* CurComp = OutComponents[i];
				AActor* const CurOwner = CurComp->GetOwner();
				if (CurOwner->GetClass()->ImplementsInterface(UOxiDamageInterface::StaticClass()))
				{
					IOxiDamageInterface* const OwnerDamageInterface = Cast<IOxiDamageInterface>(CurOwner);
					OwnerDamageInterface->Execute_TakeDamage(CurOwner, ExplosionSplashDamageAmount, OwnerLocation, GetOwner());
				}
				auto DamageCompList = CurComp->GetOwner()->GetComponentsByInterface(UOxiDamageInterface::StaticClass());
				for (int iDamage = 0; iDamage < DamageCompList.Num(); iDamage++)
				{
					IOxiDamageInterface* const DamageInterface = Cast<IOxiDamageInterface>(DamageCompList[iDamage]);
					DamageInterface->Execute_TakeDamage(DamageCompList[iDamage], ExplosionSplashDamageAmount, OwnerLocation, GetOwner());
				}
			}
		}
	}

	Health -= DamageAmount;
	return 1.0f;
}