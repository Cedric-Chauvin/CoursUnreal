// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTriggerActor.h"
#include "Components/BoxComponent.h"
#include "MazeScriptLevel.h"
#include "../CoursUnrealCppCharacter.h"

// Sets default values
ALevelTriggerActor::ALevelTriggerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	box = CreateDefaultSubobject<UBoxComponent>(FName("Trigger"));
	box->OnComponentBeginOverlap.AddDynamic(this, &ALevelTriggerActor::OnOverlapBegin);
	box->OnComponentEndOverlap.AddDynamic(this, &ALevelTriggerActor::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ALevelTriggerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelTriggerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelTriggerActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	AMazeScriptLevel* levelManager = Cast<AMazeScriptLevel>(GetLevel()->GetLevelScriptActor());
	if(!Cast<ACoursUnrealCppCharacter>(OtherActor) && levelManager)
		return;
	for (FName var : lvlToLoad)
	{
		levelManager->AddOrRemoveCompt(var, true);
	}
	levelManager->ChangeCurrentLVL(lvlName);
}

void ALevelTriggerActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMazeScriptLevel* levelManager = Cast<AMazeScriptLevel>(GetLevel()->GetLevelScriptActor());
	if (!Cast<ACoursUnrealCppCharacter>(OtherActor) && levelManager)
		return;
	for (FName var : lvlToLoad)
	{
		levelManager->AddOrRemoveCompt(var, false);
	}
}

