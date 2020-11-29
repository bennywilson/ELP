// ELP 2020

#include "OxiDestructibleActor.h"

UOxiDamageInterface::UOxiDamageInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

float IOxiDamageInterface::TakeDamage_Implementation(const float DamageAmount, const AActor* DamageCauser)
{
	UE_LOG(LogTemp, Log, TEXT("TakeDamage_Implementation() - Finally made it.  Sheesh!"));
	return 0.0f;
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
}

void UOxiDestructibleComponent::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (BaseMeshComponent != nullptr)
	{
		BaseMeshComponent->SetHiddenInGame(false, true);
	}

	if (DestructibleMeshComponent != nullptr)
	{
		DestructibleMeshComponent->SetHiddenInGame(false, true);
	}
}

bool UOxiDestructibleComponent::InitDestructibleComponent(UStaticMeshComponent* InBaseMeshComponent, USkeletalMeshComponent* InDestructibleMeshComponent)
{
	BaseMeshComponent = InBaseMeshComponent;
	DestructibleMeshComponent = InDestructibleMeshComponent;

	return true;
}

void UOxiDestructibleComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

float UOxiDestructibleComponent::TakeDamage_Internal(const float DamageAmount, const AActor* DamageCauser)
{
	UE_LOG(LogTemp, Log, TEXT("Made it yo!"));
	return 1.3f;
}