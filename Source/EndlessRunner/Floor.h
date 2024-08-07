// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floor.generated.h"

UCLASS()
class ENDLESSRUNNER_API AFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintCallable)
	class ABarrier* SpawnBarrier(FTransform SpawnPoint);

	UFUNCTION(BlueprintCallable)
	class ACoins* SpawnCoins(FTransform Transform);

	UFUNCTION(BlueprintCallable)
	class AItems* SpawnItems(FTransform SpawnPoint, int Index);

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Root", meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootScene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Floor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WallLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WallRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* ArrowComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Root", meta = (AllowPrivateAccess = "true"))
	USceneComponent* LanePositionsPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Root", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Trigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Barriers", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<ABarrier>> BarrierClasses;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Points", meta = (AllowPrivateAccess = "true"))
	TArray<FTransform> TransfromPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Points", meta = (AllowPrivateAccess = "true"))
	int NumberOfBarriers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Points", meta = (AllowPrivateAccess = "true"))
	int NumberOfCoins;


	/*this is the number of BarriersArray[2]*/
	char NumberOfBarrier2 = 0;
	char MaximumBarrierAndCoinNumber = 6;

	void DestroyFloor();

	/*index numbers is represented +1*/
	int Indexes[10];

	int ItemRandIndexes[3] = {0,0,0};
	
	void DetermineIndexes(int* indexes, int size, int number );

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coins", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ACoins> CoinClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	AFloor* NextFloor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	AFloor* PreviousFloor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	int ItemSpawnProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	int RandomItemIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	int TwoItemSpawnTogetherProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	int ThreeItemSpawnTogetherProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	int FourItemSpawnTogetherProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AItems>> ItemClasses;

	//bool bIsItFirstFloor;


public:	
	ABarrier* BarriersArray[6];

	ACoins* CoinsArray[6];

	AItems* ItemArray[6];

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FTransform GetTransformPoint();

	/*******/
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AFloor* GetNextFloor() { return NextFloor; }

	UFUNCTION(BlueprintCallable)
	void SetNextFloor(AFloor*  Value) { NextFloor = Value; }
	/*********/
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AFloor* GetPreviousFloor() { return PreviousFloor; }

	UFUNCTION(BlueprintCallable)
	void SetPreviousFloor(AFloor* Value) { PreviousFloor = Value; }

	FVector GetLanePositionPointLocation() { return LanePositionsPoint->GetComponentLocation(); }

	void SpawnObjects();


};
