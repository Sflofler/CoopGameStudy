// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"

class USHealthComponent;
class USphereComponent;
class UStaticMeshComponent;
class UMaterialInstanceDynamic;
class UParticleSystem;
class USoundCue;

UCLASS()
class COOPGAME_API ASTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	
	// Sets default values for this pawn's properties
	ASTrackerBot();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
	USHealthComponent* HealthComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
	USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
	USoundCue* SelfDestructSound;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
	USoundCue* ExplosionSound;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float MovementForce;
	
	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float RequiredDistanceToTarget;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float ExplosionRadius;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float ExplosionDamage;
	
	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	bool bUseVelocityChange;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float SelfDamageInterval;

	bool bExploded;

	bool bStartedSelfDestruction;

	UMaterialInstanceDynamic* MaterialInstanceDynamic;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	UParticleSystem* ExplosionEffect;

	FVector GetNextPathPoint();

	FVector NextPathPoint;

	FTimerHandle TimerHandle_SelfDamage;

	UFUNCTION()
	void HandleHealthChanged(USHealthComponent* UsHealthComponent, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	void SelfDestruct();

	void DamageSelf();
};
