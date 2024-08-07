// Fill out your copyright notice in the Description page of Project Settings.


#include "Magnet.h"
#include "Components/SphereComponent.h"
#include "Coins.h"
#include "Floor.h"
#include "Kismet/GameplayStatics.h"
#include "EndlessRunner/EndlessRunnerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Curves/CurveFloat.h"

AMagnet::AMagnet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MagnetArea = CreateDefaultSubobject<USphereComponent>(TEXT("Magnet Area"));
	MagnetArea->SetupAttachment(RootComponent);
}

void AMagnet::BeginPlay()
{
	Super::BeginPlay();
	SetSphereCollisionSettings(false);

	MagnetArea->OnComponentBeginOverlap.AddDynamic(this, &AMagnet::OnAreaOverlap);

	APlayerController* PlayerControllerRef = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerControllerRef)
	{
		RunnerChar = Cast<AEndlessRunnerCharacter>(PlayerControllerRef->GetPawn());
	}
}

void AMagnet::OnAreaOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACoins* CoinRef = Cast<ACoins>(OtherActor);

	if (RunnerChar)
	{
		RunnerChar->GetCoin(CoinRef);
		CoinRef->SetOwner(this);
		CoinRef->StartCoinGoesToMagnet();
	}
	
}

void AMagnet::AttractCoin(ACoins* Coin)
{
	Coin->StartCoinGoesToMagnet();
}

void AMagnet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMagnet::SetSphereCollisionSettings(bool Value)
{
	if (!Value)
	{
		MagnetArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		MagnetArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		UStaticMeshComponent* RootRef = Cast<UStaticMeshComponent>(GetRootComponent());
		if (RootRef) RootRef->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}
}
