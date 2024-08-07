// Copyright Epic Games, Inc. All Rights Reserved.

#include "EndlessRunnerCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/BoxComponent.h"
#include "Barrier.h"
#include "Coins.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "EndlessRunner/EndlessRunnerGameMode.h"
#include "Floor.h"
#include "Magnet.h"
#include "LifeShield.h"
#include "XPDoubler.h"
#include "RunnerGameInstace.h"



DEFINE_LOG_CATEGORY(LogTemplateCharacter);

/*Default Functions*/

void AEndlessRunnerCharacter::DestroyMagnet()
{
	if (Magnet && MagnetTimeCounter <= 0)
	{
		Magnet->Destroy();
		Magnet = nullptr;
		OnMagnetDestroyedbyTime.Broadcast();
	}
}

void AEndlessRunnerCharacter::DestroyShield()
{
	if ((Shield && ShieldTimeCounter <= 0))
	{
		Shield->Destroy();
		Shield = nullptr;
		OnShieldDestroyedbyTime.Broadcast();
	}
}

void AEndlessRunnerCharacter::DestroyXPDoubler()
{
	if (XPDoublerTimeCounter <= 0)
	{
		OnXPDoublerDestroyedbyTime.Broadcast();
	}
}

void AEndlessRunnerCharacter::IncreaseSpeed()
{
	if (GetCharacterMovement()->MaxWalkSpeed < MaximumSpeed) GetCharacterMovement()->MaxWalkSpeed += SpeedIncreaseAmount;
}

AEndlessRunnerCharacter::AEndlessRunnerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	CharacterCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision For Character State"));
	CharacterCollision->SetupAttachment(RootComponent);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AEndlessRunnerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AEndlessRunnerCharacter::JumpUp);

		//sliding
		EnhancedInputComponent->BindAction(SlideAction, ETriggerEvent::Triggered, this, &AEndlessRunnerCharacter::Slide);

		//Move left - right
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &AEndlessRunnerCharacter::MoveRight);
		EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Triggered, this, &AEndlessRunnerCharacter::MoveLeft);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AEndlessRunnerCharacter::BeginPlay()
{

	// Call the base class  
	Super::BeginPlay();

	bIsDead = false;
	bIsBarrierWall = false;

	GameInstance = Cast<URunnerGameInstace>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->LoadGame();
		MagnetTime = GameInstance->GetMagnetTime();
		ShieldTime = GameInstance->GetShieldTime();
		XPDoublerTime = GameInstance->GetXPDoublerTime();
		UE_LOG(LogTemp, Warning, TEXT("Max XP : %f"), GameInstance->GetMaxXP());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Instance is not valid!"));
	}

	RunnerLane = ECharacterLane::CL_Center;
	RunnerDirection = ECharacterDirection::CD_Forward;

	LanePositions.Add(ECharacterLane::CL_Center, GetActorLocation().Y);
	LanePositions.Add(ECharacterLane::CL_Left, GetActorLocation().Y - LanePosition);
	LanePositions.Add(ECharacterLane::CL_Right, GetActorLocation().Y + LanePosition);

	AEndlessRunnerGameMode* GameModeRef = Cast<AEndlessRunnerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameModeRef && GameModeRef->bIsFloorInitiliazed) CalculateLanePositions(GameModeRef->GetFirstFloor()->GetLanePositionPointLocation(), LanePosition);
	
	else CalculateLanePositions(GetActorLocation(), LanePosition);

	CharacterCollision->OnComponentBeginOverlap.AddDynamic(this, &AEndlessRunnerCharacter::OnBoxOverlap);

	if (PointWidgetClass)
	{
		PointWidgetRef = CreateWidget<UUserWidget>(GetWorld(), PointWidgetClass);
		if (PointWidgetRef) PointWidgetRef->AddToViewport();
	}
	Mana = MaximumMana;

	GetWorldTimerManager().SetTimer(ManaTimerHandle, this, &AEndlessRunnerCharacter::SecondFunction, 1.f, true);

	CurrentExperience = 0;
	DesiredExperience = 0;

}

void AEndlessRunnerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//FVector Direction = GetActorForwardVector() + GetActorLocation();
	//Direction.Normalize();
	/*AddMovementInput(GetActorForwardVector());*/
	if (bIsDead) return;

	MoveForward(DeltaTime, RunSpeed);

	CurrentExperience = FMath::FInterpTo<int>(CurrentExperience, DesiredExperience, DeltaTime, 2.5f);

	if (bIsLerping)
	{
		FVector NewLocation = GetActorLocation();

		if (RunnerDirection == ECharacterDirection::CD_Forward || RunnerDirection == ECharacterDirection::CD_BackWard)
		{
			NewLocation.Y = FMath::FInterpTo(NewLocation.Y, EndLocation.Y, DeltaTime, LerpTime);
		}

		else
		{
			NewLocation.X = FMath::FInterpTo(NewLocation.X, EndLocation.X, DeltaTime, LerpTime);
		}
		
		SetActorLocation(NewLocation);
	}
	
}

/*CharacterStateFunctions*/
void AEndlessRunnerCharacter::SetCharacterState(ECharacterState State)
{
	switch (State)
	{
	case ECharacterState::CS_RunningState:
		CharacterCollision->SetWorldLocation(this->GetActorLocation() + FVector(0.f,0.f,0.f));
		break;
	case ECharacterState::CS_JumpingState:
		CharacterCollision->SetWorldLocation(this->GetActorLocation() + FVector(0.f, 0.f, 35.f));
		break;
	case ECharacterState::CS_SlidingState:
		CharacterCollision->SetWorldLocation(this->GetActorLocation() + FVector(0.f, 0.f, -70.f));
		break;
	default:
		break;
	}
}

ECharacterState AEndlessRunnerCharacter::GetCharacterState()
{
	return RunnerState;
}

void AEndlessRunnerCharacter::ChangeCharacterState(ECharacterState State)
{
	RunnerState = State;
	SetCharacterState(RunnerState);
}

/*Movement Functions*/
void AEndlessRunnerCharacter::MoveLeft()
{
	if (bCanTurnLeft)
	{
		ChangeCharacterDirection(false);
		bCanTurnLeft = false;
		return;
	}

	if (RunnerLane == ECharacterLane::CL_Center)
	{
		ChangeLane(ECharacterLane::CL_Left);
	}

	else if (RunnerLane == ECharacterLane::CL_Right)
	{
		ChangeLane(ECharacterLane::CL_Center);
	}
}

void AEndlessRunnerCharacter::MoveRight()
{
	if (bCanTurnRight)
	{
		ChangeCharacterDirection(true);
		bCanTurnRight = false;
		return;
	}

	if (RunnerLane == ECharacterLane::CL_Center)
	{
		ChangeLane(ECharacterLane::CL_Right);
	}

	else if (RunnerLane == ECharacterLane::CL_Left)
	{
		ChangeLane(ECharacterLane::CL_Center);
	}
}

void AEndlessRunnerCharacter::Slide()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && bIsSlidingEnd)
	{
		ChangeCharacterState(ECharacterState::CS_SlidingState);
		bIsSlidingEnd = false;
		AnimInstance->Montage_Play(SlideMontage);
		AnimInstance->Montage_JumpToSection(FName("Sliding"));
	}
}

void AEndlessRunnerCharacter::JumpUp()
{
	if (bIsSlidingEnd && RunnerState == ECharacterState::CS_RunningState)
	{
		Jump();
		ChangeCharacterState(ECharacterState::CS_JumpingState);
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(JumpMontage);
			AnimInstance->Montage_JumpToSection(FName("JumpSection"));
		}
	}

}

void AEndlessRunnerCharacter::MoveForward(float DeltaTime, const float& Speed)
{
	if (Controller != nullptr && (Speed != 0.f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X) };
		AddMovementInput(Direction, Speed * DeltaTime);
	}
}

/*Mana Functions*/
void AEndlessRunnerCharacter::SecondFunction()
{
	if (bIsDead) return;
	//DesiredExperience = CurrentExperience + GetCharacterMovement()->MaxWalkSpeed;
	CalculateDesiredExperience();
	--MagnetTimeCounter;
	--ShieldTimeCounter;
	--XPDoublerTimeCounter;
	Time++;

	this->Mana -= 1;
	if (this->Mana <= 0) DeathRunnerChar();

}

void AEndlessRunnerCharacter::IncreaseMana()
{
	if (ManaIncreaseAmount < 0) return;

	this->Mana += ManaIncreaseAmount;
	if (this->Mana > MaximumMana) this->Mana = MaximumMana;
}

/*Character Direction Functions*/
/*@Direction false means left , true means right*/
void AEndlessRunnerCharacter::ChangeCharacterDirection(bool Direction)
{ 
	switch (RunnerDirection)
	{
	case ECharacterDirection::CD_Left:
		if (Direction) RunnerDirection = ECharacterDirection::CD_Forward;
		else RunnerDirection = ECharacterDirection::CD_BackWard;
		SetCharacterDirectionProperties(RunnerDirection, Direction);
		break;
	case ECharacterDirection::CD_Right:
		if (Direction) RunnerDirection = ECharacterDirection::CD_BackWard;
		else RunnerDirection = ECharacterDirection::CD_Forward;
		SetCharacterDirectionProperties(RunnerDirection, Direction);
		break;
	case ECharacterDirection::CD_Forward:
		if (Direction) RunnerDirection = ECharacterDirection::CD_Right;
		else RunnerDirection = ECharacterDirection::CD_Left;
		SetCharacterDirectionProperties(RunnerDirection, Direction);
		break;
	case ECharacterDirection::CD_BackWard:
		if (Direction) RunnerDirection = ECharacterDirection::CD_Left;
		else RunnerDirection = ECharacterDirection::CD_Right;
		SetCharacterDirectionProperties(RunnerDirection, Direction);
		break;
	default:
		break;
	}
}

void AEndlessRunnerCharacter::SetCharacterDirectionProperties(ECharacterDirection Direction, bool TurnWay)
{
	FRotator NewRotation = GetActorRotation();
	//FRotator NewRotation2 = GetControlRotation();

	if (TurnWay) // Saða dönme
	{
		NewRotation.Yaw += 90.f;
	}
	else // Sola dönme
	{
		NewRotation.Yaw -= 90.f; 
	}

	NewRotation.Normalize();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController) PlayerController->SetControlRotation(NewRotation);

	CalculateLanePositions(CalculatedLocation, LanePosition);
	FindLaneOnCorners();
	ChangeLane(RunnerLane);
}

void AEndlessRunnerCharacter::FindLaneOnCorners()
{
	if (RunnerDirection == ECharacterDirection::CD_Forward || RunnerDirection == ECharacterDirection::CD_BackWard)
	{
		float ActorYLocation = GetActorLocation().Y;
		float MinDistance = TNumericLimits<float>::Max();
		ECharacterLane NearestLane = ECharacterLane::CL_Center;

		for (auto& Elem : LanePositions)
		{
			float LaneYLocation = Elem.Value;
			float Distance = FMath::Abs(ActorYLocation - LaneYLocation);

			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				NearestLane = Elem.Key;
			}
		}

		RunnerLane = NearestLane;
	}
	else if (RunnerDirection == ECharacterDirection::CD_Left || RunnerDirection == ECharacterDirection::CD_Right)
	{
		float ActorXLocation = GetActorLocation().X;
		float MinDistance = TNumericLimits<float>::Max();
		ECharacterLane NearestLane = ECharacterLane::CL_Center;

		for (auto& Elem : LanePositions)
		{
			float LaneXLocation = Elem.Value;
			float Distance = FMath::Abs(ActorXLocation - LaneXLocation);

			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				NearestLane = Elem.Key;
			}
		}

		RunnerLane = NearestLane;
	}
}

void AEndlessRunnerCharacter::CalculateDesiredExperience()
{
	if (XPDoublerTimeCounter <= 0)
	{
		DesiredExperience = CurrentExperience + GetCharacterMovement()->MaxWalkSpeed + XPManipulator;
	}
	else
	{
		DesiredExperience = CurrentExperience + ((GetCharacterMovement()->MaxWalkSpeed + XPManipulator)* 2);
	}
}

void AEndlessRunnerCharacter::CalculateLanePositions(FVector Location, float Distance)
{
	//this->SetActorLocation(Location);
	CalculatedLocation = Location;
	switch (RunnerDirection)
	{
	case ECharacterDirection::CD_Left:
	{
		LanePositions[ECharacterLane::CL_Center] = Location.X;
		LanePositions[ECharacterLane::CL_Right] = Location.X + Distance;
		LanePositions[ECharacterLane::CL_Left] = Location.X - Distance;
		break;
	}

	case ECharacterDirection::CD_Right:
	{
		LanePositions[ECharacterLane::CL_Center] = Location.X;
		LanePositions[ECharacterLane::CL_Right] = Location.X - Distance;
		LanePositions[ECharacterLane::CL_Left] = Location.X + Distance;
		break;
	}
	case ECharacterDirection::CD_Forward:
	{
		LanePositions[ECharacterLane::CL_Center] = Location.Y;
		LanePositions[ECharacterLane::CL_Right] = Location.Y + Distance;
		LanePositions[ECharacterLane::CL_Left] = Location.Y - Distance;
		break;
	}
	case ECharacterDirection::CD_BackWard:
	{
		LanePositions[ECharacterLane::CL_Center] = Location.Y;
		LanePositions[ECharacterLane::CL_Right] = Location.Y - Distance;
		LanePositions[ECharacterLane::CL_Left] = Location.Y + Distance;
		break;
	}
	default:
		break;
	}

}

/*Character lane Functions*/
void AEndlessRunnerCharacter::ChangeLane(ECharacterLane Lane)
{
	RunnerLane = Lane;
	FVector RunnerLocation = GetActorLocation();

	switch (Lane)
	{
	case ECharacterLane::CL_Left:
		if (RunnerDirection == ECharacterDirection::CD_BackWard ||RunnerDirection == ECharacterDirection::CD_Forward)
		{
			RunnerLocation.Y = LanePositions[ECharacterLane::CL_Left];
		}
		
		else RunnerLocation.X = LanePositions[ECharacterLane::CL_Left];
		break;
	case ECharacterLane::CL_Right:
		if (RunnerDirection == ECharacterDirection::CD_BackWard || RunnerDirection == ECharacterDirection::CD_Forward)
		{
			RunnerLocation.Y = LanePositions[ECharacterLane::CL_Right];
		}

		else RunnerLocation.X = LanePositions[ECharacterLane::CL_Right];

		break;
	case ECharacterLane::CL_Center:
		if (RunnerDirection == ECharacterDirection::CD_BackWard || RunnerDirection == ECharacterDirection::CD_Forward)
		{
			RunnerLocation.Y = LanePositions[ECharacterLane::CL_Center];
		}

		else RunnerLocation.X = LanePositions[ECharacterLane::CL_Center];

		break;
	default:
		break;
	}

	StartInterpolation();
	//SetActorLocation(RunnerLocation);
}

void AEndlessRunnerCharacter::StartInterpolation()
{
	bIsLerping = true;
	if (RunnerDirection == ECharacterDirection::CD_BackWard || RunnerDirection == ECharacterDirection::CD_Forward)
	{
		EndLocation.Y = LanePositions[RunnerLane];
		//EndLocation.Y
	}

	else EndLocation.X = LanePositions[RunnerLane];
}
/*Others*/

void AEndlessRunnerCharacter::DeathRunnerChar()
{
	//GetCharacterMovement()->MaxWalkSpeed = 0.f;

	if (Shield && !bIsBarrierWall)
	{
		UE_LOG(LogTemp, Warning, TEXT("Shield Death calismadi"));
		Shield->ShieldUseSound();
		Shield->Destroy();
		OnShieldDestroyedbyTime.Broadcast();
		ShieldTimeCounter = -1.f;
		Shield = nullptr;
		return;
	}

	bIsDead = true;

	if (HittingSound)	UGameplayStatics::PlaySoundAtLocation(this, HittingSound, this->GetActorLocation());

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (GameInstance)
	{
		GameInstance->SetMaxXp(CurrentExperience);
		GameInstance->SetMaxGold(Point);
		GameInstance->SetMaxTime(Time);
		GameInstance->SetTotalGold(Point);
		GameInstance->SaveGame();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Instance is not valid!"));
	}

	if (EndWidgetClass)
	{
		EndWidgetRef = CreateWidget<UUserWidget>(GetWorld(), EndWidgetClass);
		if (EndWidgetRef) EndWidgetRef->AddToViewport();
	}

}

void AEndlessRunnerCharacter::OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	UStaticMeshComponent* MeshRef = Cast<UStaticMeshComponent>(OtherComp);

	ABarrier* BarrierRef = Cast<ABarrier>(OtherActor);
	if (BarrierRef)
	{
		DeathRunnerChar();
	}

	ACoins* CoinRef = Cast<ACoins>(OtherActor);

	GetCoin(CoinRef);
	if (CoinRef) CoinRef->Destroy();

	AItems* ItemRef = Cast<AItems>(OtherActor);
	if (ItemRef && GetCurrenFloor())
	{
		for (int i = 0; i < 6; i++)
		{
			if (GetCurrenFloor()->ItemArray[i] == ItemRef)
			{
				ItemRef->PlayPickUpSound();
				GetCurrenFloor()->ItemArray[i] = nullptr;
			}
		}
	}

	AMagnet* MagnetRef = Cast<AMagnet>(OtherActor);

	if (MagnetRef)
	{
		if (Magnet != nullptr) 
		{
			Magnet->Destroy();
			Magnet = nullptr;
			//Magnet = MagnetRef;
		}
		MagnetRef->SetOwner(this);
		Magnet = MagnetRef;
		if (MagnetTimeCounter <= 0)
		{
			OnMagnetTaken.Broadcast();
		}
		MagnetTimeCounter = MagnetTime;

		MagnetRef->SetSphereCollisionSettings(true);

		MagnetRef->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("MagnetSocket"));
		
		FTimerHandle ItemTimer;
		GetWorldTimerManager().SetTimer(ItemTimer, this, &AEndlessRunnerCharacter::DestroyMagnet, MagnetTime, false);
	}

	ALifeShield* ShieldRef = Cast<ALifeShield>(OtherActor);

	if (ShieldRef)
	{
		UE_LOG(LogTemp, Error, TEXT("overlapped"));
		if (Shield != nullptr)
		{
			Shield->Destroy();
			Shield = nullptr;
			//Shield = ShieldRef;
		}
		Shield = ShieldRef;
		ShieldRef->SetOwner(this);
		if (ShieldTimeCounter <= 0)
		{
			OnShieldtaken.Broadcast();
		}
		ShieldRef->OnShieldTaken();

		ShieldTimeCounter = ShieldTime;

		ShieldRef->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("LifeShieldSocket"));

		FTimerHandle ItemTimer;
		GetWorldTimerManager().SetTimer(ItemTimer, this, &AEndlessRunnerCharacter::DestroyShield, ShieldTime, false);
	}

	AXPDoubler* XPDoublerRef = Cast<AXPDoubler>(OtherActor);

	if (XPDoublerRef)
	{
		XPDoublerRef->SetOwner(this);
		XPDoublerRef->Destroy();
		//XPDoubler = XPDoublerRef;
		if (XPDoublerTimeCounter <= 0)
		{
			OnXPDoublertaken.Broadcast();
		}
		XPDoublerTimeCounter = XPDoublerTime;

		FTimerHandle ItemTimer;
		GetWorldTimerManager().SetTimer(ItemTimer, this, &AEndlessRunnerCharacter::DestroyXPDoubler, XPDoublerTime, false);
	}
}

void AEndlessRunnerCharacter::GetCoin(ACoins* Coin)
{
	if (Coin)
	{
		Coin->PlayPickUpSound();
		IncreaseMana();
		
		if (GetCurrenFloor())
		{
			for (int i = 0; i < 6; i++)
			{
				if (GetCurrenFloor()->CoinsArray[i] == Coin)
				{
					GetCurrenFloor()->CoinsArray[i] = nullptr;
				}
			}
		}
		IncreasePoint();
		IncreaseSpeed();

	}
}







