// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SwitchComponent.generated.h"
class APressButtonBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOneParamDelegate, bool, Openable);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TTWICE_API USwitchComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USwitchComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
	TArray<APressButtonBase*> AllSwitches;
	//Delegate to trigger bind function on changing
	FOneParamDelegate OnDoorStateChanging;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	void CheckSwitches();
	//keep door state
	bool LastState;
		
};
