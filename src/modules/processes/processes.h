#pragma once

#include "fastfetch.h"

#define FF_PROCESSES_MODULE_NAME "Processes"

void ffPrintProcesses(FFProcessesOptions* options);
void ffInitProcessesOptions(FFProcessesOptions* options);
bool ffParseProcessesCommandOptions(FFProcessesOptions* options, const char* key, const char* value);
void ffDestroyProcessesOptions(FFProcessesOptions* options);
void ffParseProcessesJsonObject(FFProcessesOptions* options, yyjson_val* module);
void ffGenerateProcessesJson(FFProcessesOptions* options, yyjson_mut_doc* doc, yyjson_mut_val* module);
void ffPrintProcessesHelpFormat(void);
