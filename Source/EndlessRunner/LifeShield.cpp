// Fill out your copyright notice in the Description page of Project Settings.


#include "LifeShield.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

ALifeShield::ALifeShield()
{
	PrimaryActorTick.bCanEverTick = true;

	LifeShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Of Shiled"));
	LifeShieldMesh->SetupAttachment(GetRootComponent());
}
void ALifeShield::BeginPlay()
{
	Super::BeginPlay();

}

void ALifeShield::OnShieldTaken()
{	
	LifeShieldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ALifeShield::ShieldUseSound()
{
	if (UseShieldSound)	UGameplayStatics::PlaySoundAtLocation(this, UseShieldSound, this->GetActorLocation());
}

void ALifeShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

