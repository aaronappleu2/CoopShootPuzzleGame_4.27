// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

class USaveRecord;
enum class EWaveState : uint8;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor,AController*, KillerController);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChessSolved, AActor*, Solver, AController*, Controller);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExploded);
/**
 * 
 */
UCLASS()
class ARROWAGE_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()


protected:

	FTimerHandle TimerHandle_BotSpawner;

	FTimerHandle TimerHandle_NextWaveStart;

	// Bots to spawn in single wave
	int32 NrOfBotsToSpawn;

	int32 WaveCount;

	float StartTime;

	UPROPERTY(EditDefaultsOnly, Category="GameMode")
	float TimeBetweenWaves;

	USaveRecord* GameRecord;


protected:

	//Hook for BP to spawn a single bot
	UFUNCTION(BlueprintImplementableEvent, Category="GameMode")
	void SpawnNewBot();

	void SpawnBotTimerElapsed();

	// Start spawning bots
	void StartWave();

	// Stop Spawning bots
	void EndWave();

	// Set timer for next startwave
	void PrepareForNextWave();

	UFUNCTION()
	void CheckWaveState();

	void CheckAnyPlayerAlive();

	void GameOver();

	void SetWaveState(EWaveState NewState);

	void RestartDiedPlayers();

	UFUNCTION()
	void GameSucceed(AActor* Solver, AController* Controller);

public:

	ASGameMode();

	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintAssignable, Category="GameMode")
	FOnActorKilled OnActorKilled;

	UPROPERTY()
	FChessSolved ChessSolved;

	UPROPERTY()
	FExploded ChessExploded;

	int32 Score;

	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	void AddTotalScore(float ScoreDelta);

};
