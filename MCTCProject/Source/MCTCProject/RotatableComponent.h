// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/World.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RotatableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActorRotationUpdateDelegate, float, RotationValue);


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dependencies")
	float RotationSmoothingValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dependencies")
	float TrackingTime;
	
	UFUNCTION(BlueprintCallable, Category = "Callable Functions")
	void OnInputStart();

	UFUNCTION(BlueprintCallable, Category = "Callable Functions")
	void OnInputEnd();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FActorRotationUpdateDelegate UpdateActorRotation;

	void TrackMousePosition();
	void CheckMouseCirculation();

	float RotationSpeed;
	bool bIsRotationEnabled;

	FVector2D MousePosition;
	TArray<FVector2D> SaveMousePosition;

	FTimerHandle PositionLoggerTimerHandle;
};
