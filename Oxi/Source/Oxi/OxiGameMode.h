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

	UCombatManager();

	virtual void BeginDestroy() override;

	template <typename UObjectTemplate, typename... VarTypes>
	static inline FDelegateHandle RegisterEventListener(UObjectTemplate* const InUserObject, const FName& InFunctionName)
	{
		if (InUserObject == nullptr)
		{
			UE_LOG(LogCombat, Log, TEXT("UCombatManager::RegisterEventListener() - NULL InUseObject. InFunctionName = %s"), *InUserObject->GetFullName(), *InFunctionName.ToString());
			return FDelegateHandle();
		}

		if (Instance == nullptr)
		{
			UE_LOG(LogCombat, Log, TEXT("UCombatManager::RegisterEventListener() - NULL combat manager.  InUserObject = %s.  InFunctionName = %s"), *InUserObject->GetFullName(), *InFunctionName.ToString());
			return FDelegateHandle();
		}
		return Instance->CharacterDeathDelegates.AddUFunction(InUserObject, InFunctionName);
	}

	static inline void UnregisterEventListener(const FDelegateHandle DelegateHandle)
	{
		if (Instance == nullptr)
		{
			UE_LOG(LogCombat, Log, TEXT("UCombatManager::UnregisterEventListener() - NULL combat manager."));
			return;
		}

		Instance->CharacterDeathDelegates.Remove(DelegateHandle);
	}

	static void TriggerDeathEvent(class UOxiCharacterComponent* const Victim, UOxiCharacterComponent* const Killer);

private:

	FOnCharacterDeath CharacterDeathDelegates;

	static UCombatManager* Instance;
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



