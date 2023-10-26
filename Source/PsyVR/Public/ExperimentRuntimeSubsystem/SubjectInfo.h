#pragma once

#include "SubjectInfo.generated.h"

USTRUCT(BlueprintType)
struct FSubjectInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "ExperimentRuntimeSubsystem")
	int32 SubjectID;

	UPROPERTY(BlueprintReadWrite, Category = "ExperimentRuntimeSubsystem")
	bool Gender;

	UPROPERTY(BlueprintReadWrite, Category = "ExperimentRuntimeSubsystem")
	FString ConfigJsonString;

	FSubjectInfo()
	{
		SubjectID = 0;
		Gender = true;
		ConfigJsonString = "{}";
	}
};

