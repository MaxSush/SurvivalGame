


#include "EnvironmentManager.h"
#include "EnvironmentWorldSettings.h"


AEnvironmentManager::AEnvironmentManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

FString AEnvironmentManager::GetCurrentTimeAsString() const
{
	FString ret = FString::Printf(
		TEXT("%04d-%02d-%02d %02d:%02d"), 
		CurrentGameTime.Year,
		CurrentGameTime.Month,
		CurrentGameTime.Day,
		CurrentGameTime.Hour, 
		CurrentGameTime.Minute
	);
	return ret;
}

int32 AEnvironmentManager::GetDayOfYear() const
{
	return CurrentGameTime.CurrentDayInYear;
}

float AEnvironmentManager::GetTemperatureAtLocation(FVector TargetLocation) const
{
	return CalculateAltitudeTemperature(TargetLocation);
}

void AEnvironmentManager::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* world = GetWorld())
	{
		if (AEnvironmentWorldSettings* ws = Cast<AEnvironmentWorldSettings>(world->GetWorldSettings()))
		{
			InitialBaseBiomeTemperature = ws->BaseBiomeTemperature;
			InitialBaseBiomeHumidity = ws->BaseBiomeHumidity;
			RealMinutesPerGameDay = ws->RealMinutesPerGameDay;
		}
		// TODO: log error
	}

	UpdatePerMinute();
	UpdatePerHour();
	UpdatePerDay();
	UpdatePerMonth();
	UpdatePerYear();
}

void AEnvironmentManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateGameTime(DeltaTime);
}

void AEnvironmentManager::UpdateGameTime(float DeltaTime)
{
	if (RealMinutesPerGameDay <= 0.f)
	{
		return;
	}
	float GameMinutesPerRealSecond = (24.f * 60.0f) / (RealMinutesPerGameDay * 60.f);

	MinutesAccumulator += (GameMinutesPerRealSecond * DeltaTime);
	if (MinutesAccumulator > 1.f)
	{
		int MinutesToAdd = FMath::FloorToInt(MinutesAccumulator);
		MinutesAccumulator -= MinutesToAdd;
		AdvanceMinute(MinutesToAdd);
	}

	CurrentTimeOfDayNormalized = CurrentGameTime.Hour + ((CurrentGameTime.Minute + MinutesAccumulator) / 60.f);
}

void AEnvironmentManager::AdvanceMinute(int32 MinutesToAdd)
{
	CurrentGameTime.Minute += MinutesToAdd;
	if (CurrentGameTime.Minute >= 60)
	{
		int HoursToAdd = CurrentGameTime.Minute / 60;
		CurrentGameTime.Minute %= 60;
		AdvanceHour(HoursToAdd);
	}

	UpdatePerMinute();
}

void AEnvironmentManager::AdvanceHour(int32 HoursToAdd)
{
	CurrentGameTime.Hour += HoursToAdd;
	if (CurrentGameTime.Hour >= 24)
	{
		int DaysToAdd = CurrentGameTime.Hour / 24;
		CurrentGameTime.Hour %= 24;
		AdvanceDay(DaysToAdd);
	}
	UpdatePerHour();
}

void AEnvironmentManager::AdvanceDay(int32 DaysToAdd)
{
	CurrentGameTime.Day += DaysToAdd;
	CurrentGameTime.CurrentDayInYear += DaysToAdd;
	int DaysInCurrentMonth = GetDaysInMonth(CurrentGameTime.Year, CurrentGameTime.Month);
	
	while (CurrentGameTime.Day > DaysInCurrentMonth)
	{
		CurrentGameTime.Day -= DaysInCurrentMonth;
		AdvanceMonth(1);
		DaysInCurrentMonth = GetDaysInMonth(CurrentGameTime.Year, CurrentGameTime.Month);
	}
	UpdatePerDay();
}

void AEnvironmentManager::AdvanceMonth(int32 MonthsToAdd)
{
	CurrentGameTime.Month += MonthsToAdd;
	while (CurrentGameTime.Month > 12)
	{
		CurrentGameTime.Month -= 12;
		AdvanceYear(1);
	}
	UpdatePerMonth();
}

void AEnvironmentManager::AdvanceYear(int32 YearsToAdd)
{
	CurrentGameTime.Year += YearsToAdd;
	UpdatePerYear();
}

int32 AEnvironmentManager::GetDaysInMonth(int32 Year, int32 Month) const
{
	switch (Month)
	{
	case 2 :
		return 28;
	case 1: 
	case 3: 
	case 5: 
	case 7: 
	case 8: 
	case 10: 
	case 12:
		return 31;
	default:
		return 30;
	}
}

void AEnvironmentManager::UpdateGameClimate()
{
	int laggedHour = (CurrentTimeOfDayNormalized - 13) * 15;
	float radiationFactor = FMath::Cos(FMath::DegreesToRadians(laggedHour));

	CurrentTemperature = BaseBiomeTemperature + (8 * radiationFactor) - CalculateAltitudeTemperature();

	float humidity = 20.0f * radiationFactor;
	CurrentHumidity = FMath::Clamp(BaseBiomeHumidity - humidity, 0.0f, 100.0f);
}

void AEnvironmentManager::UpdatePerMinute()
{
	UpdateGameClimate();
	OnTemperatureChanged.Broadcast(CurrentTemperature);
	OnGameTimeChanged.Broadcast(CurrentTimeOfDayNormalized);
}

void AEnvironmentManager::UpdatePerHour()
{
}

void AEnvironmentManager::UpdatePerDay()
{
	UpdateBaseValues();
	OnGameDayChanged.Broadcast(CurrentGameTime.CurrentDayInYear);
}

void AEnvironmentManager::UpdatePerMonth()
{
}

void AEnvironmentManager::UpdatePerYear()
{
}

void AEnvironmentManager::UpdateBaseValues()
{
	float constant = (2.0f * PI / 365.0f) * (CurrentGameTime.CurrentDayInYear - 105);
	constant = FMath::Sin(constant);
	BaseBiomeTemperature = InitialBaseBiomeTemperature + (15 * constant);
	BaseBiomeHumidity = InitialBaseBiomeHumidity - (8 * constant);
}

float AEnvironmentManager::CalculateAltitudeTemperature(FVector TargetLocation) const
{
	// in meters
	float altitude = TargetLocation.Z / 100.0f;
	// Weather cooling modifier
	// CalculatedTemperature -= CurrentCloudCover * 5.0f;
	return (altitude * EnvironmentalLapseRate);
}
