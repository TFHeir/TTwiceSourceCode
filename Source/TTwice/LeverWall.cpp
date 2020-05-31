// Fill out your copyright notice in the Description page of Project Settings.


#include "LeverWall.h"
#include "Components/BoxComponent.h"
#include "LeverBase.h"
#include "Test2DDummy.h"
#include "Paper2D/Classes/PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
ALeverWall::ALeverWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComp;
	BoxComp->SetBoxExtent(FVector(40.0f));
	BoxComp->OnComponentBeginOverlap.AddDynamic(this,&ALeverWall::OnOverlapping);
	Sprite = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALeverWall::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
}

// Called every frame
void ALeverWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALeverWall::Initialize()
{
	bIsCurrentWall = bIsWallWhenStart;
	SetCollision(bIsCurrentWall);
	for(auto &Lever:AllLevers)
	{
		Lever.Lever->OnLeverActivationChange.AddDynamic(this,&ALeverWall::ProssessLeverInput);
		Lever.LeverState = Lever.Lever->GetCurrentState();
	}
	if(Sprite->GetFlipbook()!=nullptr)
	{
		Sprite->SetLooping(false);
	}
}

void ALeverWall::OnOverlapping(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("HIHIHIHHI"));
}

void ALeverWall::SetCollision(bool bIsWall)
{
	if(bIsWall)
	{
		//KillPlayerInCollision();
		BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		BoxComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Block);
	}
	else
	{
		BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		BoxComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		BoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);
	}
	UpdateAnimation();
}

void ALeverWall::ProssessLeverInput(bool LeverInput, class ALeverBase* ThisLever)
{
	int32 Sum = 0;
	for(auto &Lever:AllLevers)
	{
		if(ThisLever == Lever.Lever)
		{
			Lever.LeverState = LeverInput;
		}
		Sum+=(int)Lever.LeverState;
	}
	bIsCurrentWall = Sum%2==1?!bIsWallWhenStart:bIsWallWhenStart;
	SetCollision(bIsCurrentWall);
	UE_LOG(LogTemp, Warning, TEXT("Set Collision is : %s"),bIsCurrentWall?TEXT("Wall"):TEXT("NonWall"));
}

void ALeverWall::KillPlayerInCollision()
{
	// TArray<AActor*> Characters;
	// BoxComp->GetOverlappingActors(Characters,ATest2DDummy::StaticClass());
	// if(Characters.Num()>0)
	// {
	// 	if(ATest2DDummy* Character = Cast<ATest2DDummy>(Characters[0]))
	// 	{
	// 		if((Character->GetActorLocation()-GetActorLocation()).Size2D() < BoxComp->GetUnscaledBoxExtent().X-10.f)
	// 		{
	// 			Character->Dead();
	// 		}
	// 	}
	// }
}

void ALeverWall::UpdateAnimation()
{
	if(Sprite->GetFlipbook()!=nullptr)
	{
		if(bIsCurrentWall)
		{
			Sprite->Play();
			if(Sound_Up)
			{
				UGameplayStatics::PlaySound2D(this,Sound_Up);
			}
		}
		else
		{
			Sprite->Reverse();
			if(Sound_Down)
			{
				UGameplayStatics::PlaySound2D(this,Sound_Down);
			}
		}
	}
}