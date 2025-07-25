
#include "Turret/WHNormalTurretBase.h"
#include "Kismet/GameplayStatics.h"
#include "Turret/WHShell.h"
#include "NiagaraActor.h"
#include "NiagaraComponent.h"


AWHNormalTurretBase::AWHNormalTurretBase()
{

}

void AWHNormalTurretBase::BeginPlay()
{
	Super::BeginPlay();

	// �л굵 ���� �ڵ� ���߿��� ��� �����ֵ��� ����
	uint8 MuzzleCount = MuzzleComps.Num(); //������
	float DispersionAngle = 0.0f;
	if (MuzzleCount == 4)
	{
		DispersionAngle = 7.5f;
	}
	else if (MuzzleCount == 3)
	{
		DispersionAngle = 5.0f;
	}
	else if (MuzzleCount == 2)
	{
		DispersionAngle = 2.5f;
	}

	for (int i = 1; i <= MuzzleCount; i++)
	{
		Dispersion.Emplace((DispersionAngle / (MuzzleCount - 1)) * (i - ((MuzzleCount + 1) / 2)));
	}
}

void AWHNormalTurretBase::Fire()
{
	Super::Fire();

	bIsFireReady = false;
	BeforeFireTime = 0.0f;

	FVector MuzzleLocation = MuzzleComps[0]->GetComponentLocation();
	FVector TargetLoc = MuzzleLocation + SkeletalMeshComp->GetForwardVector() * TargetData.Distance;  // Ÿ�� ����.
	float ArcValue = 0.65f;                       // ArcParam (0.0-1.0) ��絵 ���߿� Ÿ�� �����Ϳ��� �����ϵ��� ����
	float GravityZ = GetWorld()->GetGravityZ();
	if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, ShellVelocityVector, MuzzleLocation, TargetLoc, GravityZ, ArcValue))
	{
		FRotator LaunchAngle = ShellVelocityVector.Rotation();
		ShellLaunchAngle = LaunchAngle.Pitch;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ShellVelocity = ShellVelocityVector.Size();

	// ��ź �߻� ����
	FRotator Rot = SkeletalMeshComp->GetComponentRotation() + FRotator(ShellLaunchAngle, 0.0f, 0.0f);
	if (MuzzleComps.Num() > 0)
	{
		if (Dispersion.Num() > 0)
		{
			for (int i = 0; i < MuzzleComps.Num(); i++)
			{
				AWHShell* SpawnShell = GetWorld()->SpawnActor<AWHShell>(Shell, MuzzleComps[i]->GetComponentLocation(), Rot + FRotator(0.0f, Dispersion[i], 0.0f), SpawnParams);
			}
		}
	}
}
