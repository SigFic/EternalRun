// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items.h"
#include "LifeShield.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API ALifeShield : public AItems
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALifeShield();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//void StartInterping();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* LifeShieldMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variables", meta = (AllowPrivateAccess = "true"))
	int ShieldTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* UseShieldSound;



public:

	int GetShieldTime() { return ShieldTime; }

	void OnShieldTaken();

	void ShieldUseSound();



	
};
