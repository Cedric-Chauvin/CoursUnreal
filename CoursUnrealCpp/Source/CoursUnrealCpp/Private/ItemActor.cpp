// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"
#include "Components/SphereComponent.h"
#include "../CoursUnrealCppCharacter.h"
#include "MazeScriptLevel.h"

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	SetRootComponent(Mesh);

	Sphere = CreateDefaultSubobject<USphereComponent>(FName("Sphere"));
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::BeginOverlap);
	Sphere->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemActor::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACoursUnrealCppCharacter* chara = Cast< ACoursUnrealCppCharacter>(OtherActor);
	if (!chara)
		return;
	if(chara->AddItem(Data))
		Destroy();
}

AItemSpawn::AItemSpawn() {

}

void AItemSpawn::BeginPlay()
{
	AMazeScriptLevel* level = Cast<AMazeScriptLevel>(GetLevel()->GetLevelScriptActor());
	if (!level)
		return;
	level->AddItemSpawn(GetActorLocation());
}
