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

    FindPhysicsHandleComponent();
    FindInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;

    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
        OUT PlayerViewPointLocation,
        OUT PlayerViewPointRotation
    );

    FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

    // If the physics handle is attached
    if (PhysicsHandle->GrabbedComponent)
    {
        // Move the object that have been attached
        PhysicsHandle->SetTargetLocation(LineTraceEnd);
    }
}

// Look for attached physics handle
void UGrabber::FindPhysicsHandleComponent()
{
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (!PhysicsHandle)
    {
        UE_LOG(LogTemp, Error, TEXT("%s don't have physics handle"), *(GetOwner()->GetName()));
    }
    UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!!"));
}

// Look for attached input component
void UGrabber::FindInputComponent()
{
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (InputComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("Input component found"))

        // Bind the input
        InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("%s don't have input component"), *(GetOwner()->GetName()));
    }
}

void UGrabber::Grab()
{
    UE_LOG(LogTemp, Warning, TEXT("Grab Pressed"))

    // Check if we reach a physicsbody
    FHitResult Hit = GetFirstPhysicsBodyInReach();
    UPrimitiveComponent *ComponentToGrab = Hit.GetComponent();
    AActor *ActorHit = Hit.GetActor();

    if(ActorHit)
    {
        PhysicsHandle->GrabComponent(
            ComponentToGrab,
            NAME_None,
            ComponentToGrab->GetOwner()->GetActorLocation(),
            true // allow rotation
        );
    }
}

void UGrabber::Release()
{
    UE_LOG(LogTemp, Warning, TEXT("Grab Released"))
    PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
    // Get the player's view point
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;

    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
        OUT PlayerViewPointLocation,
        OUT PlayerViewPointRotation
    );

    FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

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
    return Hit;
}
