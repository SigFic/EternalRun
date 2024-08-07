// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EndlessRunnerGameMode.generated.h"

UCLASS(minimalapi)
class AEndlessRunnerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AEndlessRunnerGameMode();

private:
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform points", meta = (AllowPrivateAccess = "true"))
	class AFloor* FirstFloor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform points", meta = (AllowPrivateAccess = "true"))
	AFloor* LastFloor;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform points", meta = (AllowPrivateAccess = "true"))
	TArray<AFloor*> Floors;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Root", meta = (AllowPrivateAccess = "true"))
	//TSubclassOf<AFloor> FloorClass;

	int FloorCounterIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Root", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AFloor>> FloorClasses;

	int GetRandIndex();

	/*Counter for left and right Corners*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Root", meta = (AllowPrivateAccess = "true"))
	int CounterLeftCorners = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Root", meta = (AllowPrivateAccess = "true"))
	int CounterRightCorners = 0;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void initializeFloor();


public:
	UFUNCTION(BlueprintCallable)
	AFloor* AddFloor(FTransform Transform);

	TArray<AFloor*> GetFloors() { return Floors; }

	TArray<TSubclassOf<AFloor>> GetFloorClasses() { return FloorClasses; }

	FORCEINLINE int GetFloorCounterIndex() { return FloorCounterIndex; }
	FORCEINLINE void IncrementFloorCounterIndex() { FloorCounterIndex++; }

	FORCEINLINE AFloor* GetFirstFloor() { return FirstFloor; }
	FORCEINLINE AFloor* GetLastFloor() { return LastFloor; }

	void SetFirsFloor(AFloor* Value) { FirstFloor = Value; }
	void SetLastFloor(AFloor* Value) { LastFloor = Value; }

	bool bIsFloorInitiliazed = false;

};



