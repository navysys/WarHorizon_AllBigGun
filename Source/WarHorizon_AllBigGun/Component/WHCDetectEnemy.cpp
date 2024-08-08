// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/WHCDetectEnemy.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UWHCDetectEnemy::UWHCDetectEnemy()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UWHCDetectEnemy::BeginPlay()
{
	Super::BeginPlay();

	PawnOwner = Cast<APawn>(GetOwner());

	GetWorld()->GetTimerManager().SetTimer(BattleShipTimerHandle, this, &UWHCDetectEnemy::DetectBattleShip, 0.5f, true);
	//GetWorld()->GetTimerManager().SetTimer(AircraftTimerHandle, this, &UWHCDetectEnemy::DetectAircraft, 0.5f, true);
}


// Called every frame
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
		//UE_LOG(LogTemp, Warning, TEXT("Detect Enemy Comp -> Detect"));
		TArray<APawn*> ArrayPawn;
		for (auto const& OverlapResult : BOverlapResults)
		{
			if (OverlapResult.GetActor()->ActorHasTag(TEXT("BattleShip")))
			{
				APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
				ArrayPawn.Emplace(Pawn);
			}
		}
		DetectedBattleShips = ArrayPawn;
		if (DetectedBattleShips.Num() > 1)
		{
			SortingArrayToDistance(DetectedBattleShips);
			DrawDebugSphere(GetWorld(), Center, BRadius, 20, FColor::Green, false, 0.5f);
		}
		else 
		{
			DrawDebugSphere(GetWorld(), Center, BRadius, 20, FColor::Red, false, 0.5f);
		}
	}
	else
	{
		DrawDebugSphere(GetWorld(), Center, BRadius, 20, FColor::Red, false, 0.5f);
	}
}

void UWHCDetectEnemy::DetectAircraft()
{
	TArray<FOverlapResult> AOverlapResults;
	FCollisionQueryParams CollisionQueryParam;
	CollisionQueryParam.bTraceComplex = true;
	CollisionQueryParam.AddIgnoredActor(PawnOwner);

	FVector Center = PawnOwner->GetActorLocation();
	Center.Z = 16000.0f;

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
		DetectedAircrafts = ArrayPawn;
		if (DetectedAircrafts.Num() > 1)
		{
			SortingArrayToDistance(DetectedAircrafts);
		}
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

