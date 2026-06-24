

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "EnvironmentManager.generated.h"

/**
 *  TODO: Update climate logic to include humidity, wind, and precipitation effects on temperature.
 *	
 *		tune temperature curve with sun timeline curve
 */


USTRUCT(BlueprintType)
struct FGameTimeData
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment|Calendar")
	int32 Year = 2026;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment|Calendar")
	int32 Month = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment|Calendar")
	int32 Day = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment|Calendar")
	int32 Hour = 6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment|Calendar")
	int32 Minute = 0;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment|Calendar")
	int32 CurrentDayInYear = 1;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameTimeChanged, float, CurrentTimeOfDayNormalized);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameDayChanged, float, CurrentDayOfYear);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTemperatureChanged, float, CurrentTemperature);

UCLASS()
class ENVIRONMENT_API AEnvironmentManager : public AGameStateBase
{
	GENERATED_BODY()

public:
	AEnvironmentManager();

	UFUNCTION(BlueprintPure, Category = "Environment|Time")
	FString GetCurrentTimeAsString() const;

	UFUNCTION(BlueprintPure, Category = "Environment|Time")
	FORCEINLINE FGameTimeData GetCurrentGameTime() const { return CurrentGameTime; }

	UFUNCTION(BlueprintPure, Category = "Environment|Time")
	int32 GetDayOfYear() const;

	UFUNCTION(BlueprintPure, Category = "Environment|Time")
	FORCEINLINE float GetCurrentTimeOfDayNormalized() const { return CurrentTimeOfDayNormalized; }

	UFUNCTION(BlueprintPure, Category = "Environment|Climate")
	float GetTemperatureAtLocation(FVector TargetLocation) const;

	UFUNCTION(BlueprintPure, Category = "Environment|Climate")
	float GetCurrentTemperature() const { return CurrentTemperature; }

	UFUNCTION(BlueprintPure, Category = "Environment|Climate")
	float GetCurrentHumidity() const { return CurrentHumidity; }

	UPROPERTY(BlueprintAssignable, Category = "Environment|Events")
	FOnGameTimeChanged OnGameTimeChanged;

	UPROPERTY(BlueprintAssignable, Category = "Environment|Events")
	FOnGameDayChanged OnGameDayChanged;

	UPROPERTY(BlueprintAssignable, Category = "Environment|Events")
	FOnTemperatureChanged OnTemperatureChanged;


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	void UpdateGameTime(float DeltaTime);
	void UpdateGameClimate();
	void UpdateBaseValues();


	void UpdatePerMinute();
	void UpdatePerHour();
	void UpdatePerDay();
	void UpdatePerMonth();
	void UpdatePerYear();


#pragma region TimeManagement

	UPROPERTY(BlueprintReadOnly, Category = "Environment|Settings", meta = (AllowPrivateAccess = "true"))
	FGameTimeData CurrentGameTime;

	UPROPERTY(BlueprintReadOnly, Category = "Environment|Settings", meta = (AllowPrivateAccess = "true"))
	float CurrentTimeOfDayNormalized = 6.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Environment|Settings", meta = (AllowPrivateAccess = "true"))
	float RealMinutesPerGameDay = 20.f;

	float MinutesAccumulator = 0.0f;


	void AdvanceMinute(int32 MinutesToAdd);
	void AdvanceHour(int32 HoursToAdd);
	void AdvanceDay(int32 DaysToAdd);
	void AdvanceMonth(int32 MonthsToAdd);
	void AdvanceYear(int32 YearsToAdd);

	int32 GetDaysInMonth(int32 Year, int32 Month) const;

#pragma endregion

#pragma region ClimateManagement

	float InitialBaseBiomeTemperature = 25.0f;
	float InitialBaseBiomeHumidity = 71.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Environment|Climate", meta = (AllowPrivateAccess = "true"))
	float BaseBiomeTemperature = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Environment|Climate", meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "1.0"))
	float BaseBiomeHumidity = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Environment|Climate", meta = (AllowPrivateAccess = "true"))
	float EnvironmentalLapseRate = 0.0065f;

	UPROPERTY(BlueprintReadOnly, Category = "Environment|Climate", meta = (AllowPrivateAccess = "true"))
	float CurrentTemperature;

	UPROPERTY(BlueprintReadOnly, Category = "Environment|Climate", meta = (AllowPrivateAccess = "true"))
	float CurrentHumidity;  // 0 to 100


	float CalculateAltitudeTemperature(FVector TargetLocation = FVector::ZeroVector) const;

#pragma endregion

};
