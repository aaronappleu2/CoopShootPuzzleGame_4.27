// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickupActor.h"

#include "SCharacter.h"
#include "SPowerupActor.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ASPickupActor::ASPickupActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(75.0f);
	RootComponent = SphereComp;


	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->SetupAttachment(RootComponent);
	DecalComp->SetRelativeRotation(FRotator(90, 0, 0));
	DecalComp->DecalSize = FVector(64, 75, 75);

	CoolDownDuration = 10.0f;

	SetReplicates(true);


}

// Called when the game starts or when spawned
void ASPickupActor::BeginPlay()
{
	Super::BeginPlay();

	if(GetLocalRole()==ROLE_Authority)
	{
		ReSpawn();
	}

	
}

void ASPickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if(GetLocalRole()==ROLE_Authority)
	{
		ASCharacter* Player = Cast<ASCharacter>(OtherActor);
		if (PowerupInstance && Player)
		{
			PowerupInstance->ActivatePowerup(Player);
			PowerupInstance = nullptr;

			// set timer to respawm
			GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ASPickupActor::ReSpawn, CoolDownDuration);
		}
	}
	

}

void ASPickupActor::ReSpawn()
{
	if(PowerupClass==nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PowerupClass is nullptr in %s. Please update your blueprint"), *GetName());
		return;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	PowerupInstance = GetWorld()->SpawnActor<ASPowerupActor>(PowerupClass, GetTransform(), SpawnParams);
}
