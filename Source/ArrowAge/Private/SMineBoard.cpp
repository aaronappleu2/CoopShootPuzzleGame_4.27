// Fill out your copyright notice in the Description page of Project Settings.


#include "SMineBoard.h"

#include "SGameMode.h"
#include "SMineBall.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASMineBoard::ASMineBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SizeLength = 400;
	SizeHeight = 400;
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetBoxExtent(FVector(50, SizeLength, SizeHeight));
	RootComponent = BoxComp;
	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(50, SizeLength, SizeHeight);
	DefaultRow = 5;
	DefaultCol = 5;
	BoomNum = 6;
	SetReplicates(true);
	InitialMap();

}

// Called when the game starts or when spawned
void ASMineBoard::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		//Spawn a default Weapon
		SpawnBalls();
		
	}
	
}

void ASMineBoard::InitialMap()
{
	TArray<int8> Temp;
	Temp.Init(BallTypes[0], DefaultCol);
	TypeMap.Init(Temp, DefaultRow);
	
	int8 GoalRow = FMath::RandRange(0, DefaultRow-1);
	int8 GoalCol = FMath::RandRange(0, DefaultCol-1);

	int8 ToSetBoom = BoomNum;

	TypeMap[GoalRow][GoalCol] = BallTypes[5];

	for(TArray<int8> Direct : Directions)
	{
		if(GoalRow+Direct[0]>=0 && GoalRow + Direct[0]< DefaultRow && GoalCol+Direct[1]>=0 && GoalCol + Direct[1]< DefaultCol)
		{
			TypeMap[GoalRow + Direct[0]][GoalCol + Direct[1]] = BallTypes[1];
			ToSetBoom--;
		}
	}

	// Set Random Booms
	
	while(ToSetBoom>0)
	{
		if(PutRandomBooms())
		{
			ToSetBoom--;
		}
	}

	//update TypeMap
	for (int i = 0; i < DefaultRow; ++i)
	{
		for (int j = 0; j < DefaultCol; ++j)
		{
			if(TypeMap[i][j]!= BallTypes[1])
			{
				TypeMap[i][j] = GetType(i, j);
			}
		}
	}


}

void ASMineBoard::SpawnBalls()
{
	Balls.Empty();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int i = 0; i < DefaultRow; ++i)
	{
		for (int j = 0; j < DefaultCol; ++j)
		{
			ASMineBall* CurrentBall = GetWorld()->SpawnActor<ASMineBall>(MineBallClass, FVector::ZeroVector, GetActorRotation(), SpawnParams);
			if (CurrentBall)
			{
				CurrentBall->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
				CurrentBall->SetActorRelativeLocation(FVector(50, (j - 2) * SizeLength / (DefaultCol / 2 + 1), (i - 2) * SizeHeight / (DefaultRow / 2 + 1)));
				CurrentBall->SetDefaultType(TypeMap[i][j]);
				Balls.Add(CurrentBall);
			}
		}
	}
}

bool ASMineBoard::PutRandomBooms()
{
	int8 GoalRow = FMath::RandRange(0, DefaultRow-1);
	int8 GoalCol = FMath::RandRange(0, DefaultCol-1);
	if(TypeMap[GoalRow][GoalCol]==0)
	{
		TypeMap[GoalRow][GoalCol] = BallTypes[1];
		for (TArray<int8> Direct : Directions)
		{
			if (GoalRow + Direct[0] >= 0 && GoalRow + Direct[0] < DefaultRow && GoalCol + Direct[1] >= 0 && GoalCol + Direct[1] < DefaultCol)
			{
				if(GetType(GoalRow + Direct[0], GoalCol + Direct[1])==BallTypes[5])
				{
					TypeMap[GoalRow][GoalCol] = 0;
					return false;
				}
			}
		}
		return true;
	}
	return false;

}

int8 ASMineBoard::GetType(int8 Row, int8 Col)
{
	int8 RoundBoom = 0;
	for (TArray<int8> Direct : Directions)
	{
		if (Row + Direct[0] >= 0 && Row + Direct[0] < DefaultRow && Col + Direct[1] >= 0 && Col + Direct[1] < DefaultCol)
		{
			if (TypeMap[Row + Direct[0]][Col + Direct[1]] == BallTypes[1])
			{
				RoundBoom++;
			}
		}
		else
		{
			RoundBoom++;
		}
	}
	return RoundBoom*5;
}

void ASMineBoard::ExplodeOneBall()
{
	

	
	if(HasAuthority())
	{
		for (ASMineBall* Ball : Balls)
		{
			if (Ball && !Ball->bExploded)
			{
				Ball->SelfDestruct();
				return;
			}
		}
		GetWorldTimerManager().ClearTimer(FTimerHandle_Explode);
		for (ASMineBall* Ball : Balls)
		{
			Ball->Destroy();
		}
		SpawnBalls();
	}
}


// Called every frame
void ASMineBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASMineBoard::Restart()
{
	if(HasAuthority())
	{
		TypeMap.Empty();
		InitialMap();
		GetWorldTimerManager().SetTimer(FTimerHandle_Explode, this, &ASMineBoard::ExplodeOneBall, 0.1f, true, 0.1f);
	}
	
	
	
}

void ASMineBoard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASMineBoard, Balls);
}

