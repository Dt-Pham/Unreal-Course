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
    Owner = GetOwner();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // If the ActorThatOpens overlap the PressurePlate
	if (GetTotalMassOnPlate() > TriggerMass)
    {
        OnOpen.Broadcast();
    }
    else
    {
        OnClose.Broadcast();
    }
}

float UOpenDoor::GetTotalMassOnPlate()
{
    float TotalMass = 0.f;

    TArray<AActor *> OverlappingActor;
    PressurePlate->GetOverlappingActors(OverlappingActor);

    for (const auto& Actor : OverlappingActor)
    {
        TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
        UE_LOG(LogTemp, Warning, TEXT("%s is on the plate."), *Actor->GetName());
    }

    return TotalMass;
}
