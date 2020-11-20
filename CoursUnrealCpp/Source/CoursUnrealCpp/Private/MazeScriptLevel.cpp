// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeScriptLevel.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"

AMazeScriptLevel::AMazeScriptLevel() {
	PrimaryActorTick.bCanEverTick = true;
}

void AMazeScriptLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FLatentActionManager& LatentManager = GetWorld()->GetLatentActionManager();
	if (LatentManager.FindExistingAction<FStreamLevelAction>(info.CallbackTarget, info.UUID) == nullptr) {
		if (levelToLoad.Num() > 0) {
			UGameplayStatics::LoadStreamLevel(this, levelToLoad[0], true, false, info);
			levelToLoad.RemoveAt(0);
		}
		else if (levelToUnload.Num() > 0) {
			UGameplayStatics::UnloadStreamLevel(this, levelToUnload[0], FLatentActionInfo(), false);
			levelToUnload.RemoveAt(0);
		}
	}
}

void AMazeScriptLevel::AddOrRemoveCompt(FName lvlName, bool add)
{
	if (!add && !levelsCount.Contains(lvlName))
		return;
	
	if (!levelsCount.Contains(lvlName)) {
		levelToLoad.Add(lvlName);
		if (levelToUnload.Contains(lvlName))
			levelToUnload.Remove(lvlName);
		levelsCount.Add(lvlName, 0);
	}
	levelsCount[lvlName] += add ? 1 : -1;
	if (levelsCount[lvlName] == 0) {
		levelToUnload.Add(lvlName);
		if (levelToLoad.Contains(lvlName)) 
			levelToLoad.Remove(lvlName);
		levelsCount.Remove(lvlName);
	}
}
