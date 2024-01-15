// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineActor.h"


// Sets default values
ASplineActor::ASplineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	if (SplineComponent)
	{
		//Ensures that the root component is defined
		SetRootComponent(SplineComponent);
	}
}

void ASplineActor::OnConstruction(const FTransform& Transform)
{
	//If there are meshes on the spline...
	if (SplineMeshMap.Num() > 0)
	{
		//Find the middle meshes on the spline (any mesh that is not the start or end mesh)
		FSplineMeshDetails* DefaultMeshDetails = nullptr;
		if (SplineMeshMap.Contains(ESplineMeshType::DEFAULT))
		{
			DefaultMeshDetails = SplineMeshMap.Find(ESplineMeshType::DEFAULT);
		}
		else
		{
			//If there is no default mesh then do not continue the function
			return;
		}

		//Find the first mesh on the spline
		FSplineMeshDetails* StartMeshDetails = nullptr;
		if (SplineMeshMap.Contains(ESplineMeshType::START))
		{
			StartMeshDetails = SplineMeshMap.Find(ESplineMeshType::START);
		}

		//Find the last mesh on the spline
		FSplineMeshDetails* EndMeshDetails = nullptr;
		if (SplineMeshMap.Contains(ESplineMeshType::END))
		{
			EndMeshDetails = SplineMeshMap.Find(ESplineMeshType::END);
		}

		//Iterates through all points on the spline
		for (int SplineCount = 0; SplineCount < (SplineComponent->GetNumberOfSplinePoints() - 1); SplineCount++)
		{
			//Generate individual mesh spline components to attach to each point on the spline
			USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());

			//Allow meshes on spline to be mobile
			SplineMeshComponent->SetMobility(EComponentMobility::Movable);

			//Defers construction to Blueprint's Construction Script
			SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;

			SplineMeshComponent->RegisterComponentWithWorld(GetWorld());

			//Mesh attachment is relative to the spline component
			SplineMeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);

			//Define starting point on the spline
			const FVector StartPoint = SplineComponent->GetLocationAtSplinePoint(SplineCount, ESplineCoordinateSpace::Local);
			const FVector StartTangent = SplineComponent->GetTangentAtSplinePoint(SplineCount, ESplineCoordinateSpace::Local);

			//Define ending point on the spline
			const FVector EndPoint = SplineComponent->GetLocationAtSplinePoint(SplineCount + 1, ESplineCoordinateSpace::Local);
			const FVector EndTangent = SplineComponent->GetTangentAtSplinePoint(SplineCount + 1, ESplineCoordinateSpace::Local);

			SplineMeshComponent->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);

			//Enable collision on this spline component
			SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

			//Set different meshes for start, middle, and end of spline
			//Start of spline
			if (StartMeshDetails && StartMeshDetails->Mesh && SplineCount == 0)
			{
				SplineMeshComponent->SetStaticMesh(StartMeshDetails->Mesh);
				//Updates the meshes' forward axis (is it on the X, Y, or Z axis?)
				SplineMeshComponent->SetForwardAxis(StartMeshDetails->ForwardAxis);
			}
			//End of spline
			else if (EndMeshDetails && EndMeshDetails->Mesh && SplineComponent->GetNumberOfSplinePoints() > 2 && SplineCount == (SplineComponent->GetNumberOfSplinePoints() - 2))
			{
				SplineMeshComponent->SetStaticMesh(EndMeshDetails->Mesh);
				SplineMeshComponent->SetForwardAxis(EndMeshDetails->ForwardAxis);
			}
			//Middle of spline
			else
			{
				if (DefaultMeshDetails->Mesh)
				{
					SplineMeshComponent->SetStaticMesh(DefaultMeshDetails->Mesh);
					SplineMeshComponent->SetForwardAxis(DefaultMeshDetails->ForwardAxis);

					//Allows user to set materials on spline meshes
					//In this example, every other mesh in the middle of the spline will have the Alternative Material, the other meshes will have the Default Material
					if (DefaultMeshDetails->AlternativeMaterial && SplineCount > 0 && SplineCount % 2 == 0)
					{
						SplineMeshComponent->SetMaterial(0, DefaultMeshDetails->AlternativeMaterial);
					}
					else if (DefaultMeshDetails->DefaultMaterial)
					{
						SplineMeshComponent->SetMaterial(0, DefaultMeshDetails->DefaultMaterial);
					}
				}
			}
		}

	}
}

// Called when the game starts or when spawned
void ASplineActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASplineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

