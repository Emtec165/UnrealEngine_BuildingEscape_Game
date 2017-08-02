// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "Components/PrimitiveComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	const float Reach = 200.f;

	UPROPERTY(EditAnywhere)
	float RotationSpeed = 10.f;
	
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	//FVector LineTraceEnd; //Ray-cast end


	//	Ray-cast and grab what's in reach
	void Grab();

	//	Call when grab is realesed
	void Release();

	//	Rotate gabbed object
	void RotateX();
	void RotateY();
	void RotateZ();

	//	Find attached physics handle
	void FindPhysicsHandleComponent();

	//	Setup attached intput component
	void SetupIntputComponent();

	// Return hit for 1st physics body in reach
	FHitResult GetFirstPhysicsBodyInReach();

	//	Draw a red trace (line from player to end of his reach)
	void DrawDebbugLine();

	// Returns current start of reach line
	FVector GetReachLineStart();

	// Returns current end of reach line
	FVector GetReachLineEnd();
};
