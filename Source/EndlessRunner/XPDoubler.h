// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items.h"
#include "XPDoubler.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API AXPDoubler : public AItems
{
	GENERATED_BODY()

public:

	AXPDoubler();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//void StartInterping();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
