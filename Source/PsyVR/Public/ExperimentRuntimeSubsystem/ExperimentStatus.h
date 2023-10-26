#pragma once

#include "ExperimentStatus.generated.h"


USTRUCT(BlueprintType)
struct FExperimentStatus
{
	GENERATED_USTRUCT_BODY()

	// Exposed to blueprint
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ExperimentRuntimeSubsystem")
	int32 CurrentTrialIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ExperimentRuntimeSubsystem")
	int32 CurrentBlockIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ExperimentRuntimeSubsystem")
	int32 CurrentSessionIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ExperimentRuntimeSubsystem")
	int32 TotalTrialCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ExperimentRuntimeSubsystem")
	int32 TotalSessionCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ExperimentRuntimeSubsystem")
	int32 TotalBlockCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ExperimentRuntimeSubsystem")
	bool UseBlock;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ExperimentRuntimeSubsystem")
	bool UseSession;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ExperimentRuntimeSubsystem")
	bool IsFinished;

	
	// Constructor
	
	FExperimentStatus()
	{
		CurrentSessionIndex = 0;
		CurrentBlockIndex = 0;
		CurrentTrialIndex = 0;
		TotalSessionCount = 0;
		TotalBlockCount = 0;
		TotalTrialCount = 0;
		UseBlock = false;
		UseSession = false;
		IsFinished = false;
	}
};