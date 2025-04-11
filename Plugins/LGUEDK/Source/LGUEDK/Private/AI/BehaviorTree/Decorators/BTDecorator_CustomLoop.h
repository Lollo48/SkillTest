// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CustomLoop.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTDecorator_CustomLoop : public UBTDecorator
{
	GENERATED_UCLASS_BODY()
 
	/** number of executions */
	UPROPERTY(Category=Decorator, EditAnywhere, meta=(ClampMin="1", ClampMax="50"))
	int32 NumLoopsMin;
 
	UPROPERTY(Category=Decorator, EditAnywhere, meta=(ClampMin="1", ClampMax="50"))
	int32 NumLoopsMax;
 
	virtual uint16 GetInstanceMemorySize() const override;
	virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
	virtual void CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const override;
	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	virtual FString GetStaticDescription() const override;
 
#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif // WITH_EDITOR
 
protected:
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;
};

