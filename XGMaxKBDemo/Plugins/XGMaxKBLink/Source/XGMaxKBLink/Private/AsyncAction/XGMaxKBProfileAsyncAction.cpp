// Copyright 2025 Xiao Gang. All Rights Reserved.

#include "XGMaxKBProfileAsyncAction.h"

#include "Policies/CondensedJsonPrintPolicy.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonReader.h"
#include "JsonObjectConverter.h"

#include "LogXGMaxKBLink.h"

UXGMaxKBProfileAsyncAction::UXGMaxKBProfileAsyncAction(const FObjectInitializer& ObjectInitializer)
{
}

UXGMaxKBProfileAsyncAction::~UXGMaxKBProfileAsyncAction()
{
	RealeaseResources();
}

UXGMaxKBProfileAsyncAction* UXGMaxKBProfileAsyncAction::XGMaxKBProfileAsyncAcion(UObject* WorldContextObject, 
const FString& InMaxKBURL, const FString& InAPIKey)
{

	UXGMaxKBProfileAsyncAction* AsyncAction = NewObject<UXGMaxKBProfileAsyncAction>();

	AsyncAction->MaxKBURL = InMaxKBURL;
	AsyncAction->APIKey = InAPIKey;

	AsyncAction->RegisterWithGameInstance(WorldContextObject);

	return AsyncAction;
}

void UXGMaxKBProfileAsyncAction::Activate()
{
	Super::Activate();

	Then.Broadcast(AsyncID, false,
		TEXT("XGMaxKBProfileAsyncAcion is just started,please wait to be finished!"),
		FXGMaxKBProfileRespInfo());

}

void UXGMaxKBProfileAsyncAction::Activate_Internal()
{
	UE_LOG(LogXGMaxKBLink, Display, TEXT("[%s],AsyncID:[%s],Begin to execute XGMaxKBProfile AsyncAction  "),
		*FString(__FUNCTION__), *(AsyncID.ToString()));

	if (MaxKBURL.IsEmpty())
	{
		CallOnFail(AsyncID, false, TEXT("MaxKBURL is Null!!! please input URL"), FXGMaxKBProfileRespInfo());

		RealeaseResources();

		return;
	}



	if (APIKey.IsEmpty())
	{
		CallOnFail(AsyncID, false, TEXT("APIKey is Null!!! please input APIKey"), FXGMaxKBProfileRespInfo());

		RealeaseResources();

		return;
	}

	UE_LOG(LogXGMaxKBLink, Verbose, TEXT("[%s],[%s]"), *FString(__FUNCTION__), TEXT("Begin to request Max App ID"));

	TMap<FString, FString> Headers;

	Headers.Add(TEXT("Content-Type"), TEXT("application/json"));
	Headers.Add(TEXT("AUTHORIZATION"), APIKey);

	SendHttp(MaxKBURL, Headers);

}

void UXGMaxKBProfileAsyncAction::SendHttp(const FString& InServerURL, TMap<FString, FString> InHeaders)
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UXGMaxKBProfileAsyncAction::OnHttpRespReceived);

	for (auto& Tmp: InHeaders)
	{
		Request->SetHeader(Tmp.Key, Tmp.Value);
	}

	Request->SetURL(InServerURL);

	Request->SetVerb("GET");

	Request->ProcessRequest();

}

void UXGMaxKBProfileAsyncAction::OnHttpRespReceived(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	UE_LOG(LogXGMaxKBLink, Display, TEXT("[%s],AsyncID:[%s],Receive MaxKB HttpResonpse"), *FString(__FUNCTION__), *(AsyncID.ToString()));

	FString ResponseJson = TEXT("");

	if (!HttpResponse.IsValid())
	{
		FXGMaxKBProfileRespInfo HttpRespError;

		ResponseJson = TEXT("HttpResponse is Null!Please check your URL is right?");

		UE_LOG(LogXGMaxKBLink, Warning, TEXT("[%s],Http Failed!!!!!,Message:[%s]"), *FString(__FUNCTION__), *ResponseJson);

		CallOnFail(AsyncID, false, TEXT("Http Failed,Message:") + ResponseJson, HttpRespError);

		RealeaseResources();

		return;
	}

	ResponseJson = HttpResponse->GetContentAsString();

	UE_LOG(LogXGMaxKBLink, Verbose, TEXT("[%s],[%s]"), *FString(__FUNCTION__), *ResponseJson);

	if (bSucceeded && HttpRequest->GetStatus() == EHttpRequestStatus::Succeeded && HttpResponse->GetResponseCode() == 200)
	{

		FXGMaxKBProfileRespInfo RespInfo;
		FJsonObjectConverter::JsonObjectStringToUStruct(ResponseJson, &RespInfo);

		CallOnSuccess(AsyncID, true, RespInfo.data.id, RespInfo);

	}
	else
	{

		FXGMaxKBProfileRespInfo HttpError;

		UE_LOG(LogXGMaxKBLink, Warning, TEXT("[%s],Http Failed!!!!!,Message:[%s]"), *FString(__FUNCTION__), *ResponseJson);

		CallOnFail(AsyncID, false, TEXT("Http Failed,Message:") + ResponseJson, HttpError);
	}


	RealeaseResources();


}

void UXGMaxKBProfileAsyncAction::CallOnSuccess(FGuid InAsyncID, bool bInResult, FString InMessage, FXGMaxKBProfileRespInfo RespInfo)
{
	FXGMaxKBProfileDelegate TempDelegate = OnSuccess;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDelegate.Broadcast(InAsyncID, bInResult, InMessage, RespInfo);


		});
}

void UXGMaxKBProfileAsyncAction::CallOnFail(FGuid InAsyncID, bool bInResult, FString InMessage, FXGMaxKBProfileRespInfo RespInfo)
{
	FXGMaxKBProfileDelegate TempDelegate = OnFail;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDelegate.Broadcast(InAsyncID, bInResult, InMessage, RespInfo);


		});
}

void UXGMaxKBProfileAsyncAction::RealeaseResources()
{
	Then.Clear();

	OnSuccess.Clear();

	OnFail.Clear();

	MaxKBURL.Empty();

	APIKey.Empty();

	SetReadyToDestroy();
}
