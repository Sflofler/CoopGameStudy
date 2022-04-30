// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVarWeaponDebug(
	TEXT("COOP.DebugWeapons"),
	DebugWeaponDrawing, TEXT("Draw Debug Lines for Weapons"),
	ECVF_Cheat);

// Sets default values
ASWeapon::ASWeapon()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	RootComponent = SkeletalMeshComponent;

	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "Target";
}

void ASWeapon::Fire()
{
	AActor* MyOwner = GetOwner();

	if(MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;

		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + ShotDirection * 10000;

		FCollisionQueryParams QueryParameters;
		QueryParameters.AddIgnoredActor(MyOwner);
		QueryParameters.AddIgnoredActor(this);
		QueryParameters.bTraceComplex = true;

		FVector TracerEndPoint = TraceEnd;

		FHitResult Hit;
		
		if(GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParameters))
		{
			AActor* HitActor = Hit.GetActor();
			
			UGameplayStatics::ApplyPointDamage(HitActor, 20.0f, ShotDirection, Hit, MyOwner->GetInstigatorController(), this, DamageType);

			if(ImpactEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}

			TracerEndPoint = Hit.ImpactPoint;
		}

		if(DebugWeaponDrawing > 0)
		{
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Red, false, 1, 0, 1);
		}

		PlayFireEffects(TracerEndPoint);
	}
}

void ASWeapon::PlayFireEffects(const FVector TracerEndPoint) const
{
	
	if(MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, SkeletalMeshComponent, MuzzleSocketName);
	}

	if(TracerEffect)
	{
		const FVector MuzzleLocation = SkeletalMeshComponent->GetSocketLocation(MuzzleSocketName);
		
		UParticleSystemComponent* TracerComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);

		if(TracerComponent)
		{
			TracerComponent->SetVectorParameter(TracerTargetName, TracerEndPoint);
		}
	}
}

