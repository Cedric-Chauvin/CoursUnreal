// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetMathLibrary.h"

void APaintProjectile::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
	ADecalActor* decal = GetWorld()->SpawnActor<ADecalActor>(Hit.Location,FRotator());
	UMaterialInterface* mat = FindObject<UMaterialInterface>(NULL, TEXT("/Game/Material/PaintSplash"));
	if(!mat)
		mat = LoadObject<UMaterialInterface>(NULL, TEXT("/Game/Material/PaintSplash"));
	decal->SetDecalMaterial(mat);
	decal->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(Hit.Location, Hit.Location + Hit.Normal));
	decal->SetActorScale3D(FVector(0.5));
}

// Sets default values
APaintProjectile::APaintProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UStaticMeshComponent* mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	mesh->SetStaticMesh(LoadObject<UStaticMesh>(NULL, TEXT("/Engine/BasicShapes/Sphere.Sphere"), NULL));
	mesh->SetMaterial(0, LoadObject<UMaterialInterface>(NULL, TEXT("/Engine/BasicShapes/BasicShapeMaterial"), NULL));
	mesh->SetNotifyRigidBodyCollision(true);
	mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovement"));
	ProjectileMovement->ProjectileGravityScale = 0;
	ProjectileMovement->InitialSpeed = 500;
	ProjectileMovement->MaxSpeed = 500;
}

// Called when the game starts or when spawned
void APaintProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetActorScale3D(FVector(0.5));
	OnActorHit.AddDynamic(this, &APaintProjectile::OnHit);
}

// Called every frame
void APaintProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

