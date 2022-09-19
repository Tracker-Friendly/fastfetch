#include "cfdict_helpers.h"

bool ffCfDictGetString(CFMutableDictionaryRef dict, CFStringRef key, FFstrbuf* result)
{
    CFTypeRef cf = (CFTypeRef)CFDictionaryGetValue(dict, key);
    if(cf == NULL)
        return false;

    if(CFGetTypeID(cf) == CFStringGetTypeID())
    {
        CFStringRef cfStr = (CFStringRef)cf;
        uint32_t length = (uint32_t)CFStringGetLength(cfStr);
        ffStrbufEnsureFree(result, length + 1);
        if(CFStringGetCString(cfStr, result->chars, length + 1, kCFStringEncodingASCII))
        {
            result->length = length;
            // CFStringGetCString ensures the buffer is NUL terminated
            // https://developer.apple.com/documentation/corefoundation/1542721-cfstringgetcstring
        }
    }
    else if(CFGetTypeID(cf) == CFDataGetTypeID())
    {
        CFDataRef cfData = (CFDataRef)cf;
        uint32_t length = (uint32_t)CFDataGetLength(cfData);
        ffStrbufEnsureFree(result, length + 1);
        CFDataGetBytes(cfData, CFRangeMake(0, length), (uint8_t*)result->chars);
        result->length = (uint32_t)strnlen(result->chars, length);
        result->chars[result->length] = '\0';
    }
    else
    {
        return false;
    }
    return true;
}

bool ffCfDictGetBool(CFMutableDictionaryRef dict, CFStringRef key, bool* result)
{
    CFBooleanRef cf = (CFBooleanRef)CFDictionaryGetValue(dict, key);
    if(cf == NULL || CFGetTypeID(cf) != CFBooleanGetTypeID())
        return false;

    *result = CFBooleanGetValue(cf);
    return true;
}

bool ffCfDictGetInt(CFMutableDictionaryRef dict, CFStringRef key, int* result)
{
    CFNumberRef cf = (CFNumberRef)CFDictionaryGetValue(dict, key);
    if (cf == NULL || CFGetTypeID(cf) != CFNumberGetTypeID())
        return false;

    if(!CFNumberGetValue(cf, kCFNumberSInt32Type, result))
        return false;
    return true;
}