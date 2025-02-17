#include "common/printing.h"
#include "common/jsonconfig.h"
#include "detection/wmtheme/wmtheme.h"
#include "modules/wmtheme/wmtheme.h"
#include "util/stringUtils.h"

#define FF_WMTHEME_DISPLAY_NAME "WM Theme"
#define FF_WMTHEME_NUM_FORMAT_ARGS 1

void ffPrintWMTheme(FFWMThemeOptions* options)
{
    FF_STRBUF_AUTO_DESTROY themeOrError = ffStrbufCreate();
    if(ffDetectWmTheme(&themeOrError))
    {
        if(options->moduleArgs.outputFormat.length == 0)
        {
            ffPrintLogoAndKey(FF_WMTHEME_DISPLAY_NAME, 0, &options->moduleArgs, FF_PRINT_TYPE_DEFAULT);
            puts(themeOrError.chars);
        }
        else
        {
            ffPrintFormat(FF_WMTHEME_DISPLAY_NAME, 0, &options->moduleArgs, FF_WMTHEME_NUM_FORMAT_ARGS, (FFformatarg[]){
                {FF_FORMAT_ARG_TYPE_STRBUF, &themeOrError}
            });
        }
    }
    else
    {
        ffPrintError(FF_WMTHEME_DISPLAY_NAME, 0, &options->moduleArgs, "%*s", themeOrError.length, themeOrError.chars);
    }
}

void ffInitWMThemeOptions(FFWMThemeOptions* options)
{
    ffOptionInitModuleBaseInfo(&options->moduleInfo, FF_WMTHEME_MODULE_NAME, ffParseWMThemeCommandOptions, ffParseWMThemeJsonObject, ffPrintWMTheme);
    ffOptionInitModuleArg(&options->moduleArgs);
}

bool ffParseWMThemeCommandOptions(FFWMThemeOptions* options, const char* key, const char* value)
{
    const char* subKey = ffOptionTestPrefix(key, FF_WMTHEME_MODULE_NAME);
    if (!subKey) return false;
    if (ffOptionParseModuleArgs(key, subKey, value, &options->moduleArgs))
        return true;

    return false;
}

void ffDestroyWMThemeOptions(FFWMThemeOptions* options)
{
    ffOptionDestroyModuleArg(&options->moduleArgs);
}

void ffParseWMThemeJsonObject(FFWMThemeOptions* options, yyjson_val* module)
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

        ffPrintError(FF_WMTHEME_DISPLAY_NAME, 0, &options->moduleArgs, "Unknown JSON key %s", key);
    }
}
