// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CoursUnrealCppGameMode.generated.h"

UCLASS(minimalapi)
class ACoursUnrealCppGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACoursUnrealCppGameMode();

	void RespawnPlayer();
};



