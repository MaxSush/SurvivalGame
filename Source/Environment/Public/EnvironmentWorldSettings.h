

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "EnvironmentWorldSettings.generated.h"

UCLASS()
class ENVIRONMENT_API AEnvironmentWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
	
public:
	// Altitude thinking......

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Environment|Climate Data")
	float Latitude = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Environment|Climate Data")
	float BaseBiomeTemperature = 9.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Environment|Climate Data")
	float BaseBiomeHumidity = 71.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Environment|Climate Data")
	float RealMinutesPerGameDay = 3.0f;

};
