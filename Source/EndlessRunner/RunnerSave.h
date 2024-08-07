// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RunnerSave.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API URunnerSave : public USaveGame
{
	GENERATED_BODY()
	
public:

    UPROPERTY(VisibleAnywhere, Category = Basic)
    float MaxXP;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    float MaxGold;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    float MaxTime;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    float TotalGold;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    float MagnetTime;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    float LifeShieldTime;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    float XPDoublerTime;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    float MaxMagnetTime;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    float MaxXPDoublerTime;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    float MaxLifeShieldTime;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    float MagnetBuyValue;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    float ShieldBuyValue;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    float XPDoublerBuyValue;
};

