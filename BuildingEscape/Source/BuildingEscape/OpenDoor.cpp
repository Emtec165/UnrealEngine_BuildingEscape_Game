// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Gameframework/Actor.h"

#define OUT


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();

	if (!PresurePlate) { UE_LOG(LogTemp, Error, TEXT("ATriggerVolume is not attached to the %s!"), *GetOwner()->GetName()); }
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOfActorsOnPlate() > MassRequiredToOpenDoors) {
		OnOpenRequest.Broadcast();
	}
	else {
		OnCloseRequest.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {
	if (!PresurePlate) { return 0.f; }
	float TotalMass = 0.f;

	//	find all the overlapping actors
	TArray<AActor*> OverlapingActors;
	PresurePlate->GetOverlappingActors(OUT OverlapingActors);

	// iterate through them adding theri masses
	for (const auto* Actor : OverlapingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		//UE_LOG(LogTemp, Warning, TEXT("Total mass on presure plate equals %f"), TotalMass);
	}

	return TotalMass;
}