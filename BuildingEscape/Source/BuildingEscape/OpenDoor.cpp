// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

    ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // If the ActorThatOpens overlap the PressurePlate
	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
    {
        OpenDoor();
    }
}

void UOpenDoor::OpenDoor()
{
    // Get the Owner
    AActor* Owner = GetOwner();

    // Create new rotator
    FRotator NewRotation = FRotator(0.f, 90.f, 0.f);

    // Rotate the door
    Owner->SetActorRotation(NewRotation);
}
