// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	// ...
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerLocation, PlayerRotation);
	UE_LOG(LogTemp, Warning, TEXT("Player position: %s, Rotation: %s."), *PlayerLocation.ToString(), *PlayerRotation.ToString());

	FVector LineTraceEnd = PlayerLocation + PlayerRotation.Vector() * Reach;

	// RAY CASTING AKA LINE TRACING
	DrawDebugLine(GetWorld(),
		PlayerLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,	//do line presist (be in multiple frames)
		0.f,
		0.f,
		10.f);
}

