// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Public/DrawDebugHelpers.h"

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

    UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!!"));

    // Look for attached physics handle
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (!PhysicsHandle)
    {
        UE_LOG(LogTemp, Error, TEXT("%s don't have physics handle"), *(GetOwner()->GetName()));
    }

    // Look for attached input component
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (InputComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("Input component found"))

        // Bind the input
        InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("%s don't have input component"), *(GetOwner()->GetName()));
    }
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get the player's view point
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;

    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
        OUT PlayerViewPointLocation,
        OUT PlayerViewPointRotation
    );

    FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

    DrawDebugLine(
        GetWorld(),
        PlayerViewPointLocation,
        LineTraceEnd,
        FColor(255, 0, 0), // red
        false,
        0.f,
        0,
        10.f
    );

    // Set up query parameter
    FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

    // Get the collision result
    FHitResult Hit;
    bool IsCollide = GetWorld()->LineTraceSingleByObjectType
    (
        OUT Hit,
        PlayerViewPointLocation,
        LineTraceEnd,
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        TraceParameters
    );

    // Test the collision

    if (IsCollide)
    {
        UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *(Hit.GetActor()->GetName()))
    }
}

void UGrabber::Grab()
{
    UE_LOG(LogTemp, Warning, TEXT("Grab Pressed"))
}
