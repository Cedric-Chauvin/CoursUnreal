// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickable.h"
#include "PaintProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
APickable::APickable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
}

void APickable::UseObject(AActor* user)
{
	APaintProjectile* projectile = GetWorld()->SpawnActor<APaintProjectile>(user->GetTransform().GetLocation(), FRotator());
	projectile->ProjectileMovement->Velocity = user->GetActorForwardVector()*1000;
	projectile->SetActorRotation(FRotator(0));
}

// Called when the game starts or when spawned
void APickable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

