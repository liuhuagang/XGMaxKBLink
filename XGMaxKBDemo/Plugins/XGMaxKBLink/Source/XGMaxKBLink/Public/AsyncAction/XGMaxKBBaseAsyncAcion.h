// Copyright 2025 Xiao Gang. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "XGMaxKBBaseAsyncAcion.generated.h"

UCLASS()
class XGMAXKBLINK_API UXGMaxKBBaseAsyncAcion : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()


public:

	UXGMaxKBBaseAsyncAcion(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void Activate() override;

protected:

	virtual void Activate_Internal();

public:

	FGuid AsyncID;
};

