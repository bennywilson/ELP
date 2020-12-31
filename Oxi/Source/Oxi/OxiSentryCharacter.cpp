// Fill out your copyright notice in the Description page of Project Settings.

#include "OxiSentryCharacter.h"

float UOxiSentryCharacter::TakeDamage_Internal(const float DamageAmount, const AActor* DamageCauser)
{
	Super::TakeDamage_Internal(DamageAmount, DamageCauser);

	UE_LOG(LogTemp, Log, TEXT("ERE!"));

	return 0.f;
}