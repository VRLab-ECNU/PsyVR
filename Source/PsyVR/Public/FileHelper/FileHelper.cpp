#include "FileHelper.h"

#include "Platform.h"

#include "../Utils/Logger.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/FileManagerGeneric.h"
#include "HAL/PlatformFileManager.h"
#include "GenericPlatform/GenericPlatformFile.h"

#include <ios>

DEFINE_LOG_CATEGORY(LogPsyVR)


FString UFileHelper::BaseDir()
{
	FString dir = BASE_DIR;
	if(dir=="./")
		return FPaths::ProjectSavedDir();
	return dir;
}

FString ConcatFilePath(FString FilePath, FString FileName)
{
	if (FilePath.EndsWith(SPLIT))
		return FilePath.Append(FileName);
	return FilePath.Append(SPLIT).Append(FileName);
}

FString UFileHelper::JoinPathByArray(TArray<FString> Path)
{
	FString result = "";
	// if path is ends with SPLIT, ignore, else append SPLIT, but ignore the last one
	for (auto& p: Path)
	{
		if (p.EndsWith(SPLIT) || &p - &Path[0] == Path.Num() - 1)
		{
			result.Append(p);
			continue;
		}
		result.Append(p).Append(SPLIT);
	}
	return result;
}

FFileHelper::EEncodingOptions GetEncodingOpt(EEncodingOpt Encoding)
{
	FFileHelper::EEncodingOptions code;
	switch (Encoding)
	{
	case EEncodingOpt::AutoDetect:
		code = FFileHelper::EEncodingOptions::AutoDetect;
		break;
	case EEncodingOpt::ForceAnsi:
		code = FFileHelper::EEncodingOptions::ForceAnsi;
		break;
	case EEncodingOpt::ForceUnicode:
		code = FFileHelper::EEncodingOptions::ForceUnicode;
		break;
	case EEncodingOpt::ForceUTF8:
		code = FFileHelper::EEncodingOptions::ForceUTF8;
		break;
	case EEncodingOpt::ForceUTF8WithoutBOM:
		code = FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM;
		break;
	default:
		code = FFileHelper::EEncodingOptions::AutoDetect;
		break;
	}
	return code;
}


bool UFileHelper::IsFileExist(FString FileName, FString FilePath)
{
	return FFileManagerGeneric::Get().FileExists(*ConcatFilePath(FilePath, FileName));
}

bool UFileHelper::IsDirExist(FString DirPath)
{
	IPlatformFile &PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	return PlatformFile.DirectoryExists(*DirPath);
}

bool UFileHelper::CreateDir(FString DirPath)
{
	IPlatformFile &PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (!PlatformFile.DirectoryExists(*DirPath))
	{
		bool bResult = FFileManagerGeneric::Get().MakeDirectory(*DirPath, true);
		if (bResult)
		{
			UE_LOG(LogPsyVR, Log, TEXT("Create directory %s success."), *DirPath);
			return true;
		}
		UE_LOG(LogPsyVR, Error, TEXT("Create directory %s failed."), *DirPath);
		return false;
	}
	UE_LOG(LogPsyVR, Warning, TEXT("Attempt to create dir: %s while it has already exists."), *DirPath);
	return false;
}

bool UFileHelper::CreateFile(FString FileName, FString FilePath)
{
	if(!IsDirExist(FilePath))
	{
		UE_LOG(LogPsyVR, Log, TEXT("Attempting to create file %s while the directory %s does not exist. Createing dir firstly..."), *FileName, *FilePath);
		CreateDir(FilePath);
	}
	IPlatformFile &PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if(!PlatformFile.FileExists(*ConcatFilePath(FilePath, FileName)))
	{
		bool bResult = FFileHelper::SaveStringToFile(FString(), *ConcatFilePath(FilePath, FileName));
		if(bResult)
		{
			UE_LOG(LogPsyVR, Log, TEXT("Create file %s success."), *ConcatFilePath(FilePath, FileName));
			return true;
		}
		UE_LOG(LogPsyVR, Log, TEXT("Create file %s failed."), *ConcatFilePath(FilePath, FileName));
		return false;
	}
	UE_LOG(LogPsyVR, Warning, TEXT("Attempt to create file: %s while it has already exists."), *ConcatFilePath(FilePath, FileName));
	return false;
}

TArray<FString> UFileHelper::ListDir(FString DirPath, FString Filter)
{
	TArray<FString> Files;
	FFileManagerGeneric::Get().FindFiles(Files, *DirPath, *Filter);
	return Files;
}

bool UFileHelper::ReadFileToString(FString FileName, FString FilePath, FString& Content)
{
	Content = FString();
	if(!IsFileExist(FileName, FilePath))
	{
		UE_LOG(LogPsyVR, Error, TEXT("Read file %s failed. File does not exist."), *ConcatFilePath(FilePath, FileName));
		return false;
	}
	return FFileHelper::LoadFileToString(Content, *ConcatFilePath(FilePath, FileName));
}

bool UFileHelper::ReadFileToStringArray(FString FileName, FString FilePath, TArray<FString>& Content)
{
	if(!IsFileExist(FileName, FilePath))
	{
		UE_LOG(LogPsyVR, Error, TEXT("Read file %s failed. File does not exist."), *ConcatFilePath(FilePath, FileName));
		return false;
	}
	return FFileHelper::LoadFileToStringArray(Content, *ConcatFilePath(FilePath, FileName));
}

bool UFileHelper::WriteStringToFile(FString FileName, FString FilePath, FString Content, bool OverWrite,
	EEncodingOpt Encoding)
{
	if(!IsDirExist(FilePath))
	{
		UE_LOG(LogPsyVR, Log, TEXT("Attempting to write file %s while the directory %s does not exist. Createing dir firstly..."), *FileName, *FilePath);
		CreateDir(FilePath);
	}
	if(!IsFileExist(FileName, FilePath))
	{
		UE_LOG(LogPsyVR, Log, TEXT("Attempting to write file %s while the file does not exist. Createing file firstly..."), *ConcatFilePath(FilePath, FileName));
		CreateFile(FileName, FilePath);
	}
	if(OverWrite)
	{
		return FFileHelper::SaveStringToFile(Content, *ConcatFilePath(FilePath, FileName), GetEncodingOpt(Encoding));
	}
	return FFileHelper::SaveStringToFile(Content, *ConcatFilePath(FilePath, FileName), GetEncodingOpt(Encoding), &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
}

bool UFileHelper::WriteStringArrayToFile(FString FileName, FString FilePath, TArray<FString> Content, bool OverWrite,
	EEncodingOpt Encoding)
{
	if(!IsDirExist(FilePath))
	{
		UE_LOG(LogPsyVR, Log, TEXT("Attempting to write file %s while the directory %s does not exist. Createing dir firstly..."), *FileName, *FilePath);
		CreateDir(FilePath);
	}
	if(!IsFileExist(FileName, FilePath))
	{
		UE_LOG(LogPsyVR, Log, TEXT("Attempting to write file %s while the file does not exist. Createing file firstly..."), *ConcatFilePath(FilePath, FileName));
		CreateFile(FileName, FilePath);
	}
	if(OverWrite)
	{
		return FFileHelper::SaveStringArrayToFile(Content, *ConcatFilePath
			(FilePath, FileName), GetEncodingOpt(Encoding));
	}
	return FFileHelper::SaveStringArrayToFile(Content, *ConcatFilePath(FilePath, FileName), GetEncodingOpt(Encoding), &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
}

bool UFileHelper::DeleteFile(FString FileName, FString FilePath)
{
	if (!IsFileExist(FileName, FilePath))
	{
		UE_LOG(LogPsyVR, Error, TEXT("Delete file %s failed. File does not exist."), *ConcatFilePath(FilePath, FileName));
	}
	bool bResult = FFileManagerGeneric::Get().Delete(*ConcatFilePath(FilePath, FileName));
	if (bResult)
	{
		UE_LOG(LogPsyVR, Log, TEXT("Delete file %s success."), *ConcatFilePath(FilePath, FileName));
		return true;
	}
	UE_LOG(LogPsyVR, Error, TEXT("Delete file %s failed."), *ConcatFilePath(FilePath, FileName));
	return false;
}

FString UFileHelper::GetFileCleanName(FString FullPath)
{
	return FPaths::GetCleanFilename(FullPath);
}

FString UFileHelper::GetFileExtension(FString FullPath)
{
	return FPaths::GetExtension(FullPath);
}
