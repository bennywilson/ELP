// ELP 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "OxiWeaponAnimInstance.h"
#include "OxiCharacter.generated.h"

class UInputComponent;

UCLASS(config=Game)
class AOxiCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	class USkeletalMeshComponent* FP_HandsOutline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	class USkeletalMeshComponent* FP_Gun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	class USkeletalMeshComponent* FP_GunOutline;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	UFUNCTION(BlueprintCallable, Category = "Oxi Character")
	UOxiWeaponAnimInstance* GetWeaponAnimBP() const { return Cast<UOxiWeaponAnimInstance>(FP_Gun->GetAnimInstance()); }

	UFUNCTION(BlueprintCallable, Category = "Oxi Character")
	UOxiWeaponAnimInstance* GetWeaponOutlineAnimBP() const { return Cast<UOxiWeaponAnimInstance>(FP_GunOutline->GetAnimInstance()); }

	UFUNCTION(BlueprintCallable, Category = "Oxi Character")
	USkeletalMeshComponent* GetWeaonSkeletalMesh() const { return FP_Gun; }

public:
	AOxiCharacter();

protected:
	virtual void BeginPlay();

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

protected:
	
	void OnFire();

	void MoveForward(float Val);
	void MoveRight(float Val);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

