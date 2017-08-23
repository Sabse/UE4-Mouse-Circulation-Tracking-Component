// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/World.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RotatableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActorRotationUpdateDelegate, float, RotationValue);

UENUM()
enum class ECircleConditions : uint8
{
	CC_NoCondition		UMETA(DisplayName ="Clockwise And Not Clockwise Rotation"),
	CC_Clockwise		UMETA(DisplayName="Clockwise Rotation Only"),
	CC_NotClockwise		UMETA(DisplayName = "No Clockwise Rotation")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MCTCPROJECT_API URotatableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URotatableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
	float RotationSmoothingValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
	float TrackingTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
	ECircleConditions CircleConditionEnum;
	
	UFUNCTION(BlueprintCallable, Category = "Input Hook")
	void StartTracking();

	UFUNCTION(BlueprintCallable, Category = "Input Hook")
	void EndTracking();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FActorRotationUpdateDelegate UpdateActorRotation;

	void TrackMousePosition();
	void CalculateRotationFromMousePositions();

	float RotationSpeed;
	bool bIsTrackingEnabled;

	FVector2D MousePosition;
	TArray<FVector2D> SaveMousePosition;

	FTimerHandle PositionLoggerTimerHandle;
};
