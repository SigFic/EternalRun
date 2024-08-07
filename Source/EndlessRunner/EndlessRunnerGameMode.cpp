// Copyright Epic Games, Inc. All Rights Reserved.

#include "EndlessRunnerGameMode.h"
#include "EndlessRunnerCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Floor.h"

AEndlessRunnerGameMode::AEndlessRunnerGameMode() :
	FloorCounterIndex{ 0 }
{

}

int AEndlessRunnerGameMode::GetRandIndex()
{
	while (true)
	{
		int Index = FMath::RandRange(0, FloorClasses.Num() - 1);
		/*if floor is left corner*/
		if (Index != 3 && Index != 4) return Index;
		
		else if (Index == 3)
		{
			if (CounterLeftCorners == 0)
			{
				CounterLeftCorners = 4;
				CounterRightCorners = 0;
				return Index;
			}
		}

		else if (Index == 4)
		{
			if (CounterRightCorners == 0)
			{
				CounterRightCorners = 4;
				CounterLeftCorners = 0;
				return Index;
			}
		}
	}
}

void AEndlessRunnerGameMode::BeginPlay()
{
	Super::BeginPlay();
	//for (int i = 0; i < 9; i++)
	//{
	//	if (i > 0)
	//	{
	//		if (Floors.Num() > 0)
	//		{
	//			AFloor* NewFloor = AddFloor(Floors.Last()->GetTransformPoint());
	//			LastFloor->SetNextFloor(NewFloor);
	//			NewFloor->SetPreviousFloor(LastFloor);
	//			LastFloor = NewFloor;
	//			NewFloor->SpawnObjects();
	//		}
	//	}
	//	else
	//	{
	//		FirstFloor = AddFloor(FTransform());
	//		LastFloor = FirstFloor;
	//		FirstFloor->SetNextFloor(FirstFloor);
	//		FirstFloor->SetPreviousFloor(FirstFloor);
	//	}
	//}

}

void AEndlessRunnerGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEndlessRunnerGameMode::initializeFloor()
{
	for (int i = 0; i < 9; i++)
	{
		if (i > 0)
		{
			if (Floors.Num() > 0)
			{
				AFloor* NewFloor = AddFloor(Floors.Last()->GetTransformPoint());
				LastFloor->SetNextFloor(NewFloor);
				NewFloor->SetPreviousFloor(LastFloor);
				LastFloor = NewFloor;
				NewFloor->SpawnObjects();
			}
		}
		else
		{
			FirstFloor = AddFloor(FTransform());
			LastFloor = FirstFloor;
			FirstFloor->SetNextFloor(FirstFloor);
			FirstFloor->SetPreviousFloor(FirstFloor);
		}
	}

	bIsFloorInitiliazed = true;
}

AFloor* AEndlessRunnerGameMode::AddFloor(FTransform Transform)
{
	int Index = GetRandIndex();
	if (Index != 3 && CounterLeftCorners > 0) CounterLeftCorners--;

	if (Index != 4 && CounterRightCorners > 0) CounterRightCorners--;

	AFloor* FloorRef = GetWorld()->SpawnActor<AFloor>(FloorClasses[Index], Transform);
	if (FloorRef)
	{
		Floors.Add(FloorRef);
		return FloorRef;
	}

	return nullptr;
}


