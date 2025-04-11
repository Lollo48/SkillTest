// Copyright Villains, Inc. All Rights Reserved.


#include "BTDecorator_CustomLoop.h"

#include "BehaviorTree/BTCompositeNode.h"
#include "BehaviorTree/Composites/BTComposite_SimpleParallel.h"
#include "BehaviorTree/Decorators/BTDecorator_Loop.h"

UBTDecorator_CustomLoop::UBTDecorator_CustomLoop(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "RandomLoop";
	NumLoopsMin = 1;
	NumLoopsMax = 2;
	INIT_DECORATOR_NODE_NOTIFY_FLAGS();
 
	bAllowAbortNone = true;
	bAllowAbortLowerPri = true;
	bAllowAbortChildNodes = true;
	
}
 
void UBTDecorator_CustomLoop::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	FBTLoopDecoratorMemory* DecoratorMemory = GetNodeMemory<FBTLoopDecoratorMemory>(SearchData);
	FBTCompositeMemory* ParentMemory = GetParentNode()->GetNodeMemory<FBTCompositeMemory>(SearchData);
	const bool bIsSpecialNode = GetParentNode()->IsA(UBTComposite_SimpleParallel::StaticClass());
 
	if ((bIsSpecialNode && ParentMemory->CurrentChild == BTSpecialChild::NotInitialized) ||
		(!bIsSpecialNode && ParentMemory->CurrentChild != ChildIndex))
	{
		int32 numLoops = FMath::RandRange(NumLoopsMin, NumLoopsMax);
		DecoratorMemory->RemainingExecutions = IntCastChecked<uint8>(numLoops);
		DecoratorMemory->TimeStarted = GetWorld()->GetTimeSeconds();
	}
 
	bool bShouldLoop = false;
	if (DecoratorMemory->RemainingExecutions > 0)
	{
		DecoratorMemory->RemainingExecutions--;
	}
	bShouldLoop = DecoratorMemory->RemainingExecutions > 0;
 

	if (bShouldLoop)
	{
		GetParentNode()->SetChildOverride(SearchData, ChildIndex);
	}
}
 
FString UBTDecorator_CustomLoop::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: %d-%d loops"), *Super::GetStaticDescription(), NumLoopsMin, NumLoopsMax);
}
 
void UBTDecorator_CustomLoop::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);
 
	FBTLoopDecoratorMemory* DecoratorMemory = (FBTLoopDecoratorMemory*)NodeMemory;
	Values.Add(FString::Printf(TEXT("loops remaining: %d"), DecoratorMemory->RemainingExecutions));
}
 
uint16 UBTDecorator_CustomLoop::GetInstanceMemorySize() const
{
	return sizeof(FBTLoopDecoratorMemory);
}
 
void UBTDecorator_CustomLoop::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
	InitializeNodeMemory<FBTLoopDecoratorMemory>(NodeMemory, InitType);
}
 
void UBTDecorator_CustomLoop::CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const
{
	CleanupNodeMemory<FBTLoopDecoratorMemory>(NodeMemory, CleanupType);
}
 
#if WITH_EDITOR
 
FName UBTDecorator_CustomLoop::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Decorator.Loop.Icon");
}
 
#endif	