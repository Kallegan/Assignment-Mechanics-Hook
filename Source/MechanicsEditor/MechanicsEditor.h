#pragma once

#include "Engine.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "UnrealEd.h"

DECLARE_LOG_CATEGORY_EXTERN(MechanicsEditor, All, All)

class FMechanicsEditorModule : public IModuleInterface
{
//public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};