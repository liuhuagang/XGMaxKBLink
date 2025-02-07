// Copyright 2025 Xiao Gang. All Rights Reserved.

#include "XGMaxKBBaseAsyncAcion.h"

#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"
UXGMaxKBBaseAsyncAcion::UXGMaxKBBaseAsyncAcion(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer),
	AsyncID(FGuid::NewGuid())
{
}

void UXGMaxKBBaseAsyncAcion::Activate()
{
	Super::Activate();

	AsyncTask(ENamedThreads::GameThread, [this]() {

		this->Activate_Internal();

		});
}

void UXGMaxKBBaseAsyncAcion::Activate_Internal()
{

}
