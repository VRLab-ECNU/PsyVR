#pragma once

#include "Logger.h"

//DEFINE_LOG_CATEGORY(LogPsyVR)

void UPsyLogger::Log(FString Message, const EExpLogType Type, bool bPrintToScreen, bool bPrintToLog, FColor ScreenColor,
	float Duration)
{
	FColor msgColor;
	switch (Type)
	{
	case EExpLogType::Log:
		msgColor = FColor::Cyan;
		break;
	case EExpLogType::Warning:
		msgColor = FColor::Yellow;
		break;
	case EExpLogType::Error:
		msgColor = FColor::Red;
		break;
	default:
		break;
	}
	if(bPrintToLog)
	{
		switch (Type)
		{
		case EExpLogType::Log:
			UE_LOG(LogPsyVR, Log, TEXT("%s"), *Message);
			break;
		case EExpLogType::Warning:
			UE_LOG(LogPsyVR, Warning, TEXT("%s"), *Message);
			break;
		case EExpLogType::Error:
			UE_LOG(LogPsyVR, Error, TEXT("%s"), *Message);
			break;
		default:
			UE_LOG(LogPsyVR, Log, TEXT("%s"), *Message);
			break;
		}
	}
	if(bPrintToScreen)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, msgColor, Message);
	}
}

int64 UPsyLogger::GetCurrentTimeStamp()
{
	FDateTime Now = FDateTime::Now();
	return Now.ToUnixTimestamp();
}
