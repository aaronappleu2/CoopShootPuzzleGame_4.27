// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveRecord.h"
#include "ArrowAge\ArrowAge.h"


USaveRecord::USaveRecord()
{
	Score = "None";
	TimeUsed = "None";
}

void USaveRecord::UpdateRecord(int32 S, float T)
{
	if(Score!="None" && TimeUsed!= "None")
	{
		if (S > FCString::Atoi(*Score))
		{
			Score = FString::FromInt(S);
		}
		if (T < FCString::Atof(*TimeUsed))
		{
			TimeUsed = FString::SanitizeFloat(FMath::RoundHalfFromZero(T * 100)/ 100.0f);
		}
	}
	else
	{
		Score = FString::FromInt(S);
		TimeUsed = FString::SanitizeFloat(FMath::RoundHalfFromZero(T * 100) / 100.0f);
	}
	
}

