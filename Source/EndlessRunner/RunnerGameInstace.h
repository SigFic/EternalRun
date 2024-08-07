// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RunnerGameInstace.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API URunnerGameInstace : public UGameInstance
{
	GENERATED_BODY()
public:

	URunnerGameInstace()
	{
		MaxXP = 0;
		MaxGold = 0;
		MaxTime = 0;
		TotalGold = 500;
		MagnetTime = 5;
		XPDoublerTime = 5;
		LifeShieldTime = 5;
		MaxMagnetTime = 15;
		MaxXPDoublerTime = 15;
		MaxLifeShieldTime = 15;
		MagnetBuyValue = 166.f;
		ShieldBuyValue = 166.f;
		XPDoublerBuyValue = 166.f;
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save", meta = (AllowPrivateAccess = "true"))
	float MagnetTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save", meta = (AllowPrivateAccess = "true"))
	float XPDoublerTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save", meta = (AllowPrivateAccess = "true"))
	float LifeShieldTime;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save", meta = (AllowPrivateAccess = "true"))
	float MaxXP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save", meta = (AllowPrivateAccess = "true"))
	float MaxGold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save", meta = (AllowPrivateAccess = "true"))
	float MaxTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save", meta = (AllowPrivateAccess = "true"))
	float TotalGold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save", meta = (AllowPrivateAccess = "true"))
	float MaxMagnetTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save", meta = (AllowPrivateAccess = "true"))
	float MaxXPDoublerTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save", meta = (AllowPrivateAccess = "true"))
	float MaxLifeShieldTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save", meta = (AllowPrivateAccess = "true"))
	float MagnetBuyValue = 166.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save", meta = (AllowPrivateAccess = "true"))
	float ShieldBuyValue = 166.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save", meta = (AllowPrivateAccess = "true"))
	float XPDoublerBuyValue = 166.f;

private:
	/*Saving*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USaveGame> SaveClass;

	USaveGame* SaveRef = nullptr;

	float ItemValueManipulator = 500.f;

public:
	UFUNCTION(BlueprintCallable)
	bool SetMaxXp(float Value)
	{
		if (Value > MaxXP)
		{
			MaxXP = Value;
			return true;
		}
		return false;
	}

	UFUNCTION(BlueprintCallable)
	bool SetMaxGold(float Value)
	{
		if (Value > MaxGold)
		{
			MaxGold = Value;
			return true;
		}
		return false;
	}

	UFUNCTION(BlueprintCallable)
	bool SetMaxTime(float Value)
	{
		if (Value > MaxTime)
		{
			MaxTime = Value;
			return true;
		}
		return false;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxXP()
	{
		return MaxXP;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaXGold()
	{
		return MaxGold;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxTime()
	{
		return MaxTime;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetTotalGold()
	{
		return TotalGold;
	}

	UFUNCTION(BlueprintCallable)
	bool SetTotalGold(float GoldValue)
	{
		if (GoldValue + TotalGold >= 0)
		{
			TotalGold += GoldValue;
			return true;
		}
		return false;
	}

	UFUNCTION(BlueprintCallable)
	void IncreaseMagnetTime()
	{
		if (MagnetTime < MaxMagnetTime && SetTotalGold(-MagnetBuyValue))
		{
			MagnetTime += 1.f;
			MagnetBuyValue = (MagnetTime / MaxMagnetTime) * ItemValueManipulator;
		}
	}

	UFUNCTION(BlueprintCallable)
	void IncreaseShieldTime()
	{
		if (LifeShieldTime < MaxLifeShieldTime && SetTotalGold(-ShieldBuyValue))
		{
			LifeShieldTime += 1.f;
			ShieldBuyValue = (LifeShieldTime / MaxLifeShieldTime) * ItemValueManipulator;
		}
	}

	UFUNCTION(BlueprintCallable)
	void IncreaseXPDoublerTime()
	{
		if (XPDoublerTime < MaxXPDoublerTime && SetTotalGold(-XPDoublerBuyValue))
		{
			XPDoublerTime += 1.f;
			XPDoublerBuyValue = (XPDoublerTime / MaxXPDoublerTime) * ItemValueManipulator;
		}
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMagnetTime()
	{
		return MagnetTime;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetShieldTime()
	{
		return LifeShieldTime;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetXPDoublerTime()
	{
		return XPDoublerTime;
	}

	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame();

	UFUNCTION(BlueprintCallable)
	void ResetAllProperties();
};
