#pragma once

//Include File for Applications/Games who use the Flux Engine

#include "Flux/Application.h"
#include "Flux/Layer.h"
#include "Flux/Log.h"
#include "Flux/Input.h"
#include "Flux/MouseButtonCodes.h"
#include "Flux/KeyCodes.h"
#include "Flux/ImGui/ImGuiLayer.h"

#include "Flux/IScript.h"
#include "Flux/ImGui/ImGuiWrapper.h"

#include "Flux/ScriptManagerLayer.h"
#include "Flux/ScriptRegistry.h"

#ifdef __has_include
#  if __has_include("Generated/ScriptIncludes.gen.h")
#    include "Generated/ScriptIncludes.gen.h"
#  endif
#endif


//-------------------------Entrypoint-------------------------
#include "Flux/EntryPoint.h"
//-------------------------Entrypoint-------------------------
