#pragma once

#include "fastfetch.h"

#define FF_CPUUSAGE_MODULE_NAME "CPUUsage"

void ffPrepareCPUUsage();

void ffPrintCPUUsage(FFCPUUsageOptions* options);
void ffInitCPUUsageOptions(FFCPUUsageOptions* options);
bool ffParseCPUUsageCommandOptions(FFCPUUsageOptions* options, const char* key, const char* value);
void ffDestroyCPUUsageOptions(FFCPUUsageOptions* options);
void ffParseCPUUsageJsonObject(FFCPUUsageOptions* options, yyjson_val* module);
void ffGenerateCPUUsageJson(FFCPUUsageOptions* options, yyjson_mut_doc* doc, yyjson_mut_val* module);
void ffPrintCPUUsageHelpFormat(void);
