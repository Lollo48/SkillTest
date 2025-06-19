// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "NPCBase.generated.h"

class ANPCBaseController;

UCLASS()
class LGUEDK_API ANPCBase : public ACharacter
{
	GENERATED_BODY()

public:
	
	ANPCBase();

	UFUNCTION(Blueprintable,BlueprintCallable)
	UBehaviorTree* GetBehaviorTree() const;

	UFUNCTION(BlueprintCallable, Category = "Init")
	virtual void Init();

	UFUNCTION(BlueprintCallable, Category = "EnableEntity")
	virtual void EnableEntity();

	UFUNCTION(BlueprintCallable, Category = "EnableEntity")
	virtual void EnableEntityEffect() {EnableEntityEffectBP();}

	UFUNCTION(BlueprintCallable, Category = "DisableEntity")
	virtual void DisableEntity();

	UFUNCTION(BlueprintCallable, Category = "DisableEntity")
	virtual void DisableEntityEffect() {DisableEntityEffectBP();}

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Initialize")
	bool GetIsInitialize() const {return bIsInitialize;}

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "EnableEntity")
	bool GetIsEnable() const {return bIsEnable;}

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "EntityController")
	ANPCBaseController* GetEntityController() const;
	
	UFUNCTION(BlueprintCallable, Category = "InitTransform")
	virtual void ResetEntityTransform();

	UFUNCTION(BlueprintCallable, Category = "InitCollision")
	virtual void ResetEntityCollision();

protected:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI",meta=(AllowPrivateAccess="true"))
	UBehaviorTree* BehaviorTree;

	UPROPERTY()
	FVector MeshLocation;
	UPROPERTY()
	FRotator MeshRotation;
	UPROPERTY()
	FVector MeshScale;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|CollisionProfileName")
	FName EnemyMeshCollisionProfileName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|CollisionProfileName")
	FName EnemyCapsuleCollisionProfileName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|CollisionProfileName")
	TEnumAsByte<ECollisionChannel> CollisionCapsuleChannel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|CollisionProfileName")
	TEnumAsByte<ECollisionChannel> CollisionMeshChannel;
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Initialize")
	void SetIsInitialize(bool bInit) {bIsInitialize = bInit;}
	
	UFUNCTION(BlueprintCallable, Category = "EnableEntity")
	void SetIsEnable(bool bEnable) {bIsEnable = bEnable;}

	UFUNCTION(BlueprintImplementableEvent,Category = "Init")
	void InitBP();
	
	UFUNCTION(BlueprintImplementableEvent,Category = "EnableEntity")
	void EnableEntityBP();

	UFUNCTION(BlueprintImplementableEvent,Category = "EnableEntity")
	void EnableEntityEffectBP();
	
	UFUNCTION(BlueprintImplementableEvent,Category = "DisableEntity")
	void DisableEntityBP();
	
	UFUNCTION(BlueprintImplementableEvent,Category = "DisableEntity")
	void DisableEntityEffectBP();

private:

	UPROPERTY()
	ANPCBaseController* BaseController;
	
	UPROPERTY()
	bool bIsInitialize;

	UPROPERTY()
	bool bIsEnable;

};
