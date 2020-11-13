// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoursUnrealCppGameMode.h"
#include "CoursUnrealCppCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACoursUnrealCppGameMode::ACoursUnrealCppGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
