// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveTemplate.generated.h"

/**
 * 
 */
UCLASS()
class COURSUNREALCPP_API USaveTemplate : public USaveGame
{
	GENERATED_BODY()
	
public :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Save)
	FTransform playerTransform;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Save)
	TMap<FName, int> levelsState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Save)
	FDateTime saveTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Save)
		int saveIndex;

};
