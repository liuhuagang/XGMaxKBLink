// Copyright 2025 Xiao Gang. All Rights Reserved.

#include "XGMaxKBOpenAsyncAction.h"

#include "Policies/CondensedJsonPrintPolicy.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonReader.h"
#include "JsonObjectConverter.h"

#include "LogXGMaxKBLink.h"

UXGMaxKBOpenAsyncAction::UXGMaxKBOpenAsyncAction(const FObjectInitializer& ObjectInitializer)
{
}

UXGMaxKBOpenAsyncAction::~UXGMaxKBOpenAsyncAction()
{
	RealeaseResources();
}

UXGMaxKBOpenAsyncAction* UXGMaxKBOpenAsyncAction::XGMaxKBOpenAsyncAcion(UObject* WorldContextObject,
	const FString& InMaxKBURL, const FString& InAPIKey)
{

	UXGMaxKBOpenAsyncAction* AsyncAction = NewObject<UXGMaxKBOpenAsyncAction>();

	AsyncAction->MaxKBURL = InMaxKBURL;
	AsyncAction->APIKey = InAPIKey;

	AsyncAction->RegisterWithGameInstance(WorldContextObject);

	return AsyncAction;
}

void UXGMaxKBOpenAsyncAction::Activate()
{
	Super::Activate();

	Then.Broadcast(AsyncID, false,
		TEXT("XGMaxKBOpenAsyncAcion is just started,please wait to be finished!"),
		FXGMaxKBOpenRespInfo());

}

void UXGMaxKBOpenAsyncAction::Activate_Internal()
{
	UE_LOG(LogXGMaxKBLink, Display, TEXT("[%s],AsyncID:[%s],Begin to execute XGMaxKBOpen AsyncAction "),
		*FString(__FUNCTION__), *(AsyncID.ToString()));

	if (MaxKBURL.IsEmpty())
	{
		CallOnFail(AsyncID, false, TEXT("MaxKBURL is Null!!! please input URL"), FXGMaxKBOpenRespInfo());

		RealeaseResources();

		return;
	}

	if (APIKey.IsEmpty())
	{
		CallOnFail(AsyncID, false, TEXT("APIKey is Null!!! please input APIKey"), FXGMaxKBOpenRespInfo());

		RealeaseResources();

		return;
	}

	UE_LOG(LogXGMaxKBLink, Verbose, TEXT("[%s],[%s]"), *FString(__FUNCTION__), TEXT("Begin to request MaxKB Session ID "));

	TMap<FString, FString> Headers;

	Headers.Add(TEXT("Content-Type"), TEXT("application/json"));
	Headers.Add(TEXT("AUTHORIZATION"), APIKey);

	SendHttp(MaxKBURL, Headers);

}

void UXGMaxKBOpenAsyncAction::SendHttp(const FString& InServerURL, TMap<FString, FString> InHeaders)
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UXGMaxKBOpenAsyncAction::OnHttpRespReceived);

	for (auto& Tmp : InHeaders)
	{
		Request->SetHeader(Tmp.Key, Tmp.Value);
	}

	Request->SetURL(InServerURL);

	Request->SetVerb("GET");

	Request->ProcessRequest();

}

void UXGMaxKBOpenAsyncAction::OnHttpRespReceived(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	UE_LOG(LogXGMaxKBLink, Display, TEXT("[%s],AsyncID:[%s],Receive MaxKB HttpResonpse"), *FString(__FUNCTION__), *(AsyncID.ToString()));

	FString ResponseJson = TEXT("");

	if (!HttpResponse.IsValid())
	{
		FXGMaxKBOpenRespInfo HttpRespError;

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

		FXGMaxKBOpenRespInfo RespInfo;
		FJsonObjectConverter::JsonObjectStringToUStruct(ResponseJson, &RespInfo);

		CallOnSuccess(AsyncID, true, RespInfo.data, RespInfo);

	}
	else
	{

		FXGMaxKBOpenRespInfo HttpError;

		UE_LOG(LogXGMaxKBLink, Warning, TEXT("[%s],Http Failed!!!!!,Message:[%s]"), *FString(__FUNCTION__), *ResponseJson);

		CallOnFail(AsyncID, false, TEXT("Http Failed,Message:") + ResponseJson, HttpError);
	}


	RealeaseResources();


}

void UXGMaxKBOpenAsyncAction::CallOnSuccess(FGuid InAsyncID, bool bInResult, FString InMessage, FXGMaxKBOpenRespInfo RespInfo)
{
	FXGMaxKBOpenDelegate TempDelegate = OnSuccess;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDelegate.Broadcast(InAsyncID, bInResult, InMessage, RespInfo);


		});
}

void UXGMaxKBOpenAsyncAction::CallOnFail(FGuid InAsyncID, bool bInResult, FString InMessage, FXGMaxKBOpenRespInfo RespInfo)
{
	FXGMaxKBOpenDelegate TempDelegate = OnFail;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDelegate.Broadcast(InAsyncID, bInResult, InMessage, RespInfo);


		});
}

void UXGMaxKBOpenAsyncAction::RealeaseResources()
{
	Then.Clear();

	OnSuccess.Clear();

	OnFail.Clear();

	MaxKBURL.Empty();

	APIKey.Empty();

	SetReadyToDestroy();
}

