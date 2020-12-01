// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory.generated.h"

/**
 * 
 */
UCLASS()
class COURSUNREALCPP_API UInventory : public UUserWidget
{
	GENERATED_BODY()
public :
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateInventoryWidget(const TArray<UTexture2D*>& textures);
};
