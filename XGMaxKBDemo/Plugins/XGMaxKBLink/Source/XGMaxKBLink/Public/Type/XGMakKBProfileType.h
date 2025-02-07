// Copyright 2025 Xiao Gang. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "XGMakKBProfileType.generated.h"


USTRUCT(BlueprintType)
struct XGMAXKBLINK_API FXGMaxKBProfileRespData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB")
	FString    id   = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB")
	FString    name = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB")
	FString    desc = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB")
	FString    prologue = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB")
	int32    dialogue_number = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB")
	FString    icon = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB")
	FString    type = TEXT("");


	//stt_model_id
	//tts_model_id
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB")
	bool    stt_model_enable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB")
	bool    tts_model_enable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB")
	FString    tts_type = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB")
	bool    file_upload_enable = false;

	//file_upload_setting

	//work_flow

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB")
	bool    show_source = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB")
	bool    multiple_rounds_dialogue = false;


};

USTRUCT(BlueprintType)
struct XGMAXKBLINK_API FXGMaxKBProfileRespInfo
{
	GENERATED_BODY()

public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB")
	int32    code = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB")
	FString    message = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB")
	FXGMaxKBProfileRespData	data ;


};

