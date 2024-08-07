// Fill out your copyright notice in the Description page of Project Settings.


#include "Coins.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Magnet.h"

// Sets default values
ACoins::ACoins()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CoinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Coin's Static Mesh"));
	SetRootComponent(CoinMesh);

}

// Called when the game starts or when spawned
void ACoins::BeginPlay()
{
	Super::BeginPlay();
	
	DesiredRotation = GetActorRotation();
	DesiredRotation.Yaw += 180;
}

void ACoins::TurnCoinFunction(float DeltaTime)
{
	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Yaw = FMath::FInterpTo<float>(CurrentRotation.Yaw, DesiredRotation.Yaw, DeltaTime, TurnInterpSpeed);

	SetActorRotation(CurrentRotation);
	float AbsoluteRotationDifference = FMath::Abs<float>(CurrentRotation.Yaw - DesiredRotation.Yaw);
	if (AbsoluteRotationDifference < 10)
	{
		DesiredRotation.Yaw += 180;
	}
}

void ACoins::StartInterp(float DeltaTime)
{
	if (!bCanCoinGoToMagnet) return;
	if (!Magnet) return;

	FVector MagnetLocation = Magnet->GetActorLocation();
	FVector CurrentCoinLocation = GetActorLocation();
	CurrentCoinLocation = FMath::VInterpTo(CurrentCoinLocation, MagnetLocation, DeltaTime, ToMagnetInterpSpeed);
	//UE_LOG(LogTemp, Warning, TEXT("Magnet is Owner"));
	SetActorLocation(CurrentCoinLocation);

}

void ACoins::DestroyCoin()
{
	this->Destroy();
}

// Called every frame
void ACoins::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TurnCoinFunction(DeltaTime);
	StartInterp(DeltaTime);

}

void ACoins::PlayPickUpSound()
{
	if (PickUpSound)	UGameplayStatics::PlaySoundAtLocation(this, PickUpSound, this->GetActorLocation());
}

void ACoins::StartCoinGoesToMagnet()
{
	bCanCoinGoToMagnet = true;
	CoinMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Magnet = Cast<AMagnet>(GetOwner());
	FTimerHandle CoinDestroyHandle;
	GetWorldTimerManager().SetTimer(CoinDestroyHandle, this, &ACoins::DestroyCoin, DestroyTime, false);
}

