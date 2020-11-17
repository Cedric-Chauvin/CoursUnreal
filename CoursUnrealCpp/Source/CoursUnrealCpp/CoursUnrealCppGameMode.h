// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CoursUnrealCppGameMode.generated.h"

UCLASS(minimalapi)
class ACoursUnrealCppGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Menu, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> WidgetClass;

	UUserWidget* widget;

public:
	ACoursUnrealCppGameMode();

	void RespawnPlayer();

	UFUNCTION(BlueprintCallable)
	void GamePause();

	void BeginPlay() override;
};



