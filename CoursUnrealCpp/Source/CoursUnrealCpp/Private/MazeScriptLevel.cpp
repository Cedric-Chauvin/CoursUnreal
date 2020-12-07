// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeScriptLevel.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"
#include "GameFramework/SaveGame.h"
#include "GameFramework/Character.h" 
#include "..\Public\MazeScriptLevel.h"
#include "Engine/DataTable.h"
#include "TimerManager.h"
#include "ItemActor.h"
#include "../CoursUnrealCppCharacter.h"

void AMazeScriptLevel::SpawnItem()
{
	if (spawnLocations.Num() == 0)
		return;
	TArray<FName> names = Table->GetRowNames();
	int random = FMath::RandRange(0, names.Num() - 1);
	FString context = "SpawnItem";
	FItemData* data = Table->FindRow<FItemData>(names[random], context);
	random = FMath::RandRange(0, spawnLocations.Num() - 1);
	AItemActor* item = GetWorld()->SpawnActor<AItemActor>(spawnActor, spawnLocations[random], FRotator(0));
	item->Data = data;
	spawnLocations.RemoveAt(random);
}

AMazeScriptLevel::AMazeScriptLevel() {
	PrimaryActorTick.bCanEverTick = true;
}

void AMazeScriptLevel::BeginPlay()
{
	Super::BeginPlay();
	LoadSave();
	timer = FTimerHandle();
	GetWorldTimerManager().SetTimer(timer, this, &AMazeScriptLevel::SpawnItem, 10, true, 10);
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

void AMazeScriptLevel::ChangeCurrentLVL(FName lvl)
{
	currentLvl = lvl;
}

void AMazeScriptLevel::Save()
{
	USaveGame* tempSave = UGameplayStatics::LoadGameFromSlot("Slot0", 0);
	if (!tempSave) 
		tempSave = UGameplayStatics::CreateSaveGameObject(USaveTemplate::StaticClass());
	USaveTemplate* save = Cast<USaveTemplate>(tempSave);
	save->playerTransform = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetTransform();
	save->saveTime = FDateTime::Now();
	save->CurrentLevel = currentLvl;
	save->saveIndex = saveIndex;
	ACoursUnrealCppCharacter* chara = Cast<ACoursUnrealCppCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (chara)
		save->saveHealth = chara->Health;

	UGameplayStatics::SaveGameToSlot(save, "Slot" + FString::FromInt(saveIndex), 0);
}

void AMazeScriptLevel::LoadSave()
{
	FString string = "Slot" + FString::FromInt(saveIndex);
	if(!UGameplayStatics::DoesSaveGameExist(string, 0))
	{ 
		currentLvl = FName("Spawn");
		AddOrRemoveCompt(FName("Spawn"), true);
		AddOrRemoveCompt(FName("Corridor1"), true);
		return;
	}
	USaveGame* tempSave = UGameplayStatics::LoadGameFromSlot(string, 0);
	if (tempSave) {
		USaveTemplate* save = Cast<USaveTemplate>(tempSave);
		saveIndex = save->saveIndex;
		if (save->playerTransform.GetLocation() != FVector(0))
			UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->SetActorTransform(save->playerTransform);
		if (save->CurrentLevel.IsNone() || save->CurrentLevel == FName("Spawn")) {
			currentLvl = FName("Spawn");
			AddOrRemoveCompt(FName("Spawn"), true);
			AddOrRemoveCompt(FName("Corridor1"), true);
			AddOrRemoveCompt(FName("ShapeL"), true);
			AddOrRemoveCompt(FName("SyncMap"), true);
		}
		if (save->saveHealth != 0)
		{
			ACoursUnrealCppCharacter* chara = Cast<ACoursUnrealCppCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			chara->Health = save->saveHealth;
		}
	}
}

void AMazeScriptLevel::AddItemSpawn(FVector location)
{
	spawnLocations.Add(location);
}
