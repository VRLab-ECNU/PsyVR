#pragma once

#include "CoreMinimal.h"
#include "FileEncoding.h"
#include "FileHelper.generated.h"

UCLASS()
class PSYVR_API UFileHelper : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="PsyVR|FileIO")
	static FString BaseDir();

	UFUNCTION(BlueprintPure, Category="PsyVR|FileIO")
	static FString JoinPathByArray(TArray<FString> Path);

	UFUNCTION(BlueprintCallable, Category="PsyVR|FileIO")
	static bool IsFileExist(FString FileName, FString FilePath);

	UFUNCTION(BlueprintCallable, Category="PsyVR|FileIO")
	static bool IsDirExist(FString DirPath);

	UFUNCTION(BlueprintCallable, Category = "PsyVR|FileIO")
	static bool CreateDir(FString DirPath);

	UFUNCTION(BlueprintCallable, Category = "PsyVR|FileIO")
	static bool CreateFile(FString FileName, FString FilePath);

	UFUNCTION(BlueprintCallable, Category="PsyVR|FileIO")
	static TArray<FString> ListDir(FString DirPath, FString Filter);

	UFUNCTION(BlueprintPure, Category = "PsyVR|FileIO")
	static bool ReadFileToString(FString FileName, FString FilePath, FString &Content);

	UFUNCTION(BlueprintPure, Category = "PsyVR|FileIO")
	static bool ReadFileToStringArray(FString FileName, FString FilePath, TArray<FString> &Content);

	UFUNCTION(BlueprintCallable, Category = "PsyVR|FileIO")
	static bool WriteStringToFile(FString FileName, FString FilePath, FString Content, bool OverWrite, EEncodingOpt Encoding);

	UFUNCTION(BlueprintCallable, Category = "PsyVR|FileIO")
	static bool WriteStringArrayToFile(FString FileName, FString FilePath, TArray<FString> Content, bool OverWrite, EEncodingOpt Encoding);
	
	UFUNCTION(BlueprintCallable, Category="PsyVR|FileIO")
	static bool DeleteFile(FString FileName, FString FilePath);

	UFUNCTION(BlueprintCallable, Category="PsyVR|FileIO")
	static FString GetFileCleanName(FString FullPath);

	UFUNCTION(BlueprintCallable, Category="PsyVR|FileIO")
	static FString GetFileExtension(FString FullPath);
};
