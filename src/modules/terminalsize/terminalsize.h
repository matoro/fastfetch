#pragma once

#include "fastfetch.h"

#define FF_TERMINALSIZE_MODULE_NAME "TerminalSize"

void ffPrintTerminalSize(FFTerminalSizeOptions* options);
void ffInitTerminalSizeOptions(FFTerminalSizeOptions* options);
bool ffParseTerminalSizeCommandOptions(FFTerminalSizeOptions* options, const char* key, const char* value);
void ffDestroyTerminalSizeOptions(FFTerminalSizeOptions* options);
void ffParseTerminalSizeJsonObject(FFTerminalSizeOptions* options, yyjson_val* module);
void ffGenerateTerminalSizeJson(FFTerminalOptions* options, yyjson_mut_doc* doc, yyjson_mut_val* module);
void ffPrintTerminalSizeHelpFormat(void);
