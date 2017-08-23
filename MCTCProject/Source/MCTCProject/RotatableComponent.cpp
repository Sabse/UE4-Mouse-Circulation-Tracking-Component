// Fill out your copyright notice in the Description page of Project Settings.

#include "RotatableComponent.h"


// Sets default values for this component's properties
URotatableComponent::URotatableComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TrackingTime = 0.2f; //measured 0.2 is good time for tracking mouse coordinates
	RotationSmoothingValue = 0.0015f; //measured multiplying RotationSpeed with 0.0015 results in a smooth object rotation
	bIsTrackingEnabled = false;
	CircleConditionEnum = ECircleConditions::CC_NoCondition;
}


// Called when the game starts
void URotatableComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void URotatableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsTrackingEnabled) 
	{
		TrackMousePosition();
		UpdateActorRotation.Broadcast(RotationSpeed);
	}
}

// Called from Outside
void URotatableComponent::StartTracking()
{
	bIsTrackingEnabled = true;
	CalculateRotationFromMousePositions();
}

// Called from Outside
void URotatableComponent::EndTracking()
{
	bIsTrackingEnabled = false;
}

void URotatableComponent::TrackMousePosition()
{
	GetWorld()->GetFirstPlayerController()->GetMousePosition(MousePosition.X, MousePosition.Y);
	SaveMousePosition.Add(MousePosition);
}

// Calling itself after a period of time as long as tracking is enabled
void URotatableComponent::CalculateRotationFromMousePositions()
{
	if (SaveMousePosition.Num() > 2)
	{
		FVector2D FirstVector = SaveMousePosition[0];
		FVector2D SecondVector = SaveMousePosition[int(SaveMousePosition.Num() / 2)];
		FVector2D ThirdVector = SaveMousePosition[SaveMousePosition.Num() - 1];

		SaveMousePosition.Empty();

		FVector2D FirstSecondCathetus = FirstVector - SecondVector;
		FVector2D SecondThirdCathetus = ThirdVector - SecondVector;
		FVector2D FirstThirdHypotenuse = ThirdVector - FirstVector;

		FirstSecondCathetus.Normalize();
		SecondThirdCathetus.Normalize();

		float CrossProduct = FVector2D::CrossProduct(FirstSecondCathetus, SecondThirdCathetus);
		float Distance = FirstThirdHypotenuse.Size();

		if (CrossProduct < 0) //clockwise rotation
		{
			if (CircleConditionEnum == ECircleConditions::CC_NoCondition || CircleConditionEnum == ECircleConditions::CC_Clockwise)
			{
				RotationSpeed = (Distance)*RotationSmoothingValue;
			}
			else
			{
				RotationSpeed = 0.f;
			}
		}
		else //no clockwise rotation
		{
			if (CircleConditionEnum == ECircleConditions::CC_NoCondition || CircleConditionEnum == ECircleConditions::CC_NotClockwise)
			{
				RotationSpeed = -(Distance)*RotationSmoothingValue;
			}
			else
			{
				RotationSpeed = 0.f;
			}
		}
	}

	if (bIsTrackingEnabled)
	{
		GetWorld()->GetTimerManager().SetTimer(PositionLoggerTimerHandle, this, &URotatableComponent::CalculateRotationFromMousePositions, TrackingTime, false);
	}
	else
	{
		RotationSpeed = 0.f;
	}
}

