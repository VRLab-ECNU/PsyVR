// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/Requests.h"

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"


void URequests::Activate()
{
	Super::Activate();
	if(isMultiPart)
		UploadMultiPart();
	else
		Upload();
}

URequests* URequests::UploadRecordFile(const FString SubID, const FString FilePath, const FString UploadUrl, bool MultiPart)
{
	URequests *Request = NewObject<URequests>();
	Request->SubID = SubID;
	Request->UploadUrl = UploadUrl;
	Request->FilePath = FilePath;
	Request->isMultiPart = MultiPart;
	return Request;
}

void URequests::Upload()
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

	Request->SetURL(UploadUrl);
	Request->SetVerb("PUT");
	Request->SetHeader(TEXT("Content-Type"), TEXT("multipart/form-data"));
	Request->SetHeader(TEXT("SubID"), SubID);
	Request->SetContentAsStreamedFile(FilePath);
	Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
	{
		if (bSucceeded)
		{
			OnSuccess.Broadcast(HttpResponse->GetContentAsString());
		}
		else
		{
			OnFail.Broadcast(HttpResponse->GetContentAsString());
		}
	});
	Request->ProcessRequest();
}

void URequests::UploadMultiPart()
{
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(UploadUrl);

	// Set Header For multipart/formdata
	FString Boundary = "---------------------------" + FString::FromInt(FDateTime::Now().GetTicks());
	HttpRequest->SetHeader(TEXT("SubID"), SubID);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("multipart/form-data; boundary=") + Boundary);

	// Get File Data
	TArray<uint8> FileContent;
	bool bSuccess = FFileHelper::LoadFileToArray(FileContent, *FilePath);
	if(!bSuccess){
		OnFail.Broadcast(FString("Faild to load file"));
		return;
	}

	TArray<uint8> RequestContent;
	
	// Set Body Boundary
	FString BeginBoundary = "\r\n--" + Boundary + "\r\n";
	RequestContent.Append((uint8*)TCHAR_TO_ANSI(*BeginBoundary), BeginBoundary.Len());

	// Set File Content Header
	FString FileHeader = "Content-Disposition: form-data; name=\"file\"; filename=\"" + FPaths::GetCleanFilename(FilePath) + "\"\r\n";
	FString FileExtension = FPaths::GetExtension(FilePath);
	FString Type = "Content-Type: ";

	TArray<FString> SupportedFileTypes = {"csv", "json", "xlsx", "txt"};
	if(!SupportedFileTypes.Contains(FileExtension)){
		OnFail.Broadcast(FString("File type not supported"));
		return;
	}
	int32 Index = SupportedFileTypes.Find(FileExtension);
	switch (Index)
	{
		case 0:
			Type.Append("text/csv");
			break;
		case 1:
			Type.Append("text/json");
			break;
		case 2:
			Type.Append("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
			break;
		case 3:
			Type.Append("text/plain");
			break;
	}
	Type.Append("\r\n\r\n");
	FileHeader.Append(Type);
	
	RequestContent.Append((uint8*)TCHAR_TO_ANSI(*FileHeader), FileHeader.Len());

	RequestContent.Append(FileContent);

	FString EndBoundary = "\r\n--" + Boundary + "--\r\n";
	RequestContent.Append((uint8*)TCHAR_TO_ANSI(*EndBoundary), EndBoundary.Len());

	HttpRequest->SetContent(RequestContent);

	HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
	{
		if (bSucceeded)
		{
			OnSuccess.Broadcast(HttpResponse->GetContentAsString());
		}
		else
		{
			OnFail.Broadcast(HttpResponse->GetContentAsString());
		}
	});
	
	HttpRequest->ProcessRequest();
	
}

