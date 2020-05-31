// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExitBase.generated.h"

UCLASS()
class TTWICE_API AExitBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY(Category = Sprite, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbookComponent* Sprite;
	UPROPERTY(Category = Box, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* LevelTrigger;

	FTimerHandle FadeOutTimer;

	void OpenNextLevel();
public:	
	// Sets default values for this actor's properties
	AExitBase();
	
	UPROPERTY(Category = Switch, VisibleAnywhere, BlueprintReadWrite)
	class USwitchComponent* Switches;
	UPROPERTY(EditAnywhere)
	FName LevelToOpen;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool bDoorActive;
	void LevelChangingCheck();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void SetDoorActive(bool EnableDoor);
	UPROPERTY(EditAnywhere, Category="Loading")
	TSubclassOf<UUserWidget> WidgetBPReference;
	UPROPERTY(EditAnywhere, Category = Sound, BlueprintReadOnly)
	class USoundCue* Sound_Open;
	UPROPERTY(EditAnywhere, Category = Sound, BlueprintReadOnly)
	class USoundCue* Sound_Off;
};
