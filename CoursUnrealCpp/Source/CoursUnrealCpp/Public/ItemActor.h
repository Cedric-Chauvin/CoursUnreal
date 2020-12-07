// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "ItemActor.generated.h"

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

public :
		UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int Price;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int Weight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UTexture2D* Sprite;
};

UCLASS()
class COURSUNREALCPP_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class USphereComponent* Sphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Mesh;

	FItemData* Data;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};


UCLASS()
class COURSUNREALCPP_API AItemSpawn : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItemSpawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};


