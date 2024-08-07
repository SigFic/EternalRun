// Fill out your copyright notice in the Description page of Project Settings.


#include "Barrier.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ABarrier::ABarrier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	SetRootComponent(RootScene);


	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh component"));
	StaticMeshComp->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void ABarrier::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABarrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

