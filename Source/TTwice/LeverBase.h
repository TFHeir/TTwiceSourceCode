// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LeverBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTwoParamDelegate, bool, bIsActive, ALeverBase*, ThisLever);

UCLASS()
class TTWICE_API ALeverBase : public AActor
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere,Category = Box, meta = (AllowPrivateAcess = "true"))
	class UBoxComponent* Root;
	//Current State of this Lever. True is Active, false is Inactive
	bool CurrentState;

	UPROPERTY(Category = Sprite, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbookComponent* Sprite;

	TQueue<float, EQueueMode::Spsc> ToggleTime;

	float LastTriggerTime;

	FTimerHandle ToggleTimer;

	void UpdateAnimation();
public:	
	// Sets default values for this actor's properties
	ALeverBase();

	FTwoParamDelegate OnLeverActivationChange;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ToggleLever();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
	bool GetCurrentState() const {return CurrentState;};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sprite)
	class UPaperFlipbook* ActiveSprite;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sprite)
	class UPaperFlipbook* InactiveSprite;

	UPROPERTY(EditAnywhere, Category = Sound, BlueprintReadOnly)
	class USoundCue* Sound_Push;
	UPROPERTY(EditAnywhere, Category = Sound, BlueprintReadOnly)
	class USoundCue* Sound_Rebounce;
};
