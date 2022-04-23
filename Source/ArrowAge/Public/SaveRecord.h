// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveRecord.generated.h"

/**
 * 
 */

UCLASS()
class ARROWAGE_API USaveRecord : public USaveGame
{
	GENERATED_BODY()

protected:
	TArray<TArray<FString>> Records;

public:

	USaveRecord();

	UPROPERTY(BlueprintReadOnly,Category="SaveRecord")
	FString Score;
	UPROPERTY(BlueprintReadOnly, Category = "SaveRecord")
	FString TimeUsed;

	void UpdateRecord(int32 S, float T);

};
