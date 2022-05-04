// Fill out your copyright notice in the Description page of Project Settings.


#include "AExplosiveBarrel.h"

#include "Components/SHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AAExplosiveBarrel::AAExplosiveBarrel()
{
	HealthComponent = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnHealthChanged.AddDynamic(this, &AAExplosiveBarrel::HandleHealthChanged);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionObjectType(ECC_PhysicsBody);
	RootComponent = StaticMeshComponent;

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComponent"));
	RadialForceComponent->SetupAttachment(StaticMeshComponent);
	RadialForceComponent->Radius = 250;
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->bAutoActivate = false;
	RadialForceComponent->bIgnoreOwningActor = true;

	ExplosionImpulse = 400.0f;
}

// Called when the game starts or when spawned
void AAExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
}

void AAExplosiveBarrel::HandleHealthChanged(USHealthComponent* OwningHealthComponent, float Health, float HealthDelta,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if(bExploded)
	{
		return;
	}

	if(Health <= 0.0f)
	{
		bExploded = true;

		const FVector BoostIntensity = FVector::UpVector * ExplosionImpulse;
		StaticMeshComponent->AddImpulse(BoostIntensity, NAME_None, true);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

		StaticMeshComponent->SetMaterial(0, ExplodedMaterial);

		RadialForceComponent->FireImpulse();

		const TArray<AActor*> IgnoredActors;

		UGameplayStatics::ApplyRadialDamage(this, 50.0f, GetActorLocation(), 250.0f, DamageTypeClass, IgnoredActors);
	}
}

