// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimeGhostComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDeQueueDelegate, UTimeGhostComponent*, GhostComponent, FTransform, PastTimeTransform, FVector, Velocity);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TTWICE_API UTimeGhostComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTimeGhostComponent();

	TQueue<FTransform, EQueueMode::Spsc> GhostLocation;

	FDeQueueDelegate GhostPath;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void ProcessGhostLoc();

	FVector CalcVelocity(FVector LastLocation);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	class UPlayerInput* PlayerInput;
	int32 QueueSize;
	int32 MaxQSize;
		
};
