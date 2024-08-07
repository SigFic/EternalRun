// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerGameInstace.h"
#include "Kismet/GameplayStatics.h"
#include "RunnerSave.h"

void URunnerGameInstace::SaveGame()
{
    if (UGameplayStatics::DoesSaveGameExist(FString("EndlessRunnerSave1"), 0))
    {
        SaveRef = UGameplayStatics::LoadGameFromSlot(FString("EndlessRunnerSave1"), 0);
    }
    else
    {
        SaveRef = UGameplayStatics::CreateSaveGameObject(URunnerSave::StaticClass());
    }

    if (!SaveRef)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create or load save game object"));
        return;
    }

    URunnerSave* RunnerSaveRef = Cast<URunnerSave>(SaveRef);
    if (RunnerSaveRef)
    {
        RunnerSaveRef->MaxXP = MaxXP;
        RunnerSaveRef->MaxGold = MaxGold;
        RunnerSaveRef->MaxTime = MaxTime;
        RunnerSaveRef->TotalGold = TotalGold;
        RunnerSaveRef->MagnetTime = MagnetTime;
        RunnerSaveRef->XPDoublerTime = XPDoublerTime;
        RunnerSaveRef->LifeShieldTime = LifeShieldTime;
        RunnerSaveRef->MaxMagnetTime = MaxMagnetTime;
        RunnerSaveRef->MaxXPDoublerTime = MaxXPDoublerTime;
        RunnerSaveRef->MaxLifeShieldTime = MaxLifeShieldTime;
        RunnerSaveRef->MagnetBuyValue = MagnetBuyValue;
        RunnerSaveRef->ShieldBuyValue = ShieldBuyValue;
        RunnerSaveRef->XPDoublerBuyValue = XPDoublerBuyValue;
        

        if (UGameplayStatics::SaveGameToSlot(RunnerSaveRef, FString("EndlessRunnerSave1"), 0))
        {
            UE_LOG(LogTemp, Warning, TEXT("GameSaved"));
            UE_LOG(LogTemp, Warning, TEXT("Max XP : %f"), RunnerSaveRef->MaxXP);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to save game to slot"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to cast save game object"));
    }
}

void URunnerGameInstace::LoadGame()
{
    if (UGameplayStatics::DoesSaveGameExist(FString("EndlessRunnerSave1"), 0))
    {
        SaveRef = UGameplayStatics::LoadGameFromSlot(FString("EndlessRunnerSave1"), 0);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Save game does not exist"));
        return;
    }

    if (!SaveRef)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load save game object"));
        return;
    }

    URunnerSave* RunnerSaveRef = Cast<URunnerSave>(SaveRef);
    if (RunnerSaveRef)
    {
        MaxXP = RunnerSaveRef->MaxXP;
        MaxGold = RunnerSaveRef->MaxGold;
        MaxTime = RunnerSaveRef->MaxTime;
        TotalGold = RunnerSaveRef->TotalGold;
        MagnetTime = RunnerSaveRef->MagnetTime;
        LifeShieldTime = RunnerSaveRef->LifeShieldTime;
        XPDoublerTime = RunnerSaveRef->XPDoublerTime;
        MaxMagnetTime = RunnerSaveRef->MaxMagnetTime;
        MaxXPDoublerTime = RunnerSaveRef->MaxXPDoublerTime;
        MaxLifeShieldTime = RunnerSaveRef->MaxLifeShieldTime;
        MagnetBuyValue = RunnerSaveRef->MagnetBuyValue;
        ShieldBuyValue = RunnerSaveRef->ShieldBuyValue;
        XPDoublerBuyValue = RunnerSaveRef->XPDoublerBuyValue;

        UE_LOG(LogTemp, Warning, TEXT("GameLoaded"));
        UE_LOG(LogTemp, Warning, TEXT("Game Loaded : Max XP : %f"), RunnerSaveRef->MaxXP);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to cast save game object"));
    }
}

void URunnerGameInstace::ResetAllProperties()
{
    MaxXP = 0;
    MaxGold = 0;
    MaxTime = 0;
    TotalGold = 0;
    MagnetTime = 5;
    LifeShieldTime = 5;
    XPDoublerTime = 5;
    MaxMagnetTime = 15;
    MaxXPDoublerTime = 15;
    MaxLifeShieldTime = 15;
    MagnetBuyValue = 166;
    ShieldBuyValue = 166;
    XPDoublerBuyValue = 166;
}
