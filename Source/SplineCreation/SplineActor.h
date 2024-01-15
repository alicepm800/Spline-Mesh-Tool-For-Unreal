// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "SplineActor.generated.h"

UENUM(BlueprintType)
enum class ESplineMeshType: uint8
{
	DEFAULT UMETA(DisplayName = "Default Mesh"),
	START UMETA(DisplayName = "Starting Mesh"),
	END UMETA(DisplayName = "Ending Mesh"),
};

USTRUCT(BlueprintType)
struct FSplineMeshDetails : public FTableRowBase
{
	GENERATED_BODY()

	//Allows user to add their choice of mesh to a spline in Unreal
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	UStaticMesh* Mesh;

	//Exposes spline's axis as an Enum to the Blueprint's details panel
	//Allows the user to quickly change the forward axis of the meshes on the spline (is the forward axis on the X, Y, or Z axis?)
	//This updates all meshes on the spline
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	TEnumAsByte<ESplineMeshAxis::Type>ForwardAxis;

	//Allows the user to set the default material for all spline meshes in Blueprints
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	class UMaterialInterface* DefaultMaterial;

	//Allows the user to set a material that is not the default material for spline meshes in Blueprints
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	class UMaterialInterface* AlternativeMaterial;

	//Initialise forward axis on X axis
	FSplineMeshDetails() : ForwardAxis(ESplineMeshAxis::Type::X)
	{

	}

	
};

UCLASS()
class SPLINECREATION_API ASplineActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplineActor();

	void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Spline")
	USplineComponent* SplineComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spline")
	TMap<ESplineMeshType, FSplineMeshDetails> SplineMeshMap;
};
