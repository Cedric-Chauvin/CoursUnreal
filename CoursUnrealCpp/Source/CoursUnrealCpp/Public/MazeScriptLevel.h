// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MazeScriptLevel.generated.h"

/**
 * 
 */
UCLASS()
class COURSUNREALCPP_API AMazeScriptLevel : public ALevelScriptActor
{
	GENERATED_BODY()

	TMap<FName, int> levelsCount;
	TArray<FName> levelToLoad;
	TArray<FName> levelToUnload;
	FLatentActionInfo info;



public :
	AMazeScriptLevel();
	void AddOrRemoveCompt(FName lvlName,bool add);
	virtual void Tick(float DeltaTime) override;
};
