// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitBase.h"
#include "SwitchComponent.h"
#include "Paper2D/Classes/PaperFlipbookComponent.h"
#include "Classes/Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Classes/GameFramework/Character.h"
#include "TimerManager.h"
#include "UserWidget.h"
#include "Sound/SoundCue.h"

// Sets default values
AExitBase::AExitBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//create sub-object and component
	Switches = CreateDefaultSubobject<USwitchComponent>(TEXT("SwitchComp"));
	Sprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	LevelTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	//Delegate Binding
	Switches->OnDoorStateChanging.AddDynamic(this,&AExitBase::SetDoorActive);
	//Attachment
	RootComponent = LevelTrigger;
	Sprite->SetupAttachment(RootComponent);

	//Set Collision
	LevelTrigger->SetBoxExtent(FVector(5.f,5.f,100.f));
	//LevelTrigger->SetCollisionProfileName("");
	LevelTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LevelTrigger->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	LevelTrigger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LevelTrigger->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);

	bDoorActive = false;
	Sprite->SetLooping(false);
}

// Called when the game starts or when spawned
void AExitBase::BeginPlay()
{
	Super::BeginPlay();
	Sprite->PlayFromStart();
	Sprite->Stop();
}

// Called every frame
void AExitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LevelChangingCheck();
}

void AExitBase::SetDoorActive(bool EnableDoor)
{
	if(EnableDoor)
	{
		//Sprite->SetVisibility(true);
		Sprite->PlayFromStart();
		if(Sound_Open!=nullptr)
		UGameplayStatics::PlaySound2D(this,Sound_Open);
	}
	else
	{
		Sprite->Reverse();
		if(Sound_Off!=nullptr)
		UGameplayStatics::PlaySound2D(this,Sound_Off);
	}
	bDoorActive = EnableDoor;
}

void AExitBase::LevelChangingCheck()
{
	if(bDoorActive && LevelToOpen != "")
	{
		TArray<AActor*> Actors;
		ACharacter* PlayerChar = UGameplayStatics::GetPlayerCharacter(this,0);
		LevelTrigger->GetOverlappingActors(Actors,PlayerChar->StaticClass());
		if(Actors.Num()>0 && WidgetBPReference != nullptr)
		{
			PlayerChar->DisableInput(UGameplayStatics::GetPlayerController(this,0));
			GetWorld()->GetTimerManager().SetTimer(FadeOutTimer,this,&AExitBase::OpenNextLevel,0.5f,false);
			UUserWidget* UMGInstance = CreateWidget<UUserWidget>(GetWorld(), WidgetBPReference);
			UMGInstance->AddToViewport();
		}
	}
}
void AExitBase::OpenNextLevel()
{
	UGameplayStatics::OpenLevel(this,LevelToOpen,true);
}