// Fill out your copyright notice in the Description page of Project Settings.


#include "Floor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Barrier.h"
#include "EndlessRunnerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "EndlessRunnerGameMode.h"
#include "Coins.h"
#include "Items.h"


// Sets default values
AFloor::AFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene"));
	SetRootComponent(RootScene);

	Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor Static Mesh"));
	Floor->SetupAttachment(RootScene);

	WallLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall Left Static Mesh"));
	WallLeft->SetupAttachment(RootScene);

	WallRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall Right Static Mesh"));
	WallRight->SetupAttachment(RootScene);

	/*this will hold transfrom information for the next floor*/
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Component of Floor"));
	ArrowComponent->SetupAttachment(RootScene);
	
	LanePositionsPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Lane Position Point"));
	LanePositionsPoint->SetupAttachment(RootScene);

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger for the add floor at the tile"));
	Trigger->SetupAttachment(RootScene);

	MaximumBarrierAndCoinNumber = 6;
	NumberOfBarriers = 3;
	NumberOfCoins = 3;
	ItemSpawnProbability = 18;
	TwoItemSpawnTogetherProbability = 8;
	ThreeItemSpawnTogetherProbability = 3;

}

// Called when the game starts or when spawned
void AFloor::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < MaximumBarrierAndCoinNumber; i++)
	{
		BarriersArray[i] = nullptr;
		CoinsArray[i] = nullptr;
		ItemArray[i] = nullptr;
	}

	DetermineIndexes(Indexes, 9, 9);
	DetermineIndexes(ItemRandIndexes, 3, 3);


	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AFloor::OnBoxOverlap);

}

void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FTransform AFloor::GetTransformPoint()
{
	return ArrowComponent->GetComponentTransform();
}

void AFloor::SpawnObjects()
{
	for (int i = 0; i < NumberOfBarriers; i++)
	{
		BarriersArray[i] = SpawnBarrier(TransfromPoints[Indexes[i] - 1] * GetActorTransform());
	}

	for (int i = 0; i < NumberOfCoins; i++)
	{
		CoinsArray[NumberOfBarriers + i] = SpawnCoins(TransfromPoints[Indexes[NumberOfBarriers + i] - 1] * GetActorTransform());
	}

	//int number = FMath::RandRange(0, 99);
	if (ItemSpawnProbability > FMath::RandRange(0, 99))
	{
		int RandNumber = FMath::RandRange(0, 99);

		if (ThreeItemSpawnTogetherProbability > RandNumber)
		{
			ItemArray[0] = SpawnItems(TransfromPoints[Indexes[NumberOfBarriers + NumberOfCoins] - 1] * GetActorTransform(), ItemRandIndexes[0] -1 );
			ItemArray[1] = SpawnItems(TransfromPoints[Indexes[NumberOfBarriers + NumberOfCoins + 1] - 1] * GetActorTransform(), ItemRandIndexes[1] - 1);
			ItemArray[2] = SpawnItems(TransfromPoints[Indexes[NumberOfBarriers + NumberOfCoins + 2] - 1] * GetActorTransform(), ItemRandIndexes[2] - 1);
		}

		else if (TwoItemSpawnTogetherProbability > RandNumber)
		{
			ItemArray[0] = SpawnItems(TransfromPoints[Indexes[NumberOfBarriers + NumberOfCoins] - 1] * GetActorTransform(), ItemRandIndexes[0]- 1);
			ItemArray[1] = SpawnItems(TransfromPoints[Indexes[NumberOfBarriers + NumberOfCoins + 1] - 1] * GetActorTransform(), ItemRandIndexes[1] -1 );
		}

		else
		{
			//RandomItemIndex = FMath::RandRange(0, 1);
			ItemArray[0] = SpawnItems(TransfromPoints[Indexes[NumberOfBarriers + NumberOfCoins] - 1] * GetActorTransform(), ItemRandIndexes[0] - 1);
		}
	}
}

ABarrier* AFloor::SpawnBarrier(FTransform SpawnPoint)
{	
	int RandNumber;
	//FTransform SpawnLocation();
	do
	{
		RandNumber = FMath::RandRange(0, 2);
		if ((RandNumber == 2) && NumberOfBarrier2 < 2)
		{
			NumberOfBarrier2++;
		}
	} while ((RandNumber == 2 )&& NumberOfBarrier2 == 2); // düzgün calismaz ise < de

	ABarrier* BarrierValue = GetWorld()->SpawnActor<ABarrier>(BarrierClasses[RandNumber], SpawnPoint.GetLocation(), SpawnPoint.GetRotation().Rotator());
	if (BarrierValue)
	{
		BarrierValue->SetOwner(this);
		return BarrierValue;
	}

	return nullptr;
}

void AFloor::OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEndlessRunnerCharacter* RunnerCharRef = Cast<AEndlessRunnerCharacter>(OtherActor);
	AEndlessRunnerGameMode* GameModeRef = Cast<AEndlessRunnerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (RunnerCharRef && GameModeRef)
	{

		AFloor* NewFloor = GameModeRef->AddFloor(GameModeRef->GetFloors().Last()->GetTransformPoint());

		/*Set new floor as last floor*/
		GameModeRef->GetLastFloor()->SetNextFloor(NewFloor);
		NewFloor->SetPreviousFloor(GameModeRef->GetLastFloor());
		GameModeRef->SetLastFloor(NewFloor);
		NewFloor->SpawnObjects();

		/*calculate lane positions*/
		RunnerCharRef->CalculateLanePositions(GetNextFloor()->GetLanePositionPointLocation(), 250.f);
		RunnerCharRef->CurrentFlorr = GetNextFloor();

		if (GameModeRef->GetFloors()[GameModeRef->GetFloorCounterIndex()] && GameModeRef->GetFloors().Num()> 11)
		{
			AFloor* DeletedFloor = GameModeRef->GetFloors()[GameModeRef->GetFloorCounterIndex()];
			if (DeletedFloor)
			{
				GameModeRef->IncrementFloorCounterIndex();
				GameModeRef->GetFloors().Remove(DeletedFloor);
				DeletedFloor->DestroyFloor();
			}
			
		}
	}
}

void AFloor::DestroyFloor()
{

	for (int i = 0; i < MaximumBarrierAndCoinNumber; i++)
	{
		if (BarriersArray[i] != nullptr)
		{
			BarriersArray[i]->Destroy();
			BarriersArray[i] = nullptr;
		}

		if (CoinsArray[i] != nullptr && CoinsArray[i]->GetOwner() == this)
		{
			CoinsArray[i]->Destroy();
			CoinsArray[i] = nullptr;
		}

		if (ItemArray[i] != nullptr && ItemArray[i]->GetOwner() == this)
		{
			ItemArray[i]->Destroy();
			ItemArray[i] = nullptr;
		}
	}

	AEndlessRunnerGameMode* GameModeRef = Cast<AEndlessRunnerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameModeRef)
	{
		GameModeRef->SetFirsFloor(GetNextFloor());
		GetNextFloor()->SetPreviousFloor(nullptr);
		SetNextFloor(nullptr);
	}
	

	this->Destroy();
}

void AFloor::DetermineIndexes(int* indexes, int size, int number)
{
	int arrayindex = 0;
	indexes[arrayindex] = FMath::RandRange(0, size - 1) + 1;
	arrayindex++;
	number--;

	while (number > 0) {
		int RandNumber = FMath::RandRange(0, size - 1);
		bool isUnique = true;

		// Rastgele sayýnýn benzersiz olup olmadýðýný kontrol et
		for (int i = 0; i < arrayindex; i++) {
			if ((RandNumber + 1) == indexes[i]) {
				isUnique = false;
				break;
			}
		}

		// Eðer benzersiz ise array'e ekle
		if (isUnique) {
			indexes[arrayindex] = RandNumber + 1;
			arrayindex++;
			number--;
		}
	}
}

ACoins* AFloor::SpawnCoins(FTransform Transform)
{
	FRotator Rotation = Transform.Rotator();
	Rotation.Pitch += 90.f;

	Transform.SetRotation(Rotation.Quaternion());
	ACoins* Coin = GetWorld()->SpawnActor<ACoins>(CoinClass, Transform.GetLocation(), Rotation);
	if (Coin)
	{
		Coin->SetOwner(this);
		return Coin;
	}

	return nullptr;
}

AItems* AFloor::SpawnItems(FTransform SpawnPoint, int Index)
{
	FRotator ItemRotation = SpawnPoint.GetRotation().Rotator();
	ItemRotation.Yaw += 90.f;
	AItems* Item = GetWorld()->SpawnActor<AItems>(ItemClasses[Index], SpawnPoint.GetLocation() , ItemRotation);
	if (Item)
	{
		Item->SetOwner(this);
		return Item;
	}
	return nullptr;
}

// Called every frame

