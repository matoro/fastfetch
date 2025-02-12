#pragma once

#include "fastfetch.h"

#define FF_GAMEPAD_MODULE_NAME "Gamepad"

void ffPrintGamepad(FFGamepadOptions* options);
void ffInitGamepadOptions(FFGamepadOptions* options);
bool ffParseGamepadCommandOptions(FFGamepadOptions* options, const char* key, const char* value);
void ffDestroyGamepadOptions(FFGamepadOptions* options);
void ffParseGamepadJsonObject(FFGamepadOptions* options, yyjson_val* module);
void ffGenerateGamepadJson(FFGamepadOptions* options, yyjson_mut_doc* doc, yyjson_mut_val* module);
void ffPrintGamepadHelpFormat(void);
