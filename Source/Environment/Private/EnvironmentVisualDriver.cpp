
#include "EnvironmentVisualDriver.h"
#include "EnvironmentWorldSettings.h"
#include "EnvironmentManager.h"
#include "Engine/DirectionalLight.h"


AEnvironmentVisualDriver::AEnvironmentVisualDriver()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AEnvironmentVisualDriver::BeginPlay()
{
	Super::BeginPlay();

	DirectionalLight->SetActorRotation(FRotator(19.0f, 0.0f, 0.0f));


	if (UWorld* world = GetWorld())
	{
		if (AEnvironmentWorldSettings* ws = Cast<AEnvironmentWorldSettings>(world->GetWorldSettings()))
		{
			Latitude = ws->Latitude;
		}
		AEnvironmentManager* EnvironmentManager = world->GetGameState<AEnvironmentManager>();
		if (EnvironmentManager)
		{
			EnvironmentManager->OnGameTimeChanged.AddDynamic(this, &AEnvironmentVisualDriver::HandleGameTimeChanged);
			EnvironmentManager->OnGameDayChanged.AddDynamic(this, &AEnvironmentVisualDriver::HandleGameDayChanged);
		}
		// TODO: log error
	}
}

void AEnvironmentVisualDriver::Tick(float DeltaTime)
{
	if (!DirectionalLight || bStopSunRot)
	{
		return;
	}

	FRotator CurrentRotation = DirectionalLight->GetActorRotation();
	const FRotator SmoothedRotation = FMath::RInterpTo(CurrentRotation, TargetSunRotation, DeltaTime, 15.0f);
	DirectionalLight->SetActorRotation(SmoothedRotation);
}

void AEnvironmentVisualDriver::HandleGameTimeChanged(float CurrentTimeOfDayNormalized)
{
	if (CurrentTimeOfDayNormalized >= (DailySunriseTime - 1.0f) && CurrentTimeOfDayNormalized <= (DailySunsetTime + 1.0f))
	{
		float normalizedTime = (CurrentTimeOfDayNormalized - DailySunriseTime) / (DailySunsetTime - DailySunriseTime);
		float sunElevation = MaxPeakAngle * (4.0f * normalizedTime * (1.0f - normalizedTime));
		float sunAzimuth = FMath::Lerp(DailySunriseAzimuth, DailySunsetAzimuth, normalizedTime);
		TargetSunRotation = FRotator(
			-sunElevation,
			sunAzimuth - 180.0f,
			0.0f
		);

		bStopSunRot = false;
	}
	else
	{
		bStopSunRot = true;
	}
}

void AEnvironmentVisualDriver::HandleGameDayChanged(float CurrentDayOfYear)
{
	float dayAngle = FMath::DegreesToRadians((360.0f / 365.0f) * (CurrentDayOfYear - 81));
	float declination = 23.45 * FMath::Sin(dayAngle);

	MaxPeakAngle = 90.0f - Latitude + declination;

	float latitudeRad = FMath::DegreesToRadians(Latitude);
	float declinationRad = FMath::DegreesToRadians(declination);
	float tanAngle = FMath::Tan(latitudeRad) * FMath::Tan(declinationRad);
	tanAngle = FMath::Clamp(-tanAngle, -1.0f, 1.0f);

	MaxHourAngle = FMath::RadiansToDegrees(FMath::Acos(tanAngle));

	DailySunriseTime = (12.0f - (MaxHourAngle / 15.0f));
	DailySunsetTime = (12.0f + (MaxHourAngle / 15.0f));

	float cosAzimuth = FMath::Sin(declinationRad) / FMath::Cos(latitudeRad);
	cosAzimuth = FMath::Clamp(cosAzimuth, -1.0f, 1.0f);

	DailySunriseAzimuth = FMath::RadiansToDegrees(FMath::Acos(cosAzimuth));
	DailySunsetAzimuth = 360.0f - DailySunriseAzimuth;
}

