// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "EndlessRunnerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMagnetDelagate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShieldDelagate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FXPDoublerDelagate);

UENUM(BlueprintType)
enum class ECharacterLane : uint8
{
	CL_Left UMETA(DisplayName = "Left"),
	CL_Right UMETA(DisplayName = "Right"),
	CL_Center UMETA(DisplayName = "Center"),
};

UENUM(BlueprintType)
enum class ECharacterDirection : uint8
{
	CD_Left UMETA(DisplayName = "Left"),
	CD_Right UMETA(DisplayName = "Right"),
	CD_Forward UMETA(DisplayName = "Forward"),
	CD_BackWard UMETA(DisplayName = "Backward"),
};

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	CS_RunningState UMETA(DisplayName = "Running State"),
	CS_JumpingState UMETA(DisplayName = "Jumping State"),
	CS_SlidingState UMETA(DisplayName = "Sliding State")
};


DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AEndlessRunnerCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	/*variables with UPROPERTY*/
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveLeftAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveRightAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SlideAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	float LanePosition;

	// Interpolation properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	bool bIsLerping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	float LerpTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ENUM", meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterLane, float> LanePositions;

	//Anim Montages
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* JumpMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* SlideMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* RightJumpMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* LeftJumpMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	bool bIsSlidingEnd = true;

	/*Character State*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ENUM", meta = (AllowPrivateAccess = "true"))
	ECharacterState RunnerState;

	/*StateChannel*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ENUM", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CharacterCollision;

	/*Coins*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coin", meta = (AllowPrivateAccess = "true"))
	int Point;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coin", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> PointWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coin", meta = (AllowPrivateAccess = "true"))
	UUserWidget* PointWidgetRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coin", meta = (AllowPrivateAccess = "true"))
	float PointIncreaseAmount;

	/*EndWidget*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coin", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> EndWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Coin", meta = (AllowPrivateAccess = "true"))
	UUserWidget* EndWidgetRef;

	/* Mana*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mana", meta = (AllowPrivateAccess = "true"))
	float Mana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana", meta = (AllowPrivateAccess = "true"))
	float MaximumMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana", meta = (AllowPrivateAccess = "true"))
	float ManaIncreaseAmount;

	/*Runner direction*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	ECharacterDirection RunnerDirection;

	/*Others*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	int RunSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SpeedIncreaseAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MaximumSpeed;

	/*Magnet*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items", meta = (AllowPrivateAccess = "true"))
	class AMagnet* Magnet = nullptr;

	/*Shield*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items", meta = (AllowPrivateAccess = "true"))
	class ALifeShield* Shield = nullptr;

	/*Experience - Distance*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items", meta = (AllowPrivateAccess = "true"))
	int CurrentExperience;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items", meta = (AllowPrivateAccess = "true"))
	int DesiredExperience;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items", meta = (AllowPrivateAccess = "true"))
	int Time;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items", meta = (AllowPrivateAccess = "true"))
	float XPManipulator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death", meta = (AllowPrivateAccess = "true"))
	bool bIsBarrierWall;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	class USoundBase* HittingSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Corners", meta = (AllowPrivateAccess = "true"))
	bool bCanTurnLeft;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Corners", meta = (AllowPrivateAccess = "true"))
	bool bCanTurnRight;


private:
	/*Variables With No UPROPERTIES*/
	FVector StartLocation;

	FVector EndLocation;
	
	float CharacterCollisionManipulator;
	
	FTimerHandle ManaTimerHandle;

	bool bIsDead;

	float XPDoublerTime;

	float ShieldTime;

	float MagnetTime;

	float ShieldTimeCounter;

	float MagnetTimeCounter;

	float XPDoublerTimeCounter;

	class URunnerGameInstace* GameInstance;

private:
	/*Functions*/
	void SetCharacterState(ECharacterState State);

	void SetCharacterDirectionProperties(ECharacterDirection Direction, bool TurnWay);

	void CalculateDesiredExperience();

	void FindLaneOnCorners();
protected:
	/*Function without UFunction*/
	void MoveLeft();

	void MoveRight();

	void StartInterpolation();

	void Slide();

	void JumpUp();

	void MoveForward(float DeltaTime, const float& Speed);

	void ChangeLane(ECharacterLane Lane);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void SecondFunction();

	void DestroyMagnet();

	void DestroyShield();

	void DestroyXPDoubler();

	FORCEINLINE void IncreasePoint() { Point += PointIncreaseAmount; }

	void IncreaseSpeed();

protected:

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void DeathRunnerChar();
protected:

	FVector CalculatedLocation;

public:
	/*Variables*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	ECharacterLane RunnerLane;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	class AFloor* CurrentFlorr;

	/*Delagates*/
	UPROPERTY(BlueprintAssignable)
	FMagnetDelagate OnMagnetTaken;

	/*timer counter 0 olup magnet yok oldugunda ortaya cýkar*/
	UPROPERTY(BlueprintAssignable)
	FMagnetDelagate OnMagnetDestroyedbyTime;

	UPROPERTY(BlueprintAssignable)
	FShieldDelagate OnShieldtaken;

	UPROPERTY(BlueprintAssignable)
	FShieldDelagate OnShieldDestroyedbyTime;

	UPROPERTY(BlueprintAssignable)
	FXPDoublerDelagate OnXPDoublertaken;

	UPROPERTY(BlueprintAssignable)
	FXPDoublerDelagate OnXPDoublerDestroyedbyTime;

public:
	AEndlessRunnerCharacter();

	void IncreaseMana();

	void GetCoin(class ACoins* Coin);

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE AFloor* GetCurrenFloor() { return CurrentFlorr; }
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetbIsSlidingEnded() { return bIsSlidingEnd; }

	UFUNCTION(BlueprintCallable)
	void SetbIsSlidingEnded(bool Value) { bIsSlidingEnd = Value; }

	UFUNCTION(BlueprintCallable)
	ECharacterState GetCharacterState();

	UFUNCTION(BlueprintCallable)
	void ChangeCharacterState(ECharacterState State);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int& GetCoinsPoint() { return Point; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetManaPercent() const { return Mana / MaximumMana; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetMagnetTimePercent() const { return MagnetTimeCounter / MagnetTime; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetShieldTimePercent() const { return ShieldTimeCounter / ShieldTime; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE float GetXPDoublerTimePercent() const { return XPDoublerTimeCounter / XPDoublerTime; }

	/*false means left, true means right*/
	UFUNCTION(BlueprintCallable)
	void ChangeCharacterDirection(bool Direction);

	UFUNCTION(BlueprintCallable)
	void CalculateLanePositions(FVector Location, float Distance);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int GetExperience() { return CurrentExperience; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int GetTime() { return Time; }


	friend class AFloor;

};

