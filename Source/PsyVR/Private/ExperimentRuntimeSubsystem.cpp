// Fill out your copyright notice in the Description page of Project Settings.
#include "ExperimentRuntimeSubsystem/ExperimentRuntimeSubsystem.h"
#include "JsonObjectConverter.h"

DEFINE_LOG_CATEGORY(LogExpRT)

UExperimentRuntimeSubsystem::UExperimentRuntimeSubsystem()
{
	UE_LOG(LogExpRT, Log, TEXT("Experiment Runtime Subsystem Init."));
	ConditionJsonStringList = {};
	ConditionsJsonObjectString = "";
	NextTrialConditionJsonString = "";
}

FString UExperimentRuntimeSubsystem::GetCurrentConditionJsonString()
{
	TSharedPtr<FJsonObject> JsonObj;
	TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(ConditionsJsonObjectString);
	FJsonSerializer::Deserialize(Reader, JsonObj);
	
	const TArray< TSharedPtr<FJsonValue> >* Trials;
	JsonObj->GetObjectField(FString::FromInt(ExperimentStatus.CurrentSessionIndex))->TryGetArrayField(FString::FromInt(ExperimentStatus.CurrentBlockIndex), Trials);
	FString NewTrialCondition = (*Trials)[ExperimentStatus.CurrentTrialIndex]->AsString();
	return NewTrialCondition;
}


void UExperimentRuntimeSubsystem::ToggleStartExperiment(FString Config)
{
	UE_LOG(LogExpRT, Log, TEXT("Starting Experiment."));
	FString Status = "";
	FJsonObjectConverter::UStructToJsonObjectString(FExperimentStatus::StaticStruct(), &ExperimentStatus, Status, 0, 0);
	UE_LOG(LogExpRT, Log, TEXT("%s"), *Status);

	TSharedPtr<FJsonObject> JsonObj;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ConditionsJsonObjectString);
	if (!FJsonSerializer::Deserialize(JsonReader, JsonObj))
	{
		UE_LOG(LogExpRT, Error, TEXT("Error parsing conditions json string."));
		return;
	}
	
	int32 TotalSession=0; 
	int32 TotalBlock=0;
	
	if ( JsonObj->TryGetNumberField("session_count", TotalSession) && JsonObj->TryGetNumberField("block_count", TotalBlock))
	{
		ExperimentStatus.TotalBlockCount = TotalBlock;
		ExperimentStatus.TotalSessionCount = TotalSession;
		ExperimentStatus.TotalTrialCount = JsonObj->GetObjectField("0")->GetArrayField("0").Num();

		UE_LOG(LogExpRT, Log, TEXT("Got session: %d, block: %d, trial: %d"), TotalSession, TotalBlock, ExperimentStatus.TotalTrialCount);
		
	} else
	{
		UE_LOG(LogExpRT, Error, TEXT("Error parsing conditions json string: No session_count or block_count field."));
		return;
	}
	
	// Call StartExperiment delegate
	if (StartExperiment.IsBound())
	{
		StartExperiment.Broadcast(Config);
		return;
	}
	UE_LOG(LogExpRT, Warning, TEXT("StartExperiment delegate is not bound."));
}

void UExperimentRuntimeSubsystem::ToggleEndExperiment(FString Config)
{
	UE_LOG(LogExpRT, Log, TEXT("Ending Experiment."));

	if (EndExperiment.IsBound())
	{
		EndExperiment.Broadcast();
		return;
	}
	UE_LOG(LogExpRT, Warning, TEXT("EndExperiment delegate is not bound."));
}

void UExperimentRuntimeSubsystem::ToggleBeforeNewTrial()
{
	if(BeforeNewTrial.IsBound())
	{
		BeforeNewTrial.Broadcast(ExperimentStatus, GetCurrentConditionJsonString());
		return;
	}
	UE_LOG(LogExpRT, Warning, TEXT("BeforeNewTrial delegate is not bound."));
}

void UExperimentRuntimeSubsystem::ToggleStartTrial()
{
	if (StartTrial.IsBound())
	{
		StartTrial.Broadcast(ExperimentStatus, GetCurrentConditionJsonString());
		return;
	}
	UE_LOG(LogExpRT, Warning, TEXT("StartTrial delegate is not bound."));
}

void UExperimentRuntimeSubsystem::ToggleBeforeEndTrial()
{
	if (BeforeEndTrial.IsBound())
	{
		BeforeEndTrial.Broadcast(ExperimentStatus, GetCurrentConditionJsonString());
		return;
	}
	UE_LOG(LogExpRT, Warning, TEXT("BeforeEndTrial delegate is not bound."));
}

void UExperimentRuntimeSubsystem::ToggleEndTrial()
{
	if (EndTrial.IsBound())
	{
		EndTrial.Broadcast(ExperimentStatus);
		
	}else
	{
		UE_LOG(LogExpRT, Warning, TEXT("End Trial delegate is not bound."));
	}

	if(ExperimentStatus.CurrentSessionIndex == ExperimentStatus.TotalSessionCount && ExperimentStatus.CurrentBlockIndex == ExperimentStatus.TotalBlockCount && ExperimentStatus.CurrentTrialIndex == ExperimentStatus.TotalTrialCount)
	{
		ExperimentStatus.IsFinished = true;
		if(EndExperiment.IsBound())
		{
			EndExperiment.Broadcast();
		} else
		{
			UE_LOG(LogExpRT, Error, TEXT("EndExperiment Delegate Not Bound!"));
		}
		return;
	}

	if (ExperimentStatus.CurrentTrialIndex == ExperimentStatus.TotalTrialCount)
	{
		ExperimentStatus.CurrentTrialIndex = 0;
		ExperimentStatus.CurrentBlockIndex ++;
		if (ExperimentStatus.CurrentBlockIndex == ExperimentStatus.TotalBlockCount)
		{
			ExperimentStatus.CurrentBlockIndex = 0;
			ExperimentStatus.CurrentSessionIndex ++;
		}
	}
	else
	{
		ExperimentStatus.CurrentTrialIndex ++;
	}

	ToggleBeforeNewTrial();
}

FString UExperimentRuntimeSubsystem::GetDataFilename(FString Extension)
{
	FString Filename = "";
	Filename.Append(FString::FromInt(SubjectInfo.SubjectID));
	Filename.Append(TEXT("_"));
	Filename.Append(SubjectInfo.Gender?"F":"M");
	Filename.Append(Extension);

	return Filename;
}
