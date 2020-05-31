// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LeverWall.generated.h"
USTRUCT(BlueprintType)
struct FLeverInfo
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, Category = Lever,BlueprintReadOnly)
	class ALeverBase* Lever;
	//True is Active State, false is Wall State
	bool LeverState;
};
UCLASS()
class TTWICE_API ALeverWall : public AActor
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BoxComponent, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sprite, meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbookComponent* Sprite;

	void SetCollisionType(bool bIsWall);
	//Available State is false, and Wall State is true
	bool bIsCurrentWall;
	//Initialize
	void Initialize();
	//Set Collision Enable
	void SetCollision(bool bIsWall);
	//Bind All Delegate
	UFUNCTION()
	void ProssessLeverInput(bool LeverInput, class ALeverBase* ThisLever);

	void KillPlayerInCollision();

	void UpdateAnimation();


public:	
	// Sets default values for this actor's properties
	ALeverWall();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Lever)
	//All levers which control this LeverWall
	TArray<FLeverInfo> AllLevers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = InitialRef)
	bool bIsWallWhenStart;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapping(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UPROPERTY(EditAnywhere, Category = Sound, BlueprintReadOnly)
	class USoundCue* Sound_Up;
	UPROPERTY(EditAnywhere, Category = Sound, BlueprintReadOnly)
	class USoundCue* Sound_Down;
};
