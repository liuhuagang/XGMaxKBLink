// Copyright 2025 Xiao Gang. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "XGMaxKBOpenType.generated.h"


USTRUCT(BlueprintType)
struct XGMAXKBLINK_API FXGMaxKBOpenRespInfo
{
	GENERATED_BODY()

public:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB")
	int32   code = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB")
	FString message = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "XGMaxKB")
	FString	data=TEXT("");


};
