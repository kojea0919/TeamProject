// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/TaggerCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFrameWork/MainMap/MainMapGameMode.h"
#include "GameFrameWork/MainMap/MainMapGameState.h"
#include "GameFrameWork/MainMap/MainMapPlayerController.h"
#include "Player/Character/Input/STEnhancedInputComponent.h"
#include "Player/Character/Input/STInputConfig.h"
#include "GameTag/STGamePlayTags.h"
#include "Player/Character/PlayerState/STPlayerState.h"
#include "Player/Character/Component/Repel/TaggerRepelComponent.h"


ATaggerCharacter::ATaggerCharacter()
{
	
	// 캡슐컴포넌트 초기사이즈 세팅
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// 회전사용 비활성화
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;


	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate

	// 캐릭터 무브먼트 세팅
	GetCharacterMovement()->JumpZVelocity = 450.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// 스프링암 초기세팅
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character
	CameraBoom->SocketOffset = FVector(0.0f, 55.0f, 65.f);
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(CameraBoom);

	// 카메라 초기세팅
	// FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	// FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	// FollowCamera->bUsePawnControlRotation = false;

	// Mesh 세팅
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/_GamePlay/Player/Tagger/MuscleCat/Meshes/SM_Muscle_Cat.SM_Muscle_Cat"));

	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	}
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f),FRotator(0.0f, -90.0f, 0.0f));

	TaggerRepelComponent = CreateDefaultSubobject<UTaggerRepelComponent>(TEXT("TaggerRepelComponent"));
	
	GetMesh()->bOwnerNoSee = false;
	GetMesh()->bOnlyOwnerSee = false;
	GetMesh()->SetVisibility(true, true);
	GetMesh()->SetHiddenInGame(false);
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}

void ATaggerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ATaggerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ATaggerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void ATaggerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as Input Config"));

	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem);
	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext,0);

	USTEnhancedInputComponent* STInputComponent = CastChecked<USTEnhancedInputComponent>(PlayerInputComponent);
	STInputComponent->BindNativeInputAction(InputConfigDataAsset, STGamePlayTags::Input_Move, ETriggerEvent::Triggered,
		this, &ATaggerCharacter::Input_Move);
	STInputComponent->BindNativeInputAction(InputConfigDataAsset, STGamePlayTags::Input_Look, ETriggerEvent::Triggered,
		this, &ATaggerCharacter::Input_Look);
	STInputComponent->BindNativeInputAction(InputConfigDataAsset, STGamePlayTags::Input_Jump, ETriggerEvent::Started,
		this, &ATaggerCharacter::Input_Jump);
	STInputComponent->BindNativeInputAction(InputConfigDataAsset, STGamePlayTags::Input_Jump, ETriggerEvent::Completed,
		this, &ATaggerCharacter::Input_StopJump);
}

void ATaggerCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	if (MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardDirection, MovementVector.Y);
	}

	if (MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(RightDirection, MovementVector.X);
	}
	FString RoleString;
}

void ATaggerCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}

	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATaggerCharacter::Input_Jump(const FInputActionValue& InputActionValue)
{
	Jump();
}

void ATaggerCharacter::Input_StopJump(const FInputActionValue& InputActionValue)
{
	StopJumping();
}

void ATaggerCharacter::Input_Crouch(const FInputActionValue& InputActionValue)
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else if (CanCrouch())
	{
		Crouch();
	}
}

bool ATaggerCharacter::CanCrouch() const
{
	return !bIsCrouched;
}


URepelComponent* ATaggerCharacter::GetRepelComponent() const
{
	return TaggerRepelComponent;
}
