// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Barrier.generated.h"

UCLASS()
class ENDLESSRUNNER_API ABarrier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABarrier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	/*Root mesh and Static mesh of barrier*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshes", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshes", meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootScene;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
