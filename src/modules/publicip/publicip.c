#include "common/printing.h"
#include "common/jsonconfig.h"
#include "modules/publicip/publicip.h"
#include "detection/publicip/publicip.h"
#include "util/stringUtils.h"

#define FF_PUBLICIP_DISPLAY_NAME "Public IP"
#define FF_PUBLICIP_NUM_FORMAT_ARGS 1

void ffPrintPublicIp(FFPublicIpOptions* options)
{
    FFPublicIpResult result;
    ffStrbufInit(&result.ip);
    ffStrbufInit(&result.location);
    const char* error = ffDetectPublicIp(options, &result);

    if (error)
    {
        ffPrintError(FF_PUBLICIP_DISPLAY_NAME, 0, &options->moduleArgs, "%s", error);
        return;
    }

    if (options->moduleArgs.outputFormat.length == 0)
    {
        ffPrintLogoAndKey(FF_PUBLICIP_DISPLAY_NAME, 0, &options->moduleArgs, FF_PRINT_TYPE_DEFAULT);
        if (result.location.length)
            printf("%s (%s)\n", result.ip.chars, result.location.chars);
        else
            ffStrbufPutTo(&result.ip, stdout);
    }
    else
    {
        ffPrintFormat(FF_PUBLICIP_DISPLAY_NAME, 0, &options->moduleArgs, FF_PUBLICIP_NUM_FORMAT_ARGS, (FFformatarg[]) {
            {FF_FORMAT_ARG_TYPE_STRBUF, &result.ip},
            {FF_FORMAT_ARG_TYPE_STRBUF, &result.location},
        });
    }

    ffStrbufDestroy(&result.ip);
    ffStrbufDestroy(&result.location);
}

void ffInitPublicIpOptions(FFPublicIpOptions* options)
{
    ffOptionInitModuleBaseInfo(&options->moduleInfo, FF_PUBLICIP_MODULE_NAME, ffParsePublicIpCommandOptions, ffParsePublicIpJsonObject, ffPrintPublicIp, ffGeneratePublicIpJson, ffPrintPublicIpHelpFormat);
    ffOptionInitModuleArg(&options->moduleArgs);

    ffStrbufInit(&options->url);
    options->timeout = 0;
}

bool ffParsePublicIpCommandOptions(FFPublicIpOptions* options, const char* key, const char* value)
{
    const char* subKey = ffOptionTestPrefix(key, FF_PUBLICIP_MODULE_NAME);
    if (!subKey) return false;
    if (ffOptionParseModuleArgs(key, subKey, value, &options->moduleArgs))
        return true;

    if (ffStrEqualsIgnCase(subKey, "url"))
    {
        ffOptionParseString(key, value, &options->url);
        return true;
    }

    if (ffStrEqualsIgnCase(subKey, "timeout"))
    {
        options->timeout = ffOptionParseUInt32(key, value);
        return true;
    }

    return false;
}

void ffDestroyPublicIpOptions(FFPublicIpOptions* options)
{
    ffOptionDestroyModuleArg(&options->moduleArgs);

    ffStrbufDestroy(&options->url);
}

void ffParsePublicIpJsonObject(FFPublicIpOptions* options, yyjson_val* module)
{
    yyjson_val *key_, *val;
    size_t idx, max;
    yyjson_obj_foreach(module, idx, max, key_, val)
    {
        const char* key = yyjson_get_str(key_);
        if(ffStrEqualsIgnCase(key, "type"))
            continue;

        if (ffJsonConfigParseModuleArgs(key, val, &options->moduleArgs))
            continue;

        if (ffStrEqualsIgnCase(key, "url"))
        {
            ffStrbufSetS(&options->url, yyjson_get_str(val));
            continue;
        }

        if (ffStrEqualsIgnCase(key, "timeout"))
        {
            options->timeout = (uint32_t) yyjson_get_uint(val);
            continue;
        }

        ffPrintError(FF_PUBLICIP_MODULE_NAME, 0, &options->moduleArgs, "Unknown JSON key %s", key);
    }
}

void ffGeneratePublicIpJson(FFPublicIpOptions* options, yyjson_mut_doc* doc, yyjson_mut_val* module)
{
    FFPublicIpResult result;
    ffStrbufInit(&result.ip);
    ffStrbufInit(&result.location);
    const char* error = ffDetectPublicIp(options, &result);

    if (error)
    {
        yyjson_mut_obj_add_str(doc, module, "error", error);
        return;
    }

    yyjson_mut_val* obj = yyjson_mut_obj_add_obj(doc, module, "result");
    yyjson_mut_obj_add_strbuf(doc, obj, "ip", &result.ip);
    yyjson_mut_obj_add_strbuf(doc, obj, "location", &result.location);

    ffStrbufDestroy(&result.ip);
    ffStrbufDestroy(&result.location);
}

void ffPrintPublicIpHelpFormat(void)
{
    ffPrintModuleFormatHelp(FF_PUBLICIP_MODULE_NAME, "{1} ({2})", FF_PUBLICIP_NUM_FORMAT_ARGS, (const char* []) {
        "Public IP address",
        "Location"
    });
}
