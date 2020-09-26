// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"
#include "Engine/Engine.h"
#include "Modules/ModuleManager.h"

class FInventorySystemModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

INVENTORYSYSTEM_API DECLARE_LOG_CATEGORY_EXTERN(LogInventory, Log, All);