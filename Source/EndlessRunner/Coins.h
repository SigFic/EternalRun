// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Coins.generated.h"

UCLASS()
class ENDLESSRUNNER_API ACoins : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoins();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PlayPickUpSound();

	void StartCoinGoesToMagnet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* CoinMesh = nullptr;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	class USoundBase* PickUpSound;

	void TurnCoinFunction(float DeltaTime);

	FRotator DesiredRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InterpSpeed", meta = (AllowPrivateAccess = "true"))
	float TurnInterpSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InterpSpeed", meta = (AllowPrivateAccess = "true"))
	float ToMagnetInterpSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InterpSpeed", meta = (AllowPrivateAccess = "true"))
	float DestroyTime;

	bool bCanCoinGoToMagnet = false;

	void StartInterp(float DeltaTime);

	class AMagnet* Magnet = nullptr;

	void DestroyCoin();

	//AMagnet* MagnetRef = nullptr;

};
