// Fill out your copyright notice in the Description page of Project Settings.


#include "LeverBase.h"
#include "Classes/Engine/TriggerBox.h"
#include "Components/ShapeComponent.h"
#include "Classes/Components/BoxComponent.h"
#include "Containers/Queue.h"
#include "TimerManager.h"
#include "Test2DDummy.h"
#include "Paper2D/Classes/PaperFlipbookComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ALeverBase::ALeverBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Root;
	Root->OnComponentBeginOverlap.AddDynamic(this,&ALeverBase::OnBeginOverlap);
	CurrentState = false;
	LastTriggerTime = 0.0f;
	Root->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Root->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Root->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);
	Sprite = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALeverBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALeverBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateAnimation();
}


void ALeverBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	UWorld* World = GetWorld();
	if(Cast<ATest2DDummy>(OtherActor) && World!=nullptr)
	{
		float now = World->GetRealTimeSeconds();
		if(World->GetTimerManager().IsTimerActive(ToggleTimer))
		{
			//When Timer is Active, Push other toggle command into queue
			ToggleTime.Enqueue(now - LastTriggerTime);
			LastTriggerTime = now;
		}
		else
		{
			World->GetTimerManager().SetTimer(ToggleTimer,this,&ALeverBase::ToggleLever,3.0f,false);
			LastTriggerTime = now;
		}
		
		CurrentState = !CurrentState;
		if(Sound_Push!=nullptr&&Sound_Rebounce!=nullptr)
		{
			UGameplayStatics::PlaySound2D(this,CurrentState?Sound_Push:Sound_Rebounce);
		}
		OnLeverActivationChange.Broadcast(CurrentState,this);
	}
}

void ALeverBase::ToggleLever()
{
	UWorld* World = GetWorld();
	CurrentState = !CurrentState;
	if(Sound_Push!=nullptr&&Sound_Rebounce!=nullptr)
	{
		UGameplayStatics::PlaySound2D(this,CurrentState?Sound_Push:Sound_Rebounce);
	}
	OnLeverActivationChange.Broadcast(CurrentState,this);
	if(!ToggleTime.IsEmpty())
	{
		float TimeToNextTrigger;
		ToggleTime.Dequeue(TimeToNextTrigger);
		World->GetTimerManager().SetTimer(ToggleTimer,this,&ALeverBase::ToggleLever,TimeToNextTrigger,false);		
	}
}

void ALeverBase::UpdateAnimation()
{
	if(CurrentState && ActiveSprite != nullptr)
	{
		Sprite->SetFlipbook(ActiveSprite);
	}
	else if(!CurrentState && InactiveSprite != nullptr)
	{
		Sprite->SetFlipbook(InactiveSprite);
	}
}
