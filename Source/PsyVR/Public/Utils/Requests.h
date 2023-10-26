// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Requests.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResponseDelegate, FString, Response);

UCLASS()
class PSYVR_API URequests : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintAssignable)
	FResponseDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FResponseDelegate OnFail;

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true"))
	static URequests* UploadRecordFile(const FString SubID, const FString FilePath, const FString UploadUrl, bool MultiPart = false);

	virtual void Activate() override;
	
	FString UploadUrl;
	FString FilePath;
	FString SubID;
	bool isMultiPart = false;

	void Upload();
	void UploadMultiPart();
};
