// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AI/Manager/WaveSystem/Subsystem/WaveManagerSubsystem.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "EQS_Helper.generated.h"

UCLASS()
class LGUEDK_API AEQS_Helper : public AActor
{
	GENERATED_BODY()

public:

	AEQS_Helper();

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetRegionTag() const { return RegionTag; }
	UFUNCTION(BlueprintCallable)
	bool IsInsideRegion(const FVector& Point) const;
	
protected:
	
	UPROPERTY()
	TArray<AActor*> ActorsInsideOnRegion;
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void UpdateRegionEntity(AActor* InActor);

private:
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= "WorldSubsystemSettings", meta = (AllowPrivateAccess = "true"))
	FGameplayTag RegionTag;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= "WorldSubsystemSettings", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> InfluenceBox;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= "WorldSubsystemSettings", meta = (AllowPrivateAccess = "true"))
	bool bShowDebug;

	UFUNCTION(BlueprintCallable)
	void RemoveInsideActor(AActor* InActor);
	UFUNCTION(BlueprintCallable)
	void AddInsideActor(AActor* InActor);

};
