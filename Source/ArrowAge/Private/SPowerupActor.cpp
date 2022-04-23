// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor.h"
#include "SCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASPowerupActor::ASPowerupActor()
{
	PowerupInterval = 0.0f;
	TotalNrOfTicks = 0;
	bReplicates = true;

	bIsPowerupActive = false;
}


void ASPowerupActor::OnTickPowerup()
{
	TicksProcessed++;

	OnPowerupTicked();

	if(TicksProcessed>=TotalNrOfTicks)
	{
		OnExpired();

		bIsPowerupActive = false;

		Onrep_PowerupActive();

		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
	}
}

void ASPowerupActor::Onrep_PowerupActive()
{
	OnPowerupActiveChanged(bIsPowerupActive);
}

void ASPowerupActor::ActivatePowerup(ASCharacter* PlayerPawn)
{
	OnActivated(PlayerPawn);

	bIsPowerupActive = true;

	Onrep_PowerupActive();

	if (PowerupInterval > 0.0f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, this, &ASPowerupActor::OnTickPowerup, PowerupInterval, true);
	}
	else
	{
		OnTickPowerup();
	}
}

void ASPowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPowerupActor, bIsPowerupActive);
}


