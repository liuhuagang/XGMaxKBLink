// Copyright 2025 Xiao Gang. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Interfaces/IHttpRequest.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

#include "XGMaxKBBaseAsyncAcion.h"
#include "XGMaxKBChatType.h"

#include "XGMaxKBChatAsyncAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FXGMaxKBChatDelegate,
	FGuid, AsyncID,
	bool, bResult,
	FString, Message,
	FXGMaxKBChatRespInfo, RespInfo);

UCLASS(meta = (HideThen = true))
class XGMAXKBLINK_API UXGMaxKBChatAsyncAction : public UXGMaxKBBaseAsyncAcion
{
	GENERATED_BODY()

public:

	UXGMaxKBChatAsyncAction(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual ~UXGMaxKBChatAsyncAction();

public:

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true",
		WorldContext = "WorldContextObject",
		DisplayName = "XGMaxKBChatAsyncAction",
		Keywords = "XGMaxKB Chat AsyncAction"),
		Category = "XGMaxKB")
	static UXGMaxKBChatAsyncAction* XGMaxKBChatAsyncAction(UObject* WorldContextObject,
		const FString& InMaxKBURL = TEXT("http://127.0.0.1:32768/api/application/chat_message/{chat_id}"),
		const FString& InAPIKey =TEXT(""),
		FXGMaxKBChatReqInfo InReqInfo= FXGMaxKBChatReqInfo());

public:

	virtual void Activate() override;

	virtual void Activate_Internal();


protected:

	void SendHttp(const FString& InServerURL, const FString& InContentString, TMap<FString, FString> InHeaders);

	void OnHttpRespReceived(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	bool HttpSream(void* Ptr, int64 Length);

	void HandleStreamData(TArray<uint8> InStreamData);

	void RecursiveProcessingStreamData();

	void CallOnSuccess(FGuid InAsyncID, bool bInResult, FString InMessage, FXGMaxKBChatRespInfo RespInfo);

	void CallOnUpdate(FGuid InAsyncID, bool bInResult, FString InMessage, FXGMaxKBChatRespInfo RespInfo);

	void CallOnFail(FGuid InAsyncID, bool bInResult, FString InMessage, FXGMaxKBChatRespInfo RespInfo);

	void RealeaseResources();


public:

	UPROPERTY(BlueprintAssignable)
	FXGMaxKBChatDelegate Then;

	UPROPERTY(BlueprintAssignable)
	FXGMaxKBChatDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FXGMaxKBChatDelegate OnUpdate;

	UPROPERTY(BlueprintAssignable)
	FXGMaxKBChatDelegate OnFail;

protected:

	FString MaxKBURL = TEXT("");

	FString APIKey = TEXT("");

	FXGMaxKBChatReqInfo ReqInfo;

	TArray<uint8> ErrorSteamData;

	TArray<uint8> StreamData;

	TArray<uint8> NoFixStreamData;

	FString TotalString = TEXT("");
};


