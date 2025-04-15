// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Manager/Spline/MySpline.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
#include "NPCBaseStateSplineEnemy.generated.h"



UCLASS()
class LGUEDK_API ANPCBaseStateSplineEnemy : public ANPCBaseStateEnemy
{
	GENERATED_BODY()

public:

	ANPCBaseStateSplineEnemy();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Spline")
	AMySpline* GetMySpline() const;
	
protected:

	UPROPERTY(EditAnywhere, Category = "Spline")
	AMySpline* MySpline;
	
	virtual void BeginPlay() override;

};
