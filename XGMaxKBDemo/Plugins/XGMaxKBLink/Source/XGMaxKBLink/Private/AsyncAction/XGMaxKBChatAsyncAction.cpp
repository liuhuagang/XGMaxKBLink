// Copyright 2025 Xiao Gang. All Rights Reserved.

#include "XGMaxKBChatAsyncAction.h"

#include "Policies/CondensedJsonPrintPolicy.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonReader.h"
#include "JsonObjectConverter.h"

#include "Containers/StringConv.h"

#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"
#include "Misc/Base64.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"

#include "LogXGMaxKBLink.h"


UXGMaxKBChatAsyncAction::UXGMaxKBChatAsyncAction(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{


}

UXGMaxKBChatAsyncAction::~UXGMaxKBChatAsyncAction()
{
	RealeaseResources();

}

UXGMaxKBChatAsyncAction* UXGMaxKBChatAsyncAction::XGMaxKBChatAsyncAction(UObject* WorldContextObject,
	const FString& InMaxKBURL,
	const FString& InAPIKey ,
	FXGMaxKBChatReqInfo InReqInfo)
{
	UXGMaxKBChatAsyncAction* AsyncAction = NewObject<UXGMaxKBChatAsyncAction>();

	AsyncAction->MaxKBURL = InMaxKBURL;
	AsyncAction->APIKey = InAPIKey;
	AsyncAction->ReqInfo = InReqInfo;
	AsyncAction->RegisterWithGameInstance(WorldContextObject);

	return AsyncAction;
}

void UXGMaxKBChatAsyncAction::Activate()
{
	Super::Activate();

	Then.Broadcast(AsyncID, false, 
	TEXT("XGMaxKBChatAsyncAction is just started,please wait to be finished!"), 
		FXGMaxKBChatRespInfo());

}

void UXGMaxKBChatAsyncAction::Activate_Internal()
{
	UE_LOG(LogXGMaxKBLink,
	Display, 
	TEXT("[%s],AsyncID:[%s],Begin to execute XGVolcanoChatCompletion AsyncAction"),
	*FString(__FUNCTION__), *(AsyncID.ToString()));

	if (MaxKBURL.IsEmpty())
	{
		CallOnFail(AsyncID, false, TEXT("MaxKBURL is Null!!! please input URL"), FXGMaxKBChatRespInfo());

		RealeaseResources();

		return;
	}

	if (APIKey.IsEmpty())
	{
		CallOnFail(AsyncID, false, TEXT("APIKey is Null!!! please input APIKey"), FXGMaxKBChatRespInfo());

		RealeaseResources();

		return;
	}

	if (ReqInfo.message.IsEmpty())
	{
		CallOnFail(AsyncID, false, TEXT("Message is Null!!! please input message"), FXGMaxKBChatRespInfo());

		RealeaseResources();

		return;
	}



	UE_LOG(LogXGMaxKBLink, Verbose, TEXT("[%s],[%s]"), *FString(__FUNCTION__), TEXT("Begin to request MaxKB Chat"));

	TMap<FString, FString> Headers;

	Headers.Add(TEXT("accept"), TEXT("application/json"));
	Headers.Add(TEXT("AUTHORIZATION"), APIKey);
	Headers.Add(TEXT("Content-Type"), TEXT("application/json"));

	FString ConentString = TEXT("");

	FJsonObjectConverter::UStructToJsonObjectString(ReqInfo, ConentString);

	UE_LOG(LogXGMaxKBLink, Verbose, TEXT("[%s],[%s]"), *FString(__FUNCTION__), *ConentString);

	SendHttp(MaxKBURL, ConentString, Headers);


}

void UXGMaxKBChatAsyncAction::SendHttp(const FString& InServerURL, const FString& InContentString, TMap<FString, FString> InHeaders)
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UXGMaxKBChatAsyncAction::OnHttpRespReceived);


	FHttpRequestStreamDelegate Delegate;
	Delegate.BindUObject(this, &UXGMaxKBChatAsyncAction::HttpSream);

	if (ReqInfo.stream)
	{
		Request->SetResponseBodyReceiveStreamDelegate(Delegate);
	}

	Request->SetURL(InServerURL);

	for (auto& Tmp : InHeaders)
	{
		Request->SetHeader(Tmp.Key, Tmp.Value);
	}

	Request->SetVerb("Post");

	Request->SetContentAsString(InContentString);

	Request->ProcessRequest();

}

void UXGMaxKBChatAsyncAction::OnHttpRespReceived(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	UE_LOG(LogXGMaxKBLink, Display, TEXT("[%s],AsyncID:[%s],Receive VolcanoChat Chat HttpResonpse"), *FString(__FUNCTION__), *(AsyncID.ToString()));

	FString ResponseJson = TEXT("");

	if (!HttpResponse.IsValid())
	{
		ResponseJson = TEXT("HttpResponse is Null!Please check your URL is right?");

		FXGMaxKBChatRespInfo HttpError;
		HttpError.bResult = false;
		HttpError.bReqStream = ReqInfo.stream;
		HttpError.MessageContent = ResponseJson;

		UE_LOG(LogXGMaxKBLink, Warning, TEXT("[%s],Http Failed!!!!!,Message:[%s]"), *FString(__FUNCTION__), *ResponseJson);

		CallOnFail(AsyncID, false, TEXT("Http Failed,Message:") + ResponseJson, HttpError);

		RealeaseResources();

		return;
	}

	ResponseJson = HttpResponse->GetContentAsString();

	UE_LOG(LogXGMaxKBLink, Verbose, TEXT("[%s],[%s]"), *FString(__FUNCTION__), *ResponseJson);

	if (bSucceeded && HttpRequest->GetStatus() == EHttpRequestStatus::Succeeded && HttpResponse->GetResponseCode() == 200)
	{

		if (ReqInfo.stream)
		{
			//不在此处处理流式的响应数据

		}
		else
		{

			FXGMaxKBChatNoStreamRespMessage NoStreamRespMessage;
			FJsonObjectConverter::JsonObjectStringToUStruct(ResponseJson,&NoStreamRespMessage);
	

			FXGMaxKBChatRespInfo RespInfo;
			
			RespInfo.bResult=true;
			RespInfo.bReqStream=false;
			RespInfo.MessageContent = NoStreamRespMessage.data.content;
			RespInfo.NoStreamRespMessage = NoStreamRespMessage;

			CallOnSuccess(AsyncID, true, RespInfo.MessageContent, RespInfo);
		}
	}
	else
	{


		if (ReqInfo.stream)
		{
			FUTF8ToTCHAR ErrorJsonString((const ANSICHAR*)ErrorSteamData.GetData(), ErrorSteamData.Num());

			FString ErrorString(ErrorJsonString.Length(), ErrorJsonString.Get());
			
			FXGMaxKBChatErrorMessage ErrorMessage;
			FJsonObjectConverter::JsonObjectStringToUStruct(ErrorString, &ErrorMessage);

			FXGMaxKBChatRespInfo HttpError;

			HttpError.bReqStream = false;
			HttpError.bResult = false;
			HttpError.MessageContent = ErrorMessage.message;
			HttpError.ErrorRespMessage = ErrorMessage;

			UE_LOG(LogXGMaxKBLink, Warning, TEXT("[%s],Http Failed!!!!!,Message:[%s]"), *FString(__FUNCTION__), *ErrorMessage.message);

			CallOnFail(AsyncID, false, TEXT("Http Failed,Message:") + ErrorMessage.message, HttpError);



		}
		else 
		{
			FXGMaxKBChatErrorMessage ErrorMessage;
			FJsonObjectConverter::JsonObjectStringToUStruct(ResponseJson, &ErrorMessage);

			FXGMaxKBChatRespInfo HttpError;

			HttpError.bReqStream=false;
			HttpError.bResult =false;
			HttpError.MessageContent = ErrorMessage.message;
			HttpError.ErrorRespMessage = ErrorMessage;

			UE_LOG(LogXGMaxKBLink, Warning, TEXT("[%s],Http Failed!!!!!,Message:[%s]"), *FString(__FUNCTION__), *ResponseJson);

			CallOnFail(AsyncID, false, TEXT("Http Failed,Message:") + ResponseJson, HttpError);
		}
	}

	RealeaseResources();

}

bool UXGMaxKBChatAsyncAction::HttpSream(void* Ptr, int64 Length)
{

	TArray<uint8> TempStreamData;

	TempStreamData.Append((uint8*)Ptr, Length);

	UXGMaxKBChatAsyncAction* TestPtr = this;

	if (TestPtr)
	{
		AsyncTask(ENamedThreads::GameThread, [TempStreamData, TestPtr]() {

			if (TestPtr)
			{
				TestPtr->HandleStreamData(TempStreamData);
			}

			});
	}

	return true;
}

void UXGMaxKBChatAsyncAction::HandleStreamData(TArray<uint8> InStreamData)
{
	if (IsEngineExitRequested())
	{
		return;
	}


	FUTF8ToTCHAR UTF8String((const ANSICHAR*)InStreamData.GetData(), InStreamData.Num());

	FString OriginIndexedJson(UTF8String.Length(), UTF8String.Get());

	FString IndexedJson(UTF8String.Length(), UTF8String.Get());

	TSharedPtr<FJsonObject> TextJsonObject;
	TSharedRef<TJsonReader<TCHAR>>TextJsonReader = TJsonReaderFactory<TCHAR>::Create(IndexedJson);
	bool IsDeserialize = FJsonSerializer::Deserialize(TextJsonReader, TextJsonObject);



	if (IsDeserialize && TextJsonObject->TryGetField(TEXT("code")))
	{
		int32 Code = TextJsonObject->GetIntegerField(TEXT("code"));
		if (Code!=200)
		{
			ErrorSteamData = InStreamData;
			return;
		}

	}



	StreamData.Append(InStreamData);

	RecursiveProcessingStreamData();


}

void UXGMaxKBChatAsyncAction::RecursiveProcessingStreamData()
{

	int32 RecoredIndex = -1;

	for (size_t Index = 0; Index < StreamData.Num(); Index++)
	{
		if (StreamData[Index] == 10 && Index + 1 < StreamData.Num() && StreamData[Index + 1] == 10)
		{
			RecoredIndex = Index;
			break;
		}
	}

	if (RecoredIndex != -1)
	{
		TArray<uint8> HandleStreamData;
		HandleStreamData.Append(StreamData.GetData(), RecoredIndex);

		StreamData.RemoveAt(0, RecoredIndex + 2);

		FUTF8ToTCHAR HandleString((const ANSICHAR*)HandleStreamData.GetData(), HandleStreamData.Num());

		FString OriginHandleJson(HandleString.Length(), HandleString.Get());
		FString HandleJson(HandleString.Length(), HandleString.Get());

		HandleJson.RemoveFromStart(TEXT("data:"));

		UE_LOG(LogXGMaxKBLink, Verbose, TEXT("[%s], Stream  data Apeend ,JsonStr:[%s]"), *FString(__FUNCTION__), *OriginHandleJson);

		if (HandleJson.Contains(TEXT("\n")))
		{
			UE_LOG(LogXGMaxKBLink, Error, TEXT("[%s],Fatal Error  Stream  data Apeend Wroing!!!,JsonStr:[%s]"), *FString(__FUNCTION__), *OriginHandleJson);
		}


		FXGMaxKBMiddleStreamRespMessage StreamMiddleMessage;

		FJsonObjectConverter::JsonObjectStringToUStruct(HandleJson, &StreamMiddleMessage);


		FXGMaxKBChatRespInfo MiddleRespInfo;
		MiddleRespInfo.bResult = true;
		MiddleRespInfo.bReqStream = true;

		MiddleRespInfo.MessageContent = StreamMiddleMessage.content;

		MiddleRespInfo.StreamMiddleRespMessage = StreamMiddleMessage;
		TotalString += MiddleRespInfo.MessageContent;

		CallOnUpdate(AsyncID, true, MiddleRespInfo.MessageContent, MiddleRespInfo);

		if (StreamMiddleMessage.is_end)
		{

			FXGMaxKBChatRespInfo FinalRespInfo;
			FinalRespInfo.bResult = true;
			FinalRespInfo.bReqStream = true;

			FinalRespInfo.MessageContent = TotalString;

			FinalRespInfo.StreamFinalRespMessage.usage =  StreamMiddleMessage.usage;
	
			CallOnSuccess(AsyncID, true, TotalString, FinalRespInfo);

		}


		RecursiveProcessingStreamData();
	}





}

void UXGMaxKBChatAsyncAction::CallOnSuccess(FGuid InAsyncID, bool bInResult, FString InMessage, FXGMaxKBChatRespInfo RespInfo)
{
	FXGMaxKBChatDelegate TempDelegate = OnSuccess;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDelegate.Broadcast(InAsyncID, bInResult, InMessage, RespInfo);


		});

}

void UXGMaxKBChatAsyncAction::CallOnUpdate(FGuid InAsyncID, bool bInResult, FString InMessage, FXGMaxKBChatRespInfo RespInfo)
{
	FXGMaxKBChatDelegate TempDelegate = OnUpdate;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDelegate.Broadcast(InAsyncID, bInResult, InMessage, RespInfo);


		});

}

void UXGMaxKBChatAsyncAction::CallOnFail(FGuid InAsyncID, bool bInResult, FString InMessage, FXGMaxKBChatRespInfo RespInfo)
{

	FXGMaxKBChatDelegate TempDelegate = OnFail;

	AsyncTask(ENamedThreads::GameThread, [=]() {

		TempDelegate.Broadcast(InAsyncID, bInResult, InMessage, RespInfo);


		});
}

void UXGMaxKBChatAsyncAction::RealeaseResources()
{
	Then.Clear();

	OnSuccess.Clear();

	OnUpdate.Clear();

	OnFail.Clear();

	MaxKBURL.Empty();

	APIKey.Empty();

	ReqInfo = FXGMaxKBChatReqInfo();

	ErrorSteamData.Empty();


	StreamData.Empty();

	NoFixStreamData.Empty();

	TotalString.Empty();




	SetReadyToDestroy();
}

