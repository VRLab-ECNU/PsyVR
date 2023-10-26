// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/REST.h"

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

UREST* UREST::Request(const FString Url, const ERestMethod Method, const TArray<FRequestHeaderItem> Headers, FString Content, bool isJson)
{
	UREST *RestRequest = NewObject<UREST>();
	RestRequest->Url = Url;
	RestRequest->Method = Method;
	RestRequest->Headers = Headers;
	RestRequest->Content = Content;
	RestRequest->isJson = isJson;
	if(isJson)
		RestRequest->Headers.Add(FRequestHeaderItem("Content-Type", "application/json"));
	return RestRequest;
}

void UREST::Activate()
{
	Super::Activate();
	Request();
}

void UREST::Request()
{
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

	switch (Method)
	{
	case ERestMethod::Get:
		Request->SetVerb("GET");
		break;
	case ERestMethod::Post:
		Request->SetVerb("POST");
		break;
	}
	Request->SetURL(Url);
	for (FRequestHeaderItem Header : Headers)
	{
		Request->SetHeader(Header.Key, Header.Value);
	}
	TArray<uint8> RequestContent;
	RequestContent.Append((uint8*)TCHAR_TO_ANSI(*Content), Content.Len());
	Request->SetContent(RequestContent);

	Request->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceed)
	{
		if(bSucceed)
		{
			OnSuccess.Broadcast(HttpResponse->GetContentAsString(), true);
		}
		else
		{
			OnFail.Broadcast(HttpResponse->GetContentAsString(), false);
		}
	});
}


