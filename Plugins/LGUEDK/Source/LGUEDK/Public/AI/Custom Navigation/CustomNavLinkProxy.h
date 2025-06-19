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
	bool FindValidDestination(const FVector& OriginalDestination);

	UFUNCTION(BlueprintCallable, Category = "Custom Nav Link")
	void EnableCollider(AActor* ActorToEnableCollider);
	
protected:
	
	UPROPERTY(BlueprintReadOnly,Category="WorldSubsystem ControllSettings")
	FVector ValidPosition;
	
private:
	
	UPROPERTY(EditAnywhere, Category = "Custom Nav Link")
	TEnumAsByte<ENavLinkDirection::Type> AINavLinkDirection;

	UPROPERTY(EditDefaultsOnly,Category="WorldSubsystem ControllSettings")
	float DistanceBetweenEnemyRadius;

	UPROPERTY(EditDefaultsOnly,Category="WorldSubsystem ControllSettings")
	TEnumAsByte<ECollisionChannel> ObjectTypeChannelPointFree;

	bool IsPointFree(const FVector& Point) const;
	
	bool bIsOccupied;
	
	FTimerHandle OccupyTimerHandle;
	
	void Release();

};
