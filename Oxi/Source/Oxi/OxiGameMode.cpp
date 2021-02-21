// ELP 2020

#include "OxiGameMode.h"
#include "OxiHUD.h"
#include "OxiCharacter.h"
#include "UObject/ConstructorHelpers.h"


static UCombatManager* GCombatManager = nullptr;
UCombatManager::UCombatManager()
{

}

UCombatManager* UCombatManager::Get()
{
	return GCombatManager;
}

void UCombatManager::BeginDestroy()
{
	Super::BeginDestroy();
}

void UCombatManager::TriggerDeathEvent(class UOxiCharacterComponent* Victim, UOxiCharacterComponent* Killer)
{
	CharacterDeathDelegates.Broadcast(Victim, Killer);
}

AOxiGameMode::AOxiGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AOxiHUD::StaticClass();

	//if (GIsPlayInEditorWorld || GIsEditor == false)
	{
		CombatManager = CreateDefaultSubobject<UCombatManager>(TEXT("CombatManager"));

		//check(GCombatManager == nullptr);
		GCombatManager = CombatManager;
	}
}

void AOxiGameMode::BeginDestroy()
{
	Super::BeginDestroy();

//s	if (GIsPlayInEditorWorld || GIsEditor == false)
	{
	//	check(GCombatManager != nullptr);
		GCombatManager = nullptr;
	}
}