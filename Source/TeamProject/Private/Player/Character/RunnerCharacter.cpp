// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Character/RunnerCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFrameWork/MainMap/MainMapGameMode.h"
#include "GameFrameWork/MainMap/MainMapGameState.h"
#include "GameFrameWork/MainMap/MainMapPlayerController.h"
#include "Player/Character/Input/STEnhancedInputComponent.h"
#include "Player/Character/Input/STInputConfig.h"
#include "GameTag/STGamePlayTags.h"
#include "Player/Character/Component/STExtensionComponent.h"
#include "Player/Character/Component/Repel/RunnerRepelComponent.h"
#include "Player/Character/Component/Interactive/RunnerInterActiveComponent.h"
#include "Net/UnrealNetwork.h"
#include "Player/Character/PlayerState/STPlayerState.h"

ARunnerCharacter::ARunnerCharacter()
{
	// 캡슐컴포넌트 초기사이즈 세팅
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// 회전사용 비활성화
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;


	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// 캐릭터 무브먼트 세팅
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
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
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/_GamePlay/Player/SHIROKO/SHIROKO_SkeletaMesh.SHIROKO_SkeletaMesh"));

	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	}
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f),FRotator(0.0f, -90.0f, 0.0f));

	// RePelComponent
	RunnerRepelComponent = CreateDefaultSubobject<URunnerRepelComponent>(TEXT("RunnerRepelComponent"));

	// InterActiveComponent
	RunnerInterActiveComponent = CreateDefaultSubobject<URunnerInterActiveComponent>(TEXT("RunnerInterActiveComponent"));

	//StaticMeshComponent
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMesh->SetupAttachment(GetCapsuleComponent());
	StaticMesh->SetRelativeLocation(FVector(0.0f,0.0f,-85.0f));
	StaticMesh->SetIsReplicated(true);

	GetMesh()->bOwnerNoSee = false;
	GetMesh()->bOnlyOwnerSee = false;
	GetMesh()->SetVisibility(true, true);
	GetMesh()->SetHiddenInGame(false);
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}

void ARunnerCharacter::SetCurrentObjectType_Implementation(EStaticMeshType MeshType)
{	
	CurrentObjectType = MeshType;
	
	if (HasAuthority())
	{
		OnRep_ObjectType();
	}
}

void ARunnerCharacter::OnRep_ObjectType()
{
	if (!IsValid(StaticMesh))
	{
		return;
	}
	
	if (CurrentObjectType != EStaticMeshType::None)
	{
		if (AMainMapGameState * GameState = GetWorld()->GetGameState<AMainMapGameState>())
		{
			FStaticMeshInfo MeshInfo = GameState->GetObjectMesh(CurrentObjectType);
			if (UStaticMesh * TargetMesh = MeshInfo.Mesh)
			{
				StaticMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
				StaticMesh->SetHiddenInGame(false);
				StaticMesh->SetStaticMesh(TargetMesh);
				StaticMesh->SetRelativeLocation(FVector(0.0f,0.0f,MeshInfo.ZHeight));
				GetCapsuleComponent()->SetCapsuleRadius(5.f);
				GetCapsuleComponent()->SetCapsuleHalfHeight(5.f);
				GetCapsuleComponent()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_Yes;
				GetMesh()->SetHiddenInGame(true);
			}
		}
	}
	else
	{
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		StaticMesh->SetHiddenInGame(true);
		StaticMesh->SetStaticMesh(nullptr);
		GetCapsuleComponent()->SetCapsuleHalfHeight(88.f);
		GetCapsuleComponent()->SetCapsuleRadius(42.f);
		GetCapsuleComponent()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
		GetMesh()->SetHiddenInGame(false);
	}
}

void ARunnerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARunnerCharacter, CurrentObjectType);
}

void ARunnerCharacter::SetGhostMode_Implementation()
{	
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("GhostMode"));
	StaticMesh->SetHiddenInGame(true);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	GetCapsuleComponent()->SetCapsuleRadius(42.f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(88.f);
	GetCapsuleComponent()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	if (IsLocallyControlled())
	{
		GetMesh()->SetHiddenInGame(false);
	}
}

void ARunnerCharacter::SetOutLine_Implementation(const TArray<ARunnerCharacter*>& OutlineTargets, bool Active)
{
	for (const auto & RunnerCharacter : OutlineTargets)
	{
		if (!IsValid(RunnerCharacter) ||RunnerCharacter == this)
			continue;

		if (nullptr != RunnerCharacter->StaticMesh)
		{
			RunnerCharacter->StaticMesh->SetRenderCustomDepth(Active);
			RunnerCharacter->StaticMesh->CustomDepthStencilValue = Active ? 1 : 0;
		}
	}
}

void ARunnerCharacter::Multicast_PlayDeathMontage_Implementation()
{
	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
}

void ARunnerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ARunnerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ARunnerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void ARunnerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as Input Config"));

	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem);
	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext,0);

	USTEnhancedInputComponent* STInputComponent = CastChecked<USTEnhancedInputComponent>(PlayerInputComponent);
	STInputComponent->BindNativeInputAction(InputConfigDataAsset, STGamePlayTags::Input_Move, ETriggerEvent::Triggered,
		this, &ARunnerCharacter::Input_Move);
	STInputComponent->BindNativeInputAction(InputConfigDataAsset, STGamePlayTags::Input_Look, ETriggerEvent::Triggered,
		this, &ARunnerCharacter::Input_Look);
	STInputComponent->BindNativeInputAction(InputConfigDataAsset, STGamePlayTags::Input_Jump, ETriggerEvent::Started,
		this, &ARunnerCharacter::Input_Jump);
	STInputComponent->BindNativeInputAction(InputConfigDataAsset, STGamePlayTags::Input_Jump, ETriggerEvent::Completed,
		this, &ARunnerCharacter::Input_StopJump);
	STInputComponent->BindNativeInputAction(InputConfigDataAsset, STGamePlayTags::Input_CameraModeChange, ETriggerEvent::Started,
		this, &ARunnerCharacter::Input_CameraMode);
	
}

void ARunnerCharacter::Input_Move(const FInputActionValue& InputActionValue)
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
}

void ARunnerCharacter::Input_Look(const FInputActionValue& InputActionValue)
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

void ARunnerCharacter::Input_Jump(const FInputActionValue& InputActionValue)
{
	Jump();
}

void ARunnerCharacter::Input_StopJump(const FInputActionValue& InputActionValue)
{
	StopJumping();
}

void ARunnerCharacter::Input_CameraMode(const FInputActionValue& InputActionValue)
{
	bIsCameraModeYawEnabled = !bIsCameraModeYawEnabled;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = bIsCameraModeYawEnabled;
}

URepelComponent* ARunnerCharacter::GetRepelComponent() const
{
	return RunnerRepelComponent;
}

UPawnInterActiveComponent* ARunnerCharacter::GetInterActiveComponent() const
{
	return RunnerInterActiveComponent;
}

void ARunnerCharacter::Test_Implementation()
{
	if (AMainMapPlayerController* PC = Cast<AMainMapPlayerController>(GetController()))
	{
		UE_LOG(LogTemp, Warning, TEXT("OnHammerHit"));
	}
}

