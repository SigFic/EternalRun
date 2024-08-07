// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items.h"
#include "Magnet.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API AMagnet : public AItems
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AMagnet();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetSphereCollisionSettings(bool Value);

	//void StartInterping();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnAreaOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* MagnetArea = nullptr;

	class AEndlessRunnerCharacter* RunnerChar = nullptr;

	void AttractCoin(class ACoins* Coin);

};
