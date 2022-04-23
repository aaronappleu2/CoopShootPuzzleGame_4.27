// Fill out your copyright notice in the Description page of Project Settings.


#include "SMineBall.h"

#include "DrawDebugHelpers.h"
#include "SCharacter.h"
#include "SGameMode.h"
#include "SHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundCue.h"
#include "SMineBoard.h"

static int32 DebugMineBallDrawing = 0;
FAutoConsoleVariableRef CVARDebugMineBallDrawing(
	TEXT("COOP.DebugMineBall"),
	DebugMineBallDrawing,
	TEXT("Draw Debug Lines for MineBall"),
	ECVF_Cheat);

// Sets default values
ASMineBall::ASMineBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetSimulatePhysics(false);
	RootComponent = MeshComp;

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	HealthComp->SetDefaultHealth(10.0f);

	HealthComp->OnHealthChanged.AddDynamic(this, &ASMineBall::HandleTakeDamage);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(1000);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetupAttachment(RootComponent);

	ExplosionRadius = 100;
	ExplosionDamage = 40;
	DefaultType = 0;

	SelfDamageInterval = 0.25f;

	SetReplicates(true);

}

// Called when the game starts or when spawned
void ASMineBall::BeginPlay()
{
	Super::BeginPlay();
	
}


void ASMineBall::HandleTakeDamage(USHealthComponent* OwningHealthComponent, float Health, float HealthDelta,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	// explode on hitpoints == 0

	// @TODO: Pulse the material
	if (Health <= 0.0f)
	{
		if (MatInst == nullptr)
		{
			MatInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));
		}
		if (MatInst)
		{
			MatInst->SetScalarParameterValue("BallType", DefaultType);
		}
		if (DefaultType == 1)
		{
			if (HasAuthority())
			{
				ASGameMode* GM = Cast<ASGameMode>(GetWorld()->GetAuthGameMode());
				GM->ChessExploded.Broadcast();
				SelfDestruct();
			}

			
		}
		else if(DefaultType==20)
		{
			if(HasAuthority())
			{
				ASGameMode* GM = Cast<ASGameMode>(GetWorld()->GetAuthGameMode());
				GM->ChessSolved.Broadcast(DamageCauser, InstigatedBy);
			}
		}
	}

}

void ASMineBall::SelfDestruct()
{
	if (bExploded)
	{
		return;
	}
	bExploded = true;

	if(HasAuthority())
	{
		OnRep_Exploded();
	}

	ASMineBoard* MBoard = Cast<ASMineBoard>(GetAttachParentActor());
	if(MBoard)
	{
		MBoard->Restart();
	}
}


void ASMineBall::SetDefaultType(int8 BallType)
{
	DefaultType = BallType;
}


void ASMineBall::OnRep_Exploded()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	// play explode sound
	UGameplayStatics::PlaySoundAtLocation(this, ExplodeSound, GetActorLocation());

	MeshComp->SetVisibility(false, true);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called every frame
void ASMineBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ASMineBall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASMineBall, DefaultType);
	DOREPLIFETIME(ASMineBall, bExploded);
}

