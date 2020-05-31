// Fill out your copyright notice in the Description page of Project Settings.


#include "PressButtonBase.h"
#include "TimerManager.h"
#include "Paper2D/Classes/PaperFlipbookComponent.h"
#include "Classes/Components/CapsuleComponent.h"
#include "Test2DDummy.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APressButtonBase::APressButtonBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	//Capsule Component initialize
	RootComponent = CapsuleComp;
	CapsuleComp->SetCapsuleSize(16.0f,32.0f);
	CapsuleComp->SetCollisionProfileName("OverlapAllDynamic");
	//Sprite initialize
	Sprite = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(RootComponent);
	Sprite->SetLooping(false);
	//Bind function when Overlapping
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &APressButtonBase::BeginPress);
	CapsuleComp->OnComponentEndOverlap.AddDynamic(this, &APressButtonBase::EndPress);
	
	//boolean initialize
	bIsPress = false;
	bIsLever = false;
}

// Called when the game starts or when spawned
void APressButtonBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APressButtonBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// GetOverlappingComponents(XX);
	// for(UPrimitiveComponent* X : XX)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Name : %s"),*X->GetName());
	// }

}

void APressButtonBase::BeginPress(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(bIsLever)
	{
		if(bIsPress)
		ResetPress();
		else
		bIsPress = true;	
	}
	else
	{
		bIsPress = true;
		UpdateAnimation();
	}
	UE_LOG(LogTemp, Warning, TEXT("Pressed"));
	if(GetWorld()->GetTimerManager().IsTimerActive(ResetTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(ResetTimer);
	}
}

void APressButtonBase::EndPress(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TArray<AActor*> Actors;
	CapsuleComp->GetOverlappingActors(Actors);
	int flag = 0;
	if(Actors.Num()>0)
	{
		for(auto* Actor:Actors)
		{
			if(Cast<ATest2DDummy>(Actor))
			{
				flag++;
			}
		}
	}
	if(!(flag > 0))
	{
		GetWorld()->GetTimerManager().SetTimer(ResetTimer,this,&APressButtonBase::ResetPress, 2.0f, false);
		Sprite->PlayFromStart();
	}	
}

void APressButtonBase::ResetPress()
{
	bIsPress = false;
	UE_LOG(LogTemp, Warning, TEXT("Reset!"));
	UpdateAnimation();
}

void APressButtonBase::UpdateAnimation()
{
	if(!bIsPress && Idle!=nullptr)
	{
		Sprite->SetFlipbook(Idle);
		if(Sound_Release!=nullptr)
		{
			UGameplayStatics::PlaySound2D(this,Sound_Release);
		}
	}
	else if(bIsPress && ButtonActive!=nullptr)
	{
		Sprite->SetFlipbook(ButtonActive);
		Sprite->Stop();
		if(Sound_Press!=nullptr)
		{
			UGameplayStatics::PlaySound2D(this,Sound_Press);
		}
	}
}