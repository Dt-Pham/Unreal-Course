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

    // If the physics handle is attached
    if (PhysicsHandle->GrabbedComponent)
    {
        PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
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
}

// Look for attached input component
void UGrabber::FindInputComponent()
{
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (InputComponent)
    {
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
    // Check if we reach a physicsbody
    FHitResult HitResult = GetFirstPhysicsBodyInReach();
    UPrimitiveComponent *ComponentToGrab = HitResult.GetComponent();
    AActor *ActorHit = HitResult.GetActor();

    if(ActorHit)
    {
        PhysicsHandle->GrabComponent(
            ComponentToGrab,
            NAME_None,
            ComponentToGrab->GetOwner()->GetActorLocation(),
            true
        );
    }
}

void UGrabber::Release()
{
    PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
    // Set up query parameter
    FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

    // Get the collision result
    FHitResult HitResult;
    GetWorld()->LineTraceSingleByObjectType
    (
        OUT HitResult,
        GetLineTraceStart(),
        GetLineTraceEnd(),
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        TraceParameters
    );

    return HitResult;
}

FVector UGrabber::GetLineTraceStart() const
{
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;

    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
        OUT PlayerViewPointLocation,
        OUT PlayerViewPointRotation
    );

    return PlayerViewPointLocation;
}

FVector UGrabber::GetLineTraceEnd() const
{
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;

    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
        OUT PlayerViewPointLocation,
        OUT PlayerViewPointRotation
    );

    return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}
