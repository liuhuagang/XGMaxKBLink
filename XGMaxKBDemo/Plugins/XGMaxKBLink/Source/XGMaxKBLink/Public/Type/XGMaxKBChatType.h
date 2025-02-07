// Copyright 2025 Xiao Gang. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "XGMaxKBChatType.generated.h"


USTRUCT(BlueprintType)
struct XGMAXKBLINK_API FXGMaxKBChatUsageInfo
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	int32 completion_tokens = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	int32 prompt_tokens = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	int32 total_tokens = -1;

};



USTRUCT(BlueprintType)
struct XGMAXKBLINK_API FXGMaxKBMiddleStreamRespMessage
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	FString chat_id = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	FString chat_record_id = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	bool operate = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	FString content = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	FString node_id = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	TArray<FString> up_node_id_list;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	bool is_end = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	FXGMaxKBChatUsageInfo usage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	bool node_is_end = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	FString view_type = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	FString node_type = TEXT("");

};

USTRUCT(BlueprintType)
struct XGMAXKBLINK_API FXGMaxKBFinalStreamRespMessage
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	FXGMaxKBChatUsageInfo usage;

};



USTRUCT(BlueprintType)
struct XGMAXKBLINK_API FXGMaxKBChatNoStreamRespData
{
	GENERATED_USTRUCT_BODY()

public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	FString chat_id = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	FString id = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	bool operate = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	FString content = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	bool is_end = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	int32 prompt_tokens = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	int32 total_tokens = -1;
};

USTRUCT(BlueprintType)
struct XGMAXKBLINK_API FXGMaxKBChatNoStreamRespMessage
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	FString code = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	FString message = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	FXGMaxKBChatNoStreamRespData data ;




};





#pragma region ErrorMessage


USTRUCT(BlueprintType)
struct XGMAXKBLINK_API FXGMaxKBChatErrorMessage
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	FString code = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	FString message = TEXT("");

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGVolcano ChatCompletion")
	//FString data = TEXT("");

};

#pragma endregion ErrorMessage









USTRUCT(BlueprintType)
struct XGMAXKBLINK_API FXGMaxKBChatRespInfo
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	bool bResult = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	bool bReqStream = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	FString MessageContent = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	FXGMaxKBChatErrorMessage ErrorRespMessage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	FXGMaxKBMiddleStreamRespMessage StreamMiddleRespMessage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	FXGMaxKBFinalStreamRespMessage StreamFinalRespMessage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	FXGMaxKBChatNoStreamRespMessage NoStreamRespMessage;
};


USTRUCT(BlueprintType)
struct XGMAXKBLINK_API FXGMaxKBChatReqInfo
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	FString message = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	bool re_chat = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB|Chat")
	bool stream = true;

};
