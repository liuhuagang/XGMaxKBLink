// Copyright 2025 Xiao Gang. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Interfaces/IHttpRequest.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

#include "XGMaxKBBaseAsyncAcion.h"
#include "XGMaxKBOpenType.h"

#include "XGMaxKBOpenAsyncAction.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FXGMaxKBOpenDelegate,
	FGuid, AsyncID,
	bool, bResult,
	FString, Message,
	FXGMaxKBOpenRespInfo, RespInfo);



UCLASS(meta = (HideThen = true))
class XGMAXKBLINK_API UXGMaxKBOpenAsyncAction : public UXGMaxKBBaseAsyncAcion
{
	GENERATED_BODY()

public:

	UXGMaxKBOpenAsyncAction(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual ~UXGMaxKBOpenAsyncAction();

public:

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true",
		WorldContext = "WorldContextObject",
		DisplayName = "XGMaxKBOpenAsyncAcion",
		Keywords = "XG MaxKB Open AsyncAcion"),
		Category = "XGMaxKB")
	static UXGMaxKBOpenAsyncAction* XGMaxKBOpenAsyncAcion(UObject* WorldContextObject,
		const FString& InMaxKBURL = TEXT("http://127.0.0.1:32768/api/application/{application_id}/chat/open"),
		const FString& InAPIKey = TEXT(""));

	virtual void Activate() override;

public:


	UPROPERTY(BlueprintAssignable)
	FXGMaxKBOpenDelegate Then;

	UPROPERTY(BlueprintAssignable)
	FXGMaxKBOpenDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FXGMaxKBOpenDelegate OnFail;


protected:

	virtual void Activate_Internal() override;

protected:

	void SendHttp(const FString& InServerURL, TMap<FString, FString> InHeaders);

	void OnHttpRespReceived(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	void CallOnSuccess(FGuid InAsyncID, bool bInResult, FString InMessage, FXGMaxKBOpenRespInfo RespInfo);

	void CallOnFail(FGuid InAsyncID, bool bInResult, FString InMessage, FXGMaxKBOpenRespInfo RespInfo);

	void RealeaseResources();

protected:

	FString MaxKBURL = TEXT("");

	FString APIKey = TEXT("");

};

