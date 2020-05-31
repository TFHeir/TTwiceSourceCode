// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Test2DDummy.generated.h"

/**
 * 
 */
UCLASS(config=Game)
class TTWICE_API ATest2DDummy : public APaperCharacter
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	

	private:
	//Player moving function, with arrow keys
	void MoveRight(float Value);
	void MoveUp(float Value);
	//Reload level (restart game)
	void TryAgain();
	FVector GhostCurrentVelocity;

	FTimerHandle DeadToRetryTimer;

	class UUserWidget* MenuRef;

	protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	void UpdateAnimation();
	//the 2D animation when character is not moving(stand still)
	UPROPERTY(Category = Animation, BlueprintReadWrite, EditAnywhere)
	class UPaperFlipbook* Anim_Idle;
	UPROPERTY(Category = Animation, BlueprintReadWrite, EditAnywhere)
	class UPaperFlipbook* Anim_WalkRight;
	UPROPERTY(Category = Animation, BlueprintReadWrite, EditAnywhere)
	class UPaperFlipbook* Anim_WalkLeft;
	UPROPERTY(Category = Animation, BlueprintReadWrite, EditAnywhere)
	class UPaperFlipbook* Anim_WalkDown;
	UPROPERTY(Category = Animation, BlueprintReadWrite, EditAnywhere)
	class UPaperFlipbook* Anim_WalkUp;
	UPROPERTY(Category = Animation, BlueprintReadWrite, EditAnywhere)
	class UPaperFlipbook* Anim_WalkRightUp;
	UPROPERTY(Category = Animation, BlueprintReadWrite, EditAnywhere)
	class UPaperFlipbook* Anim_WalkRightDown;
	UPROPERTY(Category = Animation, BlueprintReadWrite, EditAnywhere)
	class UPaperFlipbook* Anim_WalkLeftUp;
	UPROPERTY(Category = Animation, BlueprintReadWrite, EditAnywhere)
	class UPaperFlipbook* Anim_WalkLeftDown;
	UPROPERTY(Category = Animation, BlueprintReadWrite, EditAnywhere)
	class UPaperFlipbook* Anim_Spawn;



	public:
	ATest2DDummy();
	class UTimeGhostComponent* TimeGhost;

	UFUNCTION()
	void SetGhostLocation(UTimeGhostComponent* GhostComp, FTransform NewTransform, FVector GhostVelocity);

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbookComponent* GhostFlipbook;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Ghost)
	UCapsuleComponent* GhostCollision;

	UFUNCTION()
	void Menu();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = LevelToReset)
	FName LevelToReset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Menu)
	TSubclassOf<class UUserWidget> MenuToCreate;
};
