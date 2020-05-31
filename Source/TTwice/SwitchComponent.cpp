// Fill out your copyright notice in the Description page of Project Settings.


#include "SwitchComponent.h"
#include "PressButtonBase.h"

// Sets default values for this component's properties
USwitchComponent::USwitchComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//Initialize State
	LastState = false;
	// ...
}


// Called when the game starts
void USwitchComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USwitchComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CheckSwitches();
	// ...
}

void USwitchComponent::CheckSwitches()
{
	if(GetWorld())
	{
		bool flag = true;
		for(APressButtonBase* I : AllSwitches)
		{
			if(!I->GetIsPress())
			{
				flag = false;
			}
		}
		if(LastState!=flag)
		{
			OnDoorStateChanging.Broadcast(flag);
			LastState = flag;
		}
	}
}