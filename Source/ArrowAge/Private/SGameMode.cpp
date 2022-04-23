// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameMode.h"
#include "EngineUtils.h"
#include "SaveRecord.h"
#include "SHealthComponent.h"
#include "SGameState.h"
#include "SPlayerState.h"
#include "Kismet/GameplayStatics.h"


ASGameMode::ASGameMode()
{
	GameStateClass = ASGameState::StaticClass();
	PlayerStateClass = ASPlayerState::StaticClass();
	TimeBetweenWaves = 2.0f;
	WaveCount = 0;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
	Score = 0;
	ChessSolved.AddDynamic(this, &ASGameMode::GameSucceed);
	ChessExploded.AddDynamic(this, &ASGameMode::CheckWaveState);
	bUseSeamlessTravel = true;
}

void ASGameMode::StartWave()
{
	WaveCount++;

	NrOfBotsToSpawn = WaveCount;

	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &ASGameMode::SpawnBotTimerElapsed, 1.0f, true, 0.0f);

	SetWaveState(EWaveState::WaveInProgress);
}

void ASGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);

	SetWaveState(EWaveState::WaitingToComplete);

}

void ASGameMode::PrepareForNextWave()
{

	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &ASGameMode::StartWave, TimeBetweenWaves, false);

	SetWaveState(EWaveState::WaitingToStart);

	RestartDiedPlayers();
}

void ASGameMode::CheckWaveState()
{
	bool bIsPreparingForWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);

	if(NrOfBotsToSpawn>0 || bIsPreparingForWave)
	{
		return;
	}
	
	SetWaveState(EWaveState::WaveComplete);
	PrepareForNextWave();
	
}

void ASGameMode::CheckAnyPlayerAlive()
{
	for(APlayerController* PC : TActorRange<APlayerController>(GetWorld()))
	{
		if(PC && PC->GetPawn())
		{
			APawn* MyPawn = PC->GetPawn();
			USHealthComponent* HealthComp = Cast<USHealthComponent>(MyPawn->GetComponentByClass(USHealthComponent::StaticClass()));
			if(ensure(HealthComp)&& HealthComp->GetHealth() > 0.0f)
			{
				return;
			}
		}
	}
	// No player alive
	GameOver();
}

void ASGameMode::GameOver()
{
	EndWave();

	SetWaveState(EWaveState::GameOver);

	UE_LOG(LogTemp, Log, TEXT("Game Over! players died!"));
}

void ASGameMode::SetWaveState(EWaveState NewState)
{
	ASGameState* GS = GetGameState<ASGameState>();
	if(ensure(GS))
	{
		GS->SetWaveState(NewState);
	}
}

void ASGameMode::RestartDiedPlayers()
{
	for (APlayerController* PC : TActorRange<APlayerController>(GetWorld()))
	{
		if (PC && PC->GetPawn()==nullptr)
		{
			RestartPlayer(PC);
		}
	}
}

void ASGameMode::GameSucceed(AActor* Solver, AController* Controller)
{
	EndWave();

	SetWaveState(EWaveState::GameSucceed);

	UE_LOG(LogTemp, Log, TEXT("Game Succeed"));

	if (GameRecord = Cast<USaveRecord>(UGameplayStatics::LoadGameFromSlot("1", 1)))
	{
		GameRecord->UpdateRecord(Score, UGameplayStatics::GetRealTimeSeconds(this) - StartTime);
	}
	else
	{
		GameRecord = Cast<USaveRecord>(UGameplayStatics::CreateSaveGameObject(USaveRecord::StaticClass()));
	}

	if (UGameplayStatics::SaveGameToSlot(GameRecord, "1", 1))
	{
		UE_LOG(LogTemp, Log, TEXT("Record Saved"));
	}



}


void ASGameMode::StartPlay()
{
	Super::StartPlay();
	PrepareForNextWave();
	StartTime = UGameplayStatics::GetRealTimeSeconds(this);
}


void ASGameMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();
	NrOfBotsToSpawn--;

	if(NrOfBotsToSpawn<=0)
	{
		EndWave();
	}
}

void ASGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckAnyPlayerAlive();
}

void ASGameMode::AddTotalScore(float ScoreDelta)
{
	Score += ScoreDelta;
}
