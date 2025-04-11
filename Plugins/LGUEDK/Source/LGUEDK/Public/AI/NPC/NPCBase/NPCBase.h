// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NPCBaseController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "NPCBase.generated.h"

UCLASS()
class LGUEDK_API ANPCBase : public ACharacter
{
	GENERATED_BODY()

public:
	ANPCBase();

	UFUNCTION(Blueprintable,BlueprintCallable)
	UBehaviorTree* GetBehaviorTree() const;
	
protected:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "AI",meta=(AllowPrivateAccess="true"))
	UBehaviorTree* BehaviorTree;
	
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void Init(){InitBP();};
	
	UFUNCTION(BlueprintImplementableEvent,Category = "Init")
	void InitBP();

	UFUNCTION(BlueprintCallable, Category = "EnableEntity")
	virtual void EnableEntity();
	
	UFUNCTION(BlueprintImplementableEvent,Category = "EnableEntity")
	void EnableEntityBP();

	UFUNCTION(BlueprintImplementableEvent,Category = "EnableEntity")
	void EnableEntityEffectBP();
	
	UFUNCTION(BlueprintCallable, Category = "DisableEntity")
	virtual void DisableEntity();
	
	UFUNCTION(BlueprintImplementableEvent,Category = "DisableEntity")
	void DisableEntityBP();
	
	UFUNCTION(BlueprintImplementableEvent,Category = "DisableEntity")
	void DisableEntityEffectBP();
	
};
