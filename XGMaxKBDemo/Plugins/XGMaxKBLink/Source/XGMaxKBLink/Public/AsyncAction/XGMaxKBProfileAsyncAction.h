// Copyright 2025 Xiao Gang. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Interfaces/IHttpRequest.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

#include "XGMaxKBBaseAsyncAcion.h"
#include "XGMakKBProfileType.h"



#include "XGMaxKBProfileAsyncAction.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FXGMaxKBProfileDelegate,
	FGuid, AsyncID,
	bool, bResult,
	FString, Message,
	FXGMaxKBProfileRespInfo, RespInfo);



UCLASS(meta = (HideThen = true))
class XGMAXKBLINK_API UXGMaxKBProfileAsyncAction : public UXGMaxKBBaseAsyncAcion
{
	GENERATED_BODY()

public:

	UXGMaxKBProfileAsyncAction(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual ~UXGMaxKBProfileAsyncAction();

public:

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true",
		WorldContext = "WorldContextObject",
		DisplayName = "XGMaxKBProfileAsyncAcion",
		Keywords = "XG MaxKB Profile AsyncAcion"),
		Category = "XGMaxKB")
	static UXGMaxKBProfileAsyncAction* XGMaxKBProfileAsyncAcion(UObject* WorldContextObject,
		const FString& InMaxKBURL = TEXT("http://{ip}:{Port}/api/application/profile"),
		const FString& InAPIKey = TEXT(""));

	virtual void Activate() override;

public:


	UPROPERTY(BlueprintAssignable)
	FXGMaxKBProfileDelegate Then;

	UPROPERTY(BlueprintAssignable)
	FXGMaxKBProfileDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FXGMaxKBProfileDelegate OnFail;


protected:

	virtual void Activate_Internal() override;

protected:

	void SendHttp(const FString& InServerURL,TMap<FString, FString> InHeaders);

	void OnHttpRespReceived(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	void CallOnSuccess(FGuid InAsyncID, bool bInResult, FString InMessage, FXGMaxKBProfileRespInfo RespInfo);

	void CallOnFail(FGuid InAsyncID, bool bInResult, FString InMessage, FXGMaxKBProfileRespInfo RespInfo);

	void RealeaseResources();

protected:

	FString MaxKBURL = TEXT("");

	FString APIKey = TEXT("");

};

