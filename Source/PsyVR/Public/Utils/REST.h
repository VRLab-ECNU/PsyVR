// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "REST.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRestResponseDelegate, FString, Response, bool, Success);

/**
 * 
 */
USTRUCT(BlueprintType)
struct FRequestHeaderItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString Key;

	UPROPERTY(BlueprintReadWrite)
	FString Value;

	FRequestHeaderItem(FString Key, FString Value)
	{
		this->Key = Key;
		this->Value = Value;
	}

	FRequestHeaderItem()
	{
		Key= "";
		Value = "";
	}
};

UENUM(BlueprintType)
enum ERestMethod : uint8
{
	Get,
	Post
};

UCLASS()
class PSYVR_API UREST : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	FString Url;
	ERestMethod Method;
	TArray<FRequestHeaderItem> Headers={};
	FString Content;
	bool isJson = true;

	FRestResponseDelegate OnSuccess;
	FRestResponseDelegate OnFail;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UREST* Request(const FString Url, const ERestMethod Method,TArray<FRequestHeaderItem> Headers, FString Content, bool isJson=true);

	void Activate() override;

	void Request();
	
};
