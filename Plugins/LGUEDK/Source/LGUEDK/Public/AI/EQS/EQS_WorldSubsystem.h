// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EQS_Helper.h"
#include "NavigationSystem.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
#include "Data/EQSContainer.h"
#include "Subsystems/WorldSubsystem.h"
#include "EQS_WorldSubsystem.generated.h"

/**
 * 
 */
UCLASS(Abstract,Blueprintable,BlueprintType)
class LGUEDK_API UEQS_WorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

#pragma region Point Generation and Registration
	void GenerateGridPoints(const FVector& TargetLocation, float InGridRadius, float InStepSize,float ZStep);
	void RegisterEntity(AActor* InEntity);
	void UnregisterEntity(AActor* InEntity);
#pragma endregion
#pragma region  Method - Generic
	TArray<FVector> GetPoints(const FVector& TargetLocation,float InMinDistance,float InMaxDistance);
	FVector GetRandomExplorationPoint(const AActor* SelfEntity,const AActor* TargetIgnored, const FVector& InitialSpawnPosition, const FVector& CurrentPosition, float SearchRadius, bool bWantExplore);
	FVector GetBoidExplorationPoint(const AActor* SelfEntity, const FVector& InitialSpawnPosition,const FVector& CurrentPosition, float SearchRadius, bool bWantExplore);
	FVector GetAttackPoint(const AActor* SelfEntity,const AActor* TargetIgnored,const FVector& TargetLocation, float InMinDistance, float InMaxDistance,const float Margin);
	FVector GetChasingPoint(const AActor* SelfEntity,const AActor* TargetIgnored,const FVector& TargetLocation, float InMinDistance,float InMaxDistance);
#pragma endregion
#pragma region  Method - Area Based
	void SetRegion(AEQS_Helper* InEQSHelper, const FGameplayTag& InTag);
	TArray<FVector> GetAreaPoints(const FVector& TargetLocation, float InMinDistance, float InMaxDistance, const FGameplayTag& InTag);
	FVector GetRandomExplorationAreaPoint(const AActor* SelfEntity,const AActor* TargetIgnored,const FVector& InitialSpawnPosition, const FVector& CurrentPosition, float SearchRadius, bool bWantExplore, const FGameplayTag& InTag);
	FVector GetBoidExplorationAreaPoint(const AActor* SelfEntity, const FVector& InitialSpawnPosition,const FVector& CurrentPosition, float SearchRadius, bool bWantExplore, const FGameplayTag& InTag);
	FVector GetAttackAreaPoint(const AActor* SelfEntity,const AActor* TargetIgnored,const FVector& TargetLocation, float InMinDistance,float InMaxDistance, const float Margin, const FGameplayTag& InTag);
	FVector GetChasingAreaPoint(const AActor* SelfEntity,const AActor* TargetIgnored,const FVector& TargetLocation, float InMinDistance,float InMaxDistance);
#pragma endregion
protected:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;

	virtual void Init();
	void ClearRegisteredEntities();
	void ClearEQSHelper();

	void DistributePointsToRegions();

	UFUNCTION(BlueprintImplementableEvent,Category="WorldSubsystem")
	void InitBP();

private:

	UPROPERTY(EditDefaultsOnly, Category = "GameModes")
	TArray<TSubclassOf<AGameModeBase>>GameModes;

	UPROPERTY()
	TArray<FVector> ValidPoints;
	
	UPROPERTY()
	TMap<FGameplayTag, FEQSContainer> RegionPoints;

	UPROPERTY()
	TArray<AEQS_Helper*> EQSHelper;
	
	UPROPERTY()
	TArray<AActor*> RegisteredEntities;

	UPROPERTY(EditDefaultsOnly,Category="WorldSubsystem Settings")
	float DistanceFromNavmeshEdge = 100.0f; 

	UPROPERTY(EditDefaultsOnly,Category="WorldSubsystem Settings")
	float BoidViewRadius = 800.0f;         
	UPROPERTY(EditDefaultsOnly,Category="WorldSubsystem Settings")
	float BoidSeparationRadius = 300.0f;    
	UPROPERTY(EditDefaultsOnly,Category="WorldSubsystem Settings")
	float BoidWeightCohesion = 1.0f;
	UPROPERTY(EditDefaultsOnly,Category="WorldSubsystem Settings")
	float BoidWeightAlignment = 0.7f;
	UPROPERTY(EditDefaultsOnly,Category="WorldSubsystem Settings")
	float BoidWeightSeparation = 1.5f;

	UPROPERTY(EditDefaultsOnly,Category="WorldSubsystem ControllSettings")
	TEnumAsByte<ECollisionChannel> ObjectTypeChannelPointFree;

	UPROPERTY(EditDefaultsOnly,Category="WorldSubsystem ControllSettings")
	TEnumAsByte<ECollisionChannel> CollisionChannelIsHittingSomething;

	UPROPERTY(EditDefaultsOnly,Category="WorldSubsystem ControllSettings")
	float DistanceBetweenEnemyRadius;

	UPROPERTY(EditDefaultsOnly,Category="WorldSubsystem Debug")
	bool bWantUseAreasPoint;
	UPROPERTY(EditDefaultsOnly,Category="WorldSubsystem Debug")
	bool bShowDebug = false;

	//----BOID SETTINGS----
	// POCO ALLINEATI
	// BoidViewRadius = 600.0f;           // Visione ristretta: pochi vicini rilevati
	// BoidSeparationRadius = 400.0f;     // Ampia zona di separazione per evitare collisioni
	// BoidWeightCohesion = 0.3f;         // Bassa tendenza a raggrupparsi
	// BoidWeightAlignment = 0.1f;        // Quasi nullo allineamento: ognuno ha la sua direzione
	// BoidWeightSeparation = 2.5f;       // Forte repulsione dagli altri

	//BILANCIATI
	// BoidViewRadius = 800.0f;           
	// BoidSeparationRadius = 300.0f;     
	// BoidWeightCohesion = 1.0f;         
	// BoidWeightAlignment = 0.7f;        
	// BoidWeightSeparation = 1.5f;

	//MOLTO ALLINEATI
	// BoidViewRadius = 1000.0f;          // Ampio raggio di percezione
	// BoidSeparationRadius = 200.0f;     // Distanza minima prima di allontanarsi
	// BoidWeightCohesion = 2.0f;         // Forte tendenza a restare uniti
	// BoidWeightAlignment = 2.0f;        // Altissima sincronizzazione direzionale
	// BoidWeightSeparation = 1.0f;       // Separazione sufficiente a evitare collisioni, ma non eccessiva

#pragma region Controll Functions
	bool IsInRange(const FVector& TargetPosition,const FVector& CurrentPosition, const float InMinDistance, const float InMaxDistance);

	bool IsPointFarFromNavEdge(const UNavigationSystemV1* NavSystem, const FVector& Point,const ANavigationData* NavData);

	bool IsHittingSomething(const AActor* SelfIgnored,const AActor* TargetIgnored, const FVector& Start, const FVector& End) const;

	bool IsPointFree(const AActor* SelfIgnored,const AActor* TargetIgnored,const FVector& Point) const;

	FGameplayTag GetTagFromPoint(const FVector& Point);
#pragma endregion
	
};
