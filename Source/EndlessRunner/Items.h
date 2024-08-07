// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items.generated.h"

UCLASS()
class ENDLESSRUNNER_API AItems : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AItems();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PlayPickUpSound();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	class USoundBase* PickUpSound;



};
