// ELP 2020

#include "OxiDestructibleActor.h"

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
	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	/*BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMeshComponent"));
	BaseMeshComponent->SetOnlyOwnerSee(false);
	BaseMeshComponent->SetupAttachment(this);
	BaseMeshComponent->bCastDynamicShadow = true;
	BaseMeshComponent->CastShadow = true;
	BaseMeshComponent->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	BaseMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	DestructibleMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DestructibleMeshComponent"));
	DestructibleMeshComponent->SetOnlyOwnerSee(false);
	DestructibleMeshComponent->SetupAttachment(this);
	DestructibleMeshComponent->bCastDynamicShadow = true;
	DestructibleMeshComponent->CastShadow = true;
	DestructibleMeshComponent->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	DestructibleMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));*/

	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UOxiDestructibleComponent::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
//	RegisterComponent();
//	RegisterComponentTickFunctions(true);
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
		const float t = 1.0f - FMath::Clamp((GetWorld()->GetUnpausedTimeSeconds() - SmearStartTime) / SmearLengthSec, 0.0f, 1.0f);
		const float FinalStrength = SmearStartStrength * t;


		UMaterialInstanceDynamic* const Mat = Cast<UMaterialInstanceDynamic>(DestructibleMeshComponent->GetMaterial(1));		// TODO: Unhardcode this
		if (Mat)
		{
			Mat->SetScalarParameterValue("SmearStrength", SmearStartStrength* t);
			UE_LOG(LogTemp, Log, TEXT("-> %f New smear strenght  = %f"), t, SmearStartStrength * t);
		}
	}
}

int UOxiDestructibleComponent::TakeDamage_Internal(const int DamageAmount, const AActor* DamageCauser)
{
	if (Health > 0.0f && Health - DamageAmount <= 0.0f)
	{
		BaseMeshComponent->SetHiddenInGame(true);
		BaseMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (DestructibleMeshComponent != nullptr)
		{
			SmearStartTime = GetWorld()->GetUnpausedTimeSeconds();
			DestructibleMeshComponent->SetHiddenInGame(false);
			DestructibleMeshComponent->SetAllBodiesSimulatePhysics(true);
			DestructibleMeshComponent->SetSimulatePhysics(true);
			DestructibleMeshComponent->WakeAllRigidBodies();
			DestructibleMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
			
			const USkeletalMeshSocket* const ExplosionSocket = DestructibleMeshComponent->GetSocketByName("ExplosionLocation");
			FVector ExplosionLocation = DestructibleMeshComponent->GetSocketLocation("ExplosionLocation");

			bool bFirst = true;
			for (FBodyInstance* BI : DestructibleMeshComponent->Bodies)
			{
				FVector ImpulseDir = (BI->GetCOMPosition() - ExplosionLocation).GetSafeNormal() * ExplosionImpulseMagnitude;
				const FName BoneName = DestructibleMeshComponent->GetBoneName(BI->InstanceBoneIndex);


				const float XYImpulse = FMath::RandRange(ExplosionXYImpulseMin, ExplosionXYImpulseMax);
				FVector XAmount = XYImpulse * DestructibleMeshComponent->GetComponentTransform().TransformFVector4(FVector4(1.0f, 0.0f, 0.0f, 1.0f));
				FVector YAmount = XYImpulse * DestructibleMeshComponent->GetComponentTransform().TransformFVector4(FVector4(0.0f, 1.0f, 0.0f));

				FVector FinalImpulse = ImpulseDir + XAmount + YAmount;

				FTransform BodyTransform = BI->GetUnrealWorldTransform();
				BodyTransform.SetLocation(BodyTransform.GetLocation() + FinalImpulse.GetSafeNormal() * SmearInitialPopDistance);
				BI->SetBodyTransform(BodyTransform, ETeleportType::None);

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
	}

	Health -= DamageAmount;
	return 1.0f;
}