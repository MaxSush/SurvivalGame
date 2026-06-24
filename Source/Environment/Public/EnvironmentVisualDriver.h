

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnvironmentVisualDriver.generated.h"

class ADirectionalLight;
class AEnvironmentManager;

UCLASS()
class ENVIRONMENT_API AEnvironmentVisualDriver : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnvironmentVisualDriver();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HandleGameTimeChanged(float CurrentTimeOfDayNormalized);
	UFUNCTION()
	void HandleGameDayChanged(float CurrentDayOfYear);
	
private:
	UPROPERTY(EditAnywhere, Category = "Environment|Visual Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ADirectionalLight> DirectionalLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Environment|Visual Components", meta = (AllowPrivateAccess = "true"))
	float Latitude = 45.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Environment|Visual Components", meta = (AllowPrivateAccess = "true"))
	float MaxHourAngle = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Environment|Visual Components", meta = (AllowPrivateAccess = "true"))
	float MaxPeakAngle = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Environment|Visual Components", meta = (AllowPrivateAccess = "true"))
	float DailySunriseTime = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Environment|Visual Components", meta = (AllowPrivateAccess = "true"))
	float DailySunsetTime = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Environment|Visual Components", meta = (AllowPrivateAccess = "true"))
	float DailySunriseAzimuth = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Environment|Visual Components", meta = (AllowPrivateAccess = "true"))
	float DailySunsetAzimuth = 0.0f;

	FRotator TargetSunRotation;
	bool bStopSunRot = true;

};
