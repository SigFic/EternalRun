// Fill out your copyright notice in the Description page of Project Settings.


#include "Items.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AItems::AItems()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Static Mesh"));
	SetRootComponent(ItemMesh);

}

// Called when the game starts or when spawned
void AItems::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItems::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItems::PlayPickUpSound()
{
	if (PickUpSound)	UGameplayStatics::PlaySoundAtLocation(this, PickUpSound, this->GetActorLocation());
}

