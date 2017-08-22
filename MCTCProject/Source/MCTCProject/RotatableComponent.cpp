// Fill out your copyright notice in the Description page of Project Settings.

#include "RotatableComponent.h"


// Sets default values for this component's properties
URotatableComponent::URotatableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TrackingTime = 0.2f; //measured 0.2 is good time for rotation measuring
	RotationSmoothingValue = 0.0015f;
	bIsRotationEnabled = false;
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

	if (bIsRotationEnabled) 
	{
		TrackMousePosition();
		UpdateActorRotation.Broadcast(RotationSpeed);
	}
}

void URotatableComponent::OnInputStart()
{
	//UE_LOG(LogTemp, Warning, TEXT("rotatable component: received input"));
	bIsRotationEnabled = true;
	CheckMouseCirculation();
}

void URotatableComponent::OnInputEnd()
{
	bIsRotationEnabled = false;
}

void URotatableComponent::TrackMousePosition()
{
	GetWorld()->GetFirstPlayerController()->GetMousePosition(MousePosition.X, MousePosition.Y);
	//UE_LOG(LogTemp, Warning, TEXT("X: %f, Y: %f"), MousePosition.X, MousePosition.Y);
	SaveMousePosition.Add(MousePosition);
}

void URotatableComponent::CheckMouseCirculation()
{
	//UE_LOG(LogTemp, Warning, TEXT("rotatable component: calculating mouse circulation"));
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

		float crossProduct = FVector2D::CrossProduct(FirstSecondCathetus, SecondThirdCathetus);

		float Distance = FirstThirdHypotenuse.Size();
		UE_LOG(LogTemp, Warning, TEXT("%f"), Distance);

		if (crossProduct < 0) //clockwise rotation
		{
			
			RotationSpeed = (Distance)*RotationSmoothingValue;
		}
		else //no clockwise rotation
		{
			RotationSpeed = -(Distance)*RotationSmoothingValue;
		}

		//UE_LOG(LogTemp, Warning, TEXT("%f"), crossProduct);
	}

	if (bIsRotationEnabled)
	{
		GetWorld()->GetTimerManager().SetTimer(PositionLoggerTimerHandle, this, &URotatableComponent::CheckMouseCirculation, TrackingTime, false);
	}
	else
	{
		RotationSpeed = 0.f;
	}
}

