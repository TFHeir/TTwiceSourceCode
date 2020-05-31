// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PressButtonBase.generated.h"

UCLASS()
class TTWICE_API APressButtonBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CapsuleComponent, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sprite, meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbookComponent* Sprite;
public:	
	// Sets default values for this actor's properties
	APressButtonBase();
	UFUNCTION()
	void BeginPress(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
	void EndPress(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool GetIsPress() const {return bIsPress;};

	UPROPERTY(EditAnywhere, Category = Sprite, BlueprintReadOnly)
	class UPaperFlipbook* Idle;
	UPROPERTY(EditAnywhere, Category = Sprite, BlueprintReadOnly)
	class UPaperFlipbook* ButtonActive;
	UPROPERTY(EditAnywhere, Category = Sound, BlueprintReadOnly)
	class USoundCue* Sound_Press;
	UPROPERTY(EditAnywhere, Category = Sound, BlueprintReadOnly)
	class USoundCue* Sound_Release;
private:
	bool bIsPress;

	FTimerHandle ResetTimer;

	void ResetPress();

	bool bIsLever;

	void UpdateAnimation();
};
