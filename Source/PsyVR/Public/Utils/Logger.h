#pragma once

#include "Logger.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogExpRT, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogPsyVR, Log, All);


UENUM(BlueprintType)
enum class EExpLogType : uint8
{
	Log,
	Warning,
	Error
};

UCLASS()
class PSYVR_API UPsyLogger : public UObject
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "PsyVR|Logger")
	static void Log(FString Message,const EExpLogType LogType = EExpLogType::Log, bool bPrintToScreen = false, bool bPrintToLog = true, FColor ScreenColor = FColor::White, float Duration = 2.f);

	UFUNCTION(BlueprintPure, Category="PsyVR|Logger")
	static int64 GetCurrentTimeStamp();
};