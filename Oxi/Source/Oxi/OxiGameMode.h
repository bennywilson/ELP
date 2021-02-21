// ELP 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OxiGameMode.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCharacterDeath, class UOxiCharacterComponent*, UOxiCharacterComponent*)
typedef FOnCharacterDeath::FDelegate FOnCharacterDeathDelegate;

UCLASS(Config = Engine)

class UCombatManager : public UObject
{
	GENERATED_BODY()

public:

	static UCombatManager* Get();

	UCombatManager();
	virtual void BeginDestroy() override;

	FOnCharacterDeath& GetCharacterDeathDelegates() { return CharacterDeathDelegates; }

	void TriggerDeathEvent(class UOxiCharacterComponent* Victim, UOxiCharacterComponent* Killer);

private:

	FOnCharacterDeath CharacterDeathDelegates;
};

UCLASS(minimalapi)
class AOxiGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOxiGameMode();

	void BeginDestroy();

private:

	UCombatManager* CombatManager;
};



