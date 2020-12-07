// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SaveTemplate.h" 
#include "CoursUnrealCppGameMode.generated.h"

UCLASS(minimalapi)
class ACoursUnrealCppGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Menu, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> WidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Menu, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UInventory> InventoryClass;

	UUserWidget* widget;
	class UInventory* inventoryWidget;

public:
	ACoursUnrealCppGameMode();

	void RespawnPlayer();

	UFUNCTION(BlueprintCallable)
	void GamePause();

	void OpenInventory(TArray<UTexture2D*> Textures);

	void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		void Save();
};



