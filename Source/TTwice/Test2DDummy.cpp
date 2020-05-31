// Fill out your copyright notice in the Description page of Project Settings.


#include "Test2DDummy.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimeGhostComponent.h"
#include "Classes/PaperFlipbookComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "TimerManager.h"
#include "UserWidget.h"

ATest2DDummy::ATest2DDummy()
{
    // Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
    
    // Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(36.0f);
	GetCapsuleComponent()->SetCapsuleRadius(16.0f);
	GetCapsuleComponent()->SetEnableGravity(1);
	
    // Lock character motion onto the XY plane, so the character can't move in or out of the screen
    GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, 0.0f, -1.0f));
	GetCharacterMovement()->GravityScale = 4.0f;

	// Initialize movement
	GetCharacterMovement()->MaxWalkSpeed = 192.0f;
	GetCharacterMovement()->AirControl = 1;
	GetCharacterMovement()->BrakingDecelerationFalling = GetCharacterMovement()->MaxAcceleration;
	//Sprite Setting
	GetSprite()->SetRelativeRotation(FRotator(0.0f, 90.0f, -90.0f));
	GetSprite()->SetRelativeLocation(FVector(0.0f,-15.0f,0.0f));

	//Time Ghost Setting
	TimeGhost = CreateDefaultSubobject<UTimeGhostComponent>(TEXT("TimeGhost"));
	TimeGhost->GhostPath.AddDynamic(this, &ATest2DDummy::SetGhostLocation);
	//Ghost Flipbook Setting
	GhostFlipbook = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(TEXT("GhostSprite"));
	if (GhostFlipbook)
	{
		GhostFlipbook->AlwaysLoadOnClient = true;
		GhostFlipbook->AlwaysLoadOnServer = true;
		GhostFlipbook->bOwnerNoSee = false;
		GhostFlipbook->bAffectDynamicIndirectLighting = true;
		GhostFlipbook->PrimaryComponentTick.TickGroup = TG_EndPhysics;
		GhostFlipbook->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		static FName CollisionProfileName(TEXT("NoCollision"));
		GhostFlipbook->SetCollisionProfileName(CollisionProfileName);
		GhostFlipbook->SetGenerateOverlapEvents(true);
	}
	GhostFlipbook->SetRelativeLocation(FVector(0.0f,0.0f,-100.0f));

	//Ghost Collision setting
	GhostCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("GhostCollision"));
	GhostCollision->SetupAttachment(GhostFlipbook);
	GhostCollision->SetCapsuleHalfHeight(36.0f);
	GhostCollision->SetCapsuleRadius(16.0f);
	GhostCollision->SetCollisionProfileName("OverlapAllDynamic");
    
}

void ATest2DDummy::BeginPlay()
{
    Super::BeginPlay();
	if(GhostFlipbook != nullptr)
		GhostFlipbook->SetMobility(EComponentMobility::Movable);
	if(Anim_Spawn!=nullptr)
	{
		GetSprite()->SetFlipbook(Anim_Spawn);
	}	
}

void ATest2DDummy::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
	UpdateAnimation();
}

void ATest2DDummy::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	//PlayerInputComponent->BindAxis("MoveRight", this, &ADogSoulsCharacter::MoveRight);
	//PlayerInputComponent->BindTouch(IE_Released, this, &ADogSoulsCharacter::TouchEnded);
	//PlayerInputComponent->BindGesture(EKeys::Gesture_Flick, this, &ADogSoulsCharacter::TouchRepeat);
    PlayerInputComponent->BindAxis("Right", this, &ATest2DDummy::MoveRight);
    PlayerInputComponent->BindAxis("Up", this, &ATest2DDummy::MoveUp);
	PlayerInputComponent->BindAction("RestartLevel", IE_Pressed, this, &ATest2DDummy::TryAgain);
	PlayerInputComponent->BindAction("Menu", IE_Pressed, this, &ATest2DDummy::Menu);
}

void ATest2DDummy::MoveRight(float Value)
{
    AddMovementInput(FVector(1.0f, 0.0f, 0.0f),Value);

}

void ATest2DDummy::MoveUp(float Value)
{
	AddMovementInput(FVector(0.0f, 1.0f, 0.0f),-Value);
}

void ATest2DDummy::SetGhostLocation(UTimeGhostComponent* GhostComp, FTransform NewTransform, FVector GhostVelocity)
{
	if(GhostFlipbook == nullptr)
		return;
	NewTransform.SetScale3D(GetSprite()->GetComponentScale());
	NewTransform.SetRotation(GetSprite()->GetComponentRotation().Quaternion());
	GhostFlipbook->SetWorldTransform(NewTransform);
	GhostCurrentVelocity = GhostVelocity;

	//UE_LOG(LogTemp, Warning, TEXT("GhostVelocity : %s"),*GhostVelocity.ToString());
	
}
void ATest2DDummy::UpdateAnimation()
{
	FVector Vel = GetVelocity();
	if(Vel.Size() <= 9.0)
	{
		GetSprite()->SetFlipbook(Anim_Idle);
	}
	else
	{
		if(Vel.X > 10.0)
		{
			if(Vel.Y > 50.0)
			{
				GetSprite()->SetFlipbook(Anim_WalkRightDown);
			}
			else if(Vel.Y < -50.0)
			{
				GetSprite()->SetFlipbook(Anim_WalkRightUp);
			}
			else
			{
				GetSprite()->SetFlipbook(Anim_WalkRight);
			}
			
		}
		else if(Vel.X < -10.0)
		{
			if(Vel.Y > 50.0)
			{
				GetSprite()->SetFlipbook(Anim_WalkLeftDown);
			}
			else if(Vel.Y < -50.0)
			{
				GetSprite()->SetFlipbook(Anim_WalkLeftUp);
			}
			else
			{
				GetSprite()->SetFlipbook(Anim_WalkLeft);
			}	
		}
		else
		{
			//Positive of Y Axis is down direction of screen
			if(Vel.Y > 50.0)
			{
				GetSprite()->SetFlipbook(Anim_WalkDown);
			}
			else if(Vel.Y < -50.0)
			{
				GetSprite()->SetFlipbook(Anim_WalkUp);
			}
		}
		
	}
	//GhostAnimation
	if(GhostCurrentVelocity.Size() <= 9.0f)
	{
		GhostFlipbook->SetFlipbook(Anim_Idle);
	}
	else
	{
		if(GhostCurrentVelocity.X > 10.0)
		{
			if(GhostCurrentVelocity.Y > 50.0)
			{
				GhostFlipbook->SetFlipbook(Anim_WalkRightDown);
			}
			else if(GhostCurrentVelocity.Y < -50.0)
			{
				GhostFlipbook->SetFlipbook(Anim_WalkRightUp);
			}
			else
			{
				GhostFlipbook->SetFlipbook(Anim_WalkRight);
			}
			
		}
		else if(GhostCurrentVelocity.X < -10.0)
		{
			if(GhostCurrentVelocity.Y > 50.0)
			{
				GhostFlipbook->SetFlipbook(Anim_WalkLeftDown);
			}
			else if(GhostCurrentVelocity.Y < -50.0)
			{
				GhostFlipbook->SetFlipbook(Anim_WalkLeftUp);
			}
			else
			{
				GhostFlipbook->SetFlipbook(Anim_WalkLeft);
			}	
		}
		else
		{
			//Positive of Y Axis is down direction of screen
			if(GhostCurrentVelocity.Y > 50.0)
			{
				GhostFlipbook->SetFlipbook(Anim_WalkDown);
			}
			else if(GhostCurrentVelocity.Y < -50.0)
			{
				GhostFlipbook->SetFlipbook(Anim_WalkUp);
			}
		}
		
	}
}

void ATest2DDummy::Menu()
{
	if(MenuToCreate != nullptr)
	{
		if(MenuRef==nullptr)
		{
			MenuRef = CreateWidget<UUserWidget>(GetWorld(),MenuToCreate);
			MenuRef->AddToViewport(0);
		}
		else if(MenuRef->GetIsVisible())
		{
			MenuRef->RemoveFromParent();
		}
		else
		{
			MenuRef->AddToViewport(0);
		}
		
	}
}

void ATest2DDummy::TryAgain()
{
	//UGameplayStatics::GetGameMode(this)->ResetLevel();
	if(LevelToReset!="")
	{
		UGameplayStatics::OpenLevel(this,LevelToReset);
	}
	
}