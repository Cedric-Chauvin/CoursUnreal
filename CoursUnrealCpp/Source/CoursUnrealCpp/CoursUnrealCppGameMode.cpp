// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoursUnrealCppGameMode.h"
#include "CoursUnrealCppCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MazeScriptLevel.h"
#include "Inventory.h"

ACoursUnrealCppGameMode::ACoursUnrealCppGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ACoursUnrealCppGameMode::RespawnPlayer()
{
	RestartPlayer(GetWorld()->GetFirstPlayerController());
	AMazeScriptLevel* level = Cast< AMazeScriptLevel>(GetLevel()->GetLevelScriptActor());
	if(level)
		level->LoadSave();
}

void ACoursUnrealCppGameMode::GamePause()
{
	if (!GetWorld()->GetFirstPlayerController()->IsPaused())
	{
		widget->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetFirstPlayerController()->SetPause(true);
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	}
	else
	{
		widget->SetVisibility(ESlateVisibility::Hidden);
		GetWorld()->GetFirstPlayerController()->SetPause(false);
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	}
}

void ACoursUnrealCppGameMode::OpenInventory(TArray<UTexture2D*> Textures)
{
	if (!GetWorld()->GetFirstPlayerController()->IsPaused())
	{
		inventoryWidget->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetFirstPlayerController()->SetPause(true);
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		inventoryWidget->UpdateInventoryWidget(Textures);
	}
	else
	{
		inventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		GetWorld()->GetFirstPlayerController()->SetPause(false);
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	}
}

void ACoursUnrealCppGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (WidgetClass)
	{
		widget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), WidgetClass);
		widget->AddToViewport();
		widget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (InventoryClass)
	{
		inventoryWidget = CreateWidget<UInventory>(GetWorld()->GetFirstPlayerController(), InventoryClass);
		inventoryWidget->AddToViewport();
		inventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ACoursUnrealCppGameMode::Save()
{
	AMazeScriptLevel* level = Cast< AMazeScriptLevel>(GetLevel()->GetLevelScriptActor());
	level->Save();
}
