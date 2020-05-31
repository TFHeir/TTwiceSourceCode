// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeGhostComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "Engine/World.h"
#include "PaperFlipbookComponent.h"
#include "Engine.h"

// Sets default values for this component's properties
UTimeGhostComponent::UTimeGhostComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	QueueSize = 0;
	SetTickGroup(ETickingGroup::TG_EndPhysics);
	
	
	// ...
}


// Called when the game starts
void UTimeGhostComponent::BeginPlay()
{
	Super::BeginPlay();
	// TArray<float> AAA = {10.0f, 15.0f};
	// TQueue<float,EQueueMode::Spsc> QQQ;
	// float c;
	// QQQ.Enqueue(10.0f);
	// QQQ.Enqueue(15.0f);
	// QQQ.Dequeue(c);
	// UE_LOG(LogTemp, Warning, TEXT("Pop : %f"),AAA.Pop());
	// UE_LOG(LogTemp, Warning, TEXT("Dequeue : %f"),c);
	// QQQ.Dequeue(c);
	// UE_LOG(LogTemp, Warning, TEXT("Dequeue : %f"),c);
	// ...
	
}


// Called every frame
void UTimeGhostComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(PlayerInput == nullptr)
	{
		APlayerController* PlayerCon = UGameplayStatics::GetPlayerController(GetWorld(),0);
		if(PlayerCon != nullptr)
		{
			PlayerInput = PlayerCon->PlayerInput;
			MaxQSize = 1/DeltaTime*3;
			//GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Yellow,FString::FromInt(MaxQSize));
		}
		if(PlayerInput == nullptr)
		return;
	}
	ProcessGhostLoc();
	//UE_LOG(LogTemp, Warning, TEXT("Right Key Holding Time : %f"),PlayerInput->GetTimeDown(EKeys::Right));
	// ...
}

void UTimeGhostComponent::ProcessGhostLoc()
{
	FTransform NewTrans = GetOwner()->GetTransform();
	if(QueueSize < MaxQSize)
	{
		GhostLocation.Enqueue(NewTrans);
		QueueSize++;
	}
	else
	{
		GhostLocation.Enqueue(NewTrans);
		FTransform Test;
		GhostLocation.Dequeue(Test);
		FVector Loc = Test.GetLocation();
		Loc.Z -= 5.0f;
		Loc.Y -= 15.0f;
		Test.SetLocation(Loc);
		GhostPath.Broadcast(this, Test, CalcVelocity(Loc));
		
		//UE_LOG(LogTemp, Warning, TEXT("Trans Loc : %s"),*Test.GetLocation().ToString());
		//UE_LOG(LogTemp, Warning, TEXT("Trans Rot : %s"),*Test.GetRotation().ToString());
	}
	
}

FVector UTimeGhostComponent::CalcVelocity(FVector LastLocation)
{
	FTransform NextTrans;
	GhostLocation.Peek(NextTrans);
	FVector NextLoc = NextTrans.GetLocation();
	NextLoc.Z -= 5.0f;
	NextLoc.Y -= 15.0f;
	FVector Velocity = (NextLoc-LastLocation)*60;
	return Velocity;
}