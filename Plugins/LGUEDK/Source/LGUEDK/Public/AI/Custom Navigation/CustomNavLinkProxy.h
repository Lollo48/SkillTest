// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavLinkProxy.h"
#include "NavLinkCustomComponent.h" 
#include "CustomNavLinkProxy.generated.h"

UCLASS()
class LGUEDK_API ACustomNavLinkProxy : public ANavLinkProxy
{
	GENERATED_BODY()

public:
	ACustomNavLinkProxy();
	
	virtual void BeginPlay() override;

	
	UFUNCTION(BlueprintPure, Category =  "Custom Nav Link")
	bool IsAvailable() const;
	
	UFUNCTION(BlueprintCallable, Category =  "Custom Nav Link")
	void MarkAsOccupied(float OccupyDuration = 1.5f);
	
	UFUNCTION(BlueprintCallable, Category = "Custom Nav Link")
	FVector FindValidDestination(const FVector& OriginalDestination,const int32& MaxAttempts,bool& bDebug);
	
private:
	UPROPERTY(EditAnywhere, Category = "Custom Nav Link")
	TEnumAsByte<ENavLinkDirection::Type> AINavLinkDirection;
	
	bool bIsOccupied;
	
	
	FTimerHandle OccupyTimerHandle;

	float JumpHeightRequired;
	
	UFUNCTION(BlueprintCallable, Category = "Custom Nav Link")
	bool CheckJumpHeightRequired(AActor* Pawn) const;
	
	void Release();

};
