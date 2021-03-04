// Shoot Them Up Game. All rights reserved!


#include "AI/EQS/EnvQueryTest_PickupPickable.h"

#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "Pickups/STUBasePickup.h"

UEnvQueryTest_PickupPickable::UEnvQueryTest_PickupPickable(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
	SetWorkOnFloatValues(false);
}

void UEnvQueryTest_PickupPickable::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* DataOwner = QueryInstance.Owner.Get();
	BoolValue.BindData(DataOwner, QueryInstance.QueryID);
	const bool WannaBeTaken = BoolValue.GetValue();
	
	for(FEnvQueryInstance::ItemIterator It(this, QueryInstance); IsEditorOnly(); ++It)
	{
		AActor* ItemActor = GetItemActor(QueryInstance, It.GetIndex());
		const auto PickupActor = Cast<ASTUBasePickup>(ItemActor);
		if(!PickupActor) continue;

		const auto CouldBeTaken = PickupActor->CouldBeTaken();

		/*if(CouldBeTaken)
		{
			It.SetScore(TestPurpose, FilterType, true, true);
		}
		else
			It.ForceItemState(EEnvItemStatus::Failed);*/
		It.SetScore(TestPurpose, FilterType, CouldBeTaken, WannaBeTaken);
	}
}
