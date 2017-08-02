// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Gameframework/Actor.h"
#include "Engine/World.h"

#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupIntputComponent();
}

///	Look for Physics Handle
void UGrabber::FindPhysicsHandleComponent() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s don't have UPhysicsHandleComponent"), *GetOwner()->GetName())
	}
}

/// Look for Input Component
void UGrabber::SetupIntputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("%s found UInputComponent"), *GetOwner()->GetName())
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s don't have UInputComponent"), *GetOwner()->GetName())
	}
}

/// Grab an object
void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"))

	//	LINE TRACE and see if we can reach any actors with physics handle body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); // gets the mesh in our case
	auto ActorHit = HitResult.GetActor();

	///	If we hit something then attach a physics handle
	if (ActorHit != nullptr){
		if (!PhysicsHandle) { return; }
		// attach physics handle
		PhysicsHandle->GrabComponent(ComponentToGrab,
			NAME_None, // no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true //	allow rotation
		);
	}
}

/// Release an object
void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab released"))
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; }
	//	if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent){
		// move the object that player is holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() {

	///Setup query parameters
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());
	FHitResult HitResult;

	///Line tracing AKA Ray-casting
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	AActor* CollidingObject = HitResult.GetActor();
	if (CollidingObject) {
		UE_LOG(LogTemp, Warning, TEXT("Player hits %s"), *CollidingObject->GetName());
	}

	return HitResult;
}

///	Draw a red trace (line from player to end of his reach)
void UGrabber::DrawDebbugLine() {
	DrawDebugLine(GetWorld(),
		GetReachLineStart(),
		GetReachLineEnd(),
		FColor(255, 0, 0),
		false,	//do line presist (be in multiple frames)
		0.f,
		0.f,
		10.f
	);
}

FVector UGrabber::GetReachLineStart() {
	FVector PlayerLocation;
	FRotator PlayerRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerLocation, OUT PlayerRotation);
	return PlayerLocation;
}

FVector UGrabber::GetReachLineEnd() {
	FVector PlayerLocation;
	FRotator PlayerRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerLocation,
		OUT PlayerRotation
		);

	///Report Player view point
	//UE_LOG(LogTemp, Warning, TEXT("Player position: %s, Rotation: %s."), *PlayerLocation.ToString(), *PlayerRotation.ToString());

	return PlayerLocation + PlayerRotation.Vector() * Reach;
}

