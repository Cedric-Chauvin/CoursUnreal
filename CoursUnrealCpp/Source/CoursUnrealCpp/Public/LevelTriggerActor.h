 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelTriggerActor.generated.h"

UCLASS()
class COURSUNREALCPP_API ALevelTriggerActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Level, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Level, meta = (AllowPrivateAccess = "true"))
	FName lvlName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Level, meta = (AllowPrivateAccess = "true"))
	TArray<FName> lvlToLoad;

public:
	// Sets default values for this actor's properties
	ALevelTriggerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
