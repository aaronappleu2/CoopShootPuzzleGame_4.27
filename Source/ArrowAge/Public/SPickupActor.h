// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPickupActor.generated.h"

class USphereComponent;
class ASPowerupActor;

UCLASS()
class ARROWAGE_API ASPickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPickupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UDecalComponent* DecalComp;

	UPROPERTY(EditInstanceOnly, Category = "PickupActor")
	TSubclassOf<ASPowerupActor> PowerupClass;

	ASPowerupActor* PowerupInstance;

	UPROPERTY(EditDefaultsOnly, Category="PickupActor")
	float CoolDownDuration;

	FTimerHandle TimerHandle_RespawnTimer;

	UFUNCTION()
	void ReSpawn();

public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};

