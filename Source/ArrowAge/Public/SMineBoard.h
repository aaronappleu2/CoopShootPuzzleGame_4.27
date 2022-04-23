// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "SMineBoard.generated.h"
class ASMineBall;
class UBoxComponent;
class UDecalComponent;
UCLASS()
class ARROWAGE_API ASMineBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASMineBoard();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UDecalComponent* DecalComp;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectileWeapon")
	TSubclassOf<AActor> MineBallClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<INT8> BallTypes = { 0,1,5,10,15,20 };
	TArray<TArray<int8>> Directions = { {1,0},{-1,0},{0,1},{0,-1} };

	UPROPERTY(Replicated)
	TArray<ASMineBall*> Balls;

	TArray<TArray<int8>> TypeMap;

	FTimerHandle FTimerHandle_Explode;

	int16 DefaultRow;
	int16 DefaultCol;
	int16 BoomNum;

	float SizeLength;
	float SizeHeight;

	void InitialMap();

	void SpawnBalls();

	bool PutRandomBooms();
	int8  GetType(int8 Row, int8 Col);

	UFUNCTION()
	void ExplodeOneBall();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Restart();



};
