#include "common/printing.h"
#include "common/jsonconfig.h"
#include "detection/font/font.h"
#include "modules/font/font.h"
#include "util/stringUtils.h"

#define FF_FONT_NUM_FORMAT_ARGS (FF_DETECT_FONT_NUM_FONTS + 1)

void ffPrintFont(FFFontOptions* options)
{
    FFFontResult font;
    for(uint32_t i = 0; i < FF_DETECT_FONT_NUM_FONTS; ++i)
        ffStrbufInit(&font.fonts[i]);
    ffStrbufInit(&font.display);

    const char* error = ffDetectFont(&font);

    if(error)
    {
        ffPrintError(FF_FONT_MODULE_NAME, 0, &options->moduleArgs, "%s", error);
    }
    else
    {
        if(options->moduleArgs.outputFormat.length == 0)
        {
            ffPrintLogoAndKey(FF_FONT_MODULE_NAME, 0, &options->moduleArgs, FF_PRINT_TYPE_DEFAULT);
            ffStrbufPutTo(&font.display, stdout);
        }
        else
        {
            ffPrintFormat(FF_FONT_MODULE_NAME, 0, &options->moduleArgs, FF_FONT_NUM_FORMAT_ARGS, (FFformatarg[]) {
                {FF_FORMAT_ARG_TYPE_STRBUF, &font.fonts[0]},
                {FF_FORMAT_ARG_TYPE_STRBUF, &font.fonts[1]},
                {FF_FORMAT_ARG_TYPE_STRBUF, &font.fonts[2]},
                {FF_FORMAT_ARG_TYPE_STRBUF, &font.fonts[3]},
                {FF_FORMAT_ARG_TYPE_STRBUF, &font.display},
            });
        }
    }

    ffStrbufDestroy(&font.display);
    for (uint32_t i = 0; i < FF_DETECT_FONT_NUM_FONTS; ++i)
        ffStrbufDestroy(&font.fonts[i]);
}

void ffInitFontOptions(FFFontOptions* options)
{
    ffOptionInitModuleBaseInfo(&options->moduleInfo, FF_FONT_MODULE_NAME, ffParseFontCommandOptions, ffParseFontJsonObject, ffPrintFont, ffGenerateFontJson, ffPrintFontHelpFormat);
    ffOptionInitModuleArg(&options->moduleArgs);
}

bool ffParseFontCommandOptions(FFFontOptions* options, const char* key, const char* value)
{
    const char* subKey = ffOptionTestPrefix(key, FF_FONT_MODULE_NAME);
    if (!subKey) return false;
    if (ffOptionParseModuleArgs(key, subKey, value, &options->moduleArgs))
        return true;

    return false;
}

void ffDestroyFontOptions(FFFontOptions* options)
{
    ffOptionDestroyModuleArg(&options->moduleArgs);
}

void ffParseFontJsonObject(FFFontOptions* options, yyjson_val* module)
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

        ffPrintError(FF_FONT_MODULE_NAME, 0, &options->moduleArgs, "Unknown JSON key %s", key);
    }
}

void ffGenerateFontJson(FF_MAYBE_UNUSED FFFontOptions* options, yyjson_mut_doc* doc, yyjson_mut_val* module)
{
    FFFontResult font;
    for(uint32_t i = 0; i < FF_DETECT_FONT_NUM_FONTS; ++i)
        ffStrbufInit(&font.fonts[i]);
    ffStrbufInit(&font.display);

    const char* error = ffDetectFont(&font);
    if(error)
    {
        yyjson_mut_obj_add_str(doc, module, "error", error);
    }
    else
    {
        yyjson_mut_val* obj = yyjson_mut_obj_add_obj(doc, module, "result");
        yyjson_mut_obj_add_strbuf(doc, obj, "display", &font.display);
        yyjson_mut_val* fontsArr = yyjson_mut_obj_add_arr(doc, obj, "fonts");
        for (uint32_t i = 0; i < FF_DETECT_FONT_NUM_FONTS; ++i)
            yyjson_mut_arr_add_strbuf(doc, fontsArr, &font.fonts[i]);
    }

    ffStrbufDestroy(&font.display);
    for (uint32_t i = 0; i < FF_DETECT_FONT_NUM_FONTS; ++i)
        ffStrbufDestroy(&font.fonts[i]);
}

void ffPrintFontHelpFormat(void)
{
    ffPrintModuleFormatHelp(FF_FONT_MODULE_NAME, "{5}", FF_FONT_NUM_FORMAT_ARGS, (const char* []) {
        "Font 1",
        "Font 2",
        "Font 3",
        "Font 4",
        "Combined fonts"
    });
}
