// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveTemplate.h"
#include "Math/Vector.h"
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
	FName currentLvl;
	int saveIndex;
	FLatentActionInfo info;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemSpawn, meta = (AllowPrivateAccess = "true"))
		class UDataTable* Table;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemSpawn, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AItemActor> spawnActor;
	FTimerHandle timer;
	TArray<FVector> spawnLocations;

	void SpawnItem();

public :
	AMazeScriptLevel();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void AddOrRemoveCompt(FName lvlName,bool add);
	void ChangeCurrentLVL(FName lvl);

	void Save();
	void LoadSave();

	void AddItemSpawn(FVector location);
};
