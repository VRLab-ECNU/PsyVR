// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PsyBPFL.generated.h"

/**
 * 
 */
UCLASS()
class PSYVR_API UPsyBPFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category="PsyVR|Utils")
	static FString ConcatCsvRow(TArray<FString> Row, FString Delimiter=",", bool WithNewline=true);
	
};
