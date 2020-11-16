// Fill out your copyright notice in the Description page of Project Settings.


#include "KillPlayer.h"
#include "../CoursUnrealCppGameMode.h"
#include "../CoursUnrealCppCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"

void UKillPlayer::Kill(class AActor* SelfActor, class AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	OtherActor->Destroy();
	if (Cast<ACoursUnrealCppCharacter>(OtherActor) != nullptr)
	{
		ACoursUnrealCppGameMode* gameMode = Cast<ACoursUnrealCppGameMode>(GetWorld()->GetAuthGameMode());
		gameMode->RespawnPlayer();  
		FTransform transform = OtherActor->GetTransform();
		transform.SetScale3D(FVector(1));
		UParticleSystemComponent* particleInstance = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Effect, transform,true);
		FTimerHandle newTimer = FTimerHandle();
		GetOwner()->GetWorldTimerManager().SetTimer(newTimer, this, &UKillPlayer::DestroyParticle, 1, true, EffectDuration);
		timers.Add(newTimer);
		components.Add(particleInstance);
	}
}

void UKillPlayer::DestroyParticle()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(timers[0]);
	components[0]->DestroyComponent();
	timers.RemoveAt(0);
	components.RemoveAt(0);
}

// Sets default values for this component's properties
UKillPlayer::UKillPlayer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKillPlayer::BeginPlay()
{
	Super::BeginPlay();
	GetOwner()->OnActorHit.AddDynamic(this, &UKillPlayer::Kill);
}


// Called every frame
void UKillPlayer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

