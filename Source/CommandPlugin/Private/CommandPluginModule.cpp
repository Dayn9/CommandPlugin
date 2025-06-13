// Copyright (c) 2024, Dane Sherman. All rights reserved.

#include "CommandPluginModule.h"

#define LOCTEXT_NAMESPACE "FCommandPluginModule"

DEFINE_LOG_CATEGORY(LogCommandPlugin)

void FCommandPluginModule::StartupModule()
{

}

void FCommandPluginModule::ShutdownModule()
{

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCommandPluginModule, CommandPluginModule)