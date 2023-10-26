// Fill out your copyright notice in the Description page of Project Settings.

// ECNU VRLab
// Author: Shuye Chen
// Version: 1.0
// ReleaseDate: 2023.9.29

#pragma once

// Head files of utility

#include "ExperimentStatus.h"
#include "SubjectInfo.h"
#include "../Utils/Logger.h"

// Head files generated by Unreal
#include "CoreMinimal.h"

#include "Subsystems/GameInstanceSubsystem.h"
#include "ExperimentRuntimeSubsystem.generated.h"

// Declare delegates for experiment runtime lifecycle
// Parameters passing is implemented by JSON string, so the official plugin "JSON Blueprint Utilities" is required.

//     Experiment
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartExperimentSignature, FString, Config);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndExperimentSignature);

//     Session
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBeforeNewSessionSignature, FExperimentStatus, ExperimentStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartSessionSignature, FExperimentStatus, ExperimentStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBeforeEndSessionSignature, FExperimentStatus, ExperimentStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEndSessionSignature, FExperimentStatus, ExperimentStatus);

//     Block
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBeforeNextBlockSignature, FExperimentStatus, ExperimentStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartBlockSignature, FExperimentStatus, ExperimentStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBeforeEndBlockSignature, FExperimentStatus, ExperimentStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEndBlockSignature, FExperimentStatus, ExperimentStatus);

//     Trial
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBeforeNewTrialSignature, FExperimentStatus, ExperimentStatus, FString,
											 NextConditionJsonString);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStartTrialSignature, FExperimentStatus, ExperimentStatus, FString,
											 CurrentConditionJsonString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBeforeEndTrialSignature, FExperimentStatus, ExperimentStatus, FString,
											 CurrentConditionJsonString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEndTrialSignature, FExperimentStatus, ExperimentStatus);



UCLASS(Blueprintable)
class PSYVR_API UExperimentRuntimeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	
public:
	UExperimentRuntimeSubsystem();
	
	// Experiment Runtime State
	UPROPERTY(BlueprintReadWrite, Category = "ExperimentRuntimeSubsystem")
	FExperimentStatus ExperimentStatus;

	UPROPERTY(BlueprintReadWrite, Category = "ExperimentRuntimeSubsystem")
	FSubjectInfo SubjectInfo;

	UPROPERTY(BlueprintReadWrite, Category = "ExperimentRuntimeSubsystem")
	TArray<FString> ConditionJsonStringList;

	// Experiment Runtime Lifecycle Delegates
	//     Experiment
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "ExperimentRuntimeSubsystem")
	FStartExperimentSignature StartExperiment;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "ExperimentRuntimeSubsystem")
	FEndExperimentSignature EndExperiment;

	//	 Session
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "ExperimentRuntimeSubsystem")
	FBeforeNewSessionSignature BeforeNewSession;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "ExperimentRuntimeSubsystem")
	FStartSessionSignature StartSession;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "ExperimentRuntimeSubsystem")
	FBeforeEndSessionSignature BeforeEndSession;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "ExperimentRuntimeSubsystem")
	FEndSessionSignature EndSession;

	//	 Block
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "ExperimentRuntimeSubsystem")
	FBeforeNextBlockSignature BeforeNextBlock;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "ExperimentRuntimeSubsystem")
	FStartBlockSignature StartBlock;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "ExperimentRuntimeSubsystem")
	FBeforeEndBlockSignature BeforeEndBlock;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "ExperimentRuntimeSubsystem")
	FEndBlockSignature EndBlock;

	//    Trial
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "ExperimentRuntimeSubsystem")
	FBeforeNewTrialSignature BeforeNewTrial;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "ExperimentRuntimeSubsystem")
	FStartTrialSignature StartTrial;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "ExperimentRuntimeSubsystem")
	FBeforeEndTrialSignature BeforeEndTrial;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "ExperimentRuntimeSubsystem")
	FEndTrialSignature EndTrial;

	// Experiment Runtime Lifecycle Functions
	//     Experiment
	UFUNCTION(BlueprintCallable, Category = "ExperimentRuntimeSubsystem")
	void ToggleStartExperiment(FString Config);

	UFUNCTION(BlueprintCallable, Category = "ExperimentRuntimeSubsystem")
	void ToggleEndExperiment(FString Config);

	//   Trial
	UFUNCTION(BlueprintCallable, Category = "ExperimentRuntimeSubsystem")
	void ToggleBeforeNewTrial();
	
	UFUNCTION(BlueprintCallable, Category = "ExperimentRuntimeSubsystem")
	void ToggleStartTrial();

	UFUNCTION(BlueprintCallable, Category = "ExperimentRuntimeSubsystem")
	void ToggleBeforeEndTrial();

	UFUNCTION(BlueprintCallable, Category = "ExperimentRuntimeSubsystem")
	void ToggleEndTrial();

	UFUNCTION(BlueprintPure, Category = "ExperimentRuntimeSubsystem")
	FString GetDataFilename(FString Extension=".csv");

protected:
	FString GetCurrentConditionJsonString();
	FString ConditionsJsonObjectString;
	FString NextTrialConditionJsonString;
	int32 pastTrialCount = 0;
};