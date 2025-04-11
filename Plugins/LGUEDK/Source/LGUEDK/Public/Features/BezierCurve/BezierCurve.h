// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BezierCurve.generated.h"

UCLASS()
class LGUEDK_API ABezierCurve : public AActor
{
	GENERATED_BODY()

public:
	
	ABezierCurve();

	
protected:
	
	virtual void BeginPlay() override;
	
	virtual void OnConstruction(const FTransform& Transform) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bezier Settings", meta = (MakeEditWidget = true))
	FVector StartLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bezier Settings", meta = (MakeEditWidget = true))
	FVector MiddleLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bezier Settings", meta = (MakeEditWidget = true))
	FVector EndLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bezier Settings")
	int32 CurveResolution = 20;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bezier Settings")
	float Speed;

	
private:

	FVector WorldStartPosition;
	FVector WorldMiddlePosition;
	FVector WorldEndPosition;
	float TimeElapsed;
	
	FVector CalculateBezierPoint(float Alpha) const;
	
	void DrawCurve() const ;
	
	void AnimateDebugBox(float DeltaTime);
	
	void CalculateStartEndLocations();
	
};


