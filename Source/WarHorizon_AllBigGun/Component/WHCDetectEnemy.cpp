
#include "Component/WHCDetectEnemy.h"
#include "DrawDebugHelpers.h"


UWHCDetectEnemy::UWHCDetectEnemy()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWHCDetectEnemy::BeginPlay()
{
	Super::BeginPlay();

	PawnOwner = Cast<APawn>(GetOwner());

	//GetWorld()->GetTimerManager().SetTimer(BattleShipTimerHandle, this, &UWHCDetectEnemy::DetectBattleShip, 0.5f, true);
	GetWorld()->GetTimerManager().SetTimer(AircraftTimerHandle, this, &UWHCDetectEnemy::DetectAircraft, 0.5f, true);
}

void UWHCDetectEnemy::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void UWHCDetectEnemy::InitDetectEnemyComponent(uint8 TeamInt)
{
	if (TeamInt == 1)
	{
		BPresetName = FName("TeamABattleShipPreset");
		APresetName = FName("TeamAAircraftPreset");
	}
	else if (TeamInt == 2)
	{
		BPresetName = FName("TeamBBattleShipPreset");
		APresetName = FName("TeamBAircraftPreset");
	}
	else if (TeamInt == 3)
	{
		// 중립 유닛일 경우를 가정
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Detect Enemy Comp -> InitDetectEnemyComponent : 알 수 없는 TeamInt"));
	}
}

void UWHCDetectEnemy::DetectBattleShip()
{
	TArray<FOverlapResult> BOverlapResults;
	FCollisionQueryParams CollisionQueryParam;
	CollisionQueryParam.bTraceComplex = true;
	CollisionQueryParam.AddIgnoredActor(PawnOwner);

	FVector Center = PawnOwner->GetActorLocation();

	bool bResult = GetWorld()->OverlapMultiByProfile(BOverlapResults, Center, FQuat::Identity, BPresetName, FCollisionShape::MakeSphere(BRadius), CollisionQueryParam);
	if (bResult)
	{
		TArray<APawn*> ArrayPawn;
		for (auto const& OverlapResult : BOverlapResults)
		{
			if (OverlapResult.GetActor()->ActorHasTag(TEXT("BattleShip")))
			{
				APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
				ArrayPawn.Emplace(Pawn);
			}
		}
		if (ArrayPawn.Num() > 1)
		{
			SortingArrayToDistance(ArrayPawn);
			//DrawDebugSphere(GetWorld(), Center, BRadius, 20, FColor::Green, false, 0.5f);
		}
		else 
		{
			//DrawDebugSphere(GetWorld(), Center, BRadius, 20, FColor::Red, false, 0.5f);
		}
		*DetectedBattleShips = ArrayPawn;
	}
	else
	{
		//DrawDebugSphere(GetWorld(), Center, BRadius, 20, FColor::Red, false, 0.5f);
	}
}

void UWHCDetectEnemy::DetectAircraft()
{
	TArray<FOverlapResult> AOverlapResults;
	FCollisionQueryParams CollisionQueryParam;
	CollisionQueryParam.bTraceComplex = true;
	CollisionQueryParam.AddIgnoredActor(PawnOwner);

	FVector Center = PawnOwner->GetActorLocation();

	bool bResult = GetWorld()->OverlapMultiByProfile(AOverlapResults, Center, FQuat::Identity, APresetName, FCollisionShape::MakeSphere(ARadius), CollisionQueryParam);
	if (bResult)
	{
		TArray<APawn*> ArrayPawn;
		for (auto const& OverlapResult : AOverlapResults)
		{
			if (OverlapResult.GetActor()->ActorHasTag(TEXT("Aircraft")))
			{
				APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
				ArrayPawn.Emplace(Pawn);
				DrawDebugSphere(GetWorld(), Center, ARadius, 20, FColor::Green, false, 0.5f);
			}
		}
		if (ArrayPawn.Num() > 1)
		{
			SortingArrayToDistance(ArrayPawn);
		}
		*DetectedAircrafts = ArrayPawn;
	}
	else
	{
		DrawDebugSphere(GetWorld(), Center, ARadius, 20, FColor::Red, false, 0.5f);
	}
}

void UWHCDetectEnemy::SortingArrayToDistance(TArray<APawn*> ArrayPawn)
{
	ArrayPawn.Sort([&](const APawn& A, const APawn& B) { return A.GetDistanceTo(PawnOwner) < B.GetDistanceTo(PawnOwner); });
}

