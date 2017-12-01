#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "glue_common.h"

namespace glue {

static vector<WidgetClassDefine*> sWidgetClasses;
static IntType    _int_type;
static ColorType  _color_type;
static GroupType  _group_type;
static StringType _string_type;
static TextType   _text_type;
static ImageType  _image_type;
static FontType   _font_type;
static RdrType    _rdr_type;

static PropType* _base_types [] = {
    &_int_type,
    &_string_type,
    &_color_type,
    &_text_type,
    &_image_type,
    &_group_type,
    &_font_type,
    &_rdr_type
};

static map<string, EnumType*> sNamedEnumTypes;
static map<string, Property*> sNamedProperties;
static map<string, int> sNamedEventIds;

map<mWidgetClass*, WidgetClassDefine*> WidgetClassDefine::widgetMaps;

static PropType * getPropType(int type) {
    if (type >= 0 && type < PropType::ENUM) {
        return _base_types[type];
    }

    return NULL;
}

static Property* getNamedProperty(const char* name) {
    map<string, Property*>::iterator it = sNamedProperties.find(name);
    if (it == sNamedProperties.end()) {
        return NULL;
    }
    return it->second;
}

Property* Property::getProperty(const char* name) {
    return getNamedProperty(name);
}

static PropType * getNamedEnumType(const char* name) {
    map<string, EnumType*>::iterator it = sNamedEnumTypes.find(name);
    if (it == sNamedEnumTypes.end()) {
        return NULL;
    }

    return it->second;
}

EnumType* EnumType::create(const char* name, ...) {
    EnumType *ptype = new EnumType;

    if (name != NULL) {
        ptype->name = name;
    }

    va_list arg;
    va_start(arg, name);

    while (true) {
        const char* opname = va_arg(arg, const char*);
        if (opname == NULL) {
            break;
        }

        int opval = va_arg(arg, int);

        ptype->valMaps[opname] = opval;
    }

    va_end(arg);
}

void WidgetClassDefine::addClassDefine(WidgetClassDefine* define) {
    widgetMaps[define->getOwnerClass()] = define;
}

WidgetClassDefine* WidgetClassDefine::getClassDefine(mWidgetClass* ownerClass) {
    map<mWidgetClass*, WidgetClassDefine*>::iterator it =
            widgetMaps.find(ownerClass);
    if (it == widgetMaps.end()) {
        return NULL;
    }

    return it->second;
}

void* GetWidgetEventHandlers(mWidget *widget) {
    BOOL ok = FALSE;
    return (void*)_c(widget)->getSpecificData(widget, WIDGET_KEY, &ok);
}

void SetWidgetEventHandlers(mWidget* widget, void *pt) {
    _c(widget)->setSpecificData(widget, WIDGET_KEY, (DWORD)pt, NULL/*TODO free*/);
}


int GetEventIdByName(const char* name) {
    map<string, int>::iterator it = sNamedEventIds.find(name);
    if (it == sNamedEventIds.end()) {
        return -1;
    }
    return it->second;
}


//////////////////////////////////////////////////////////////////////////

#include "widget_classes_list.cpp"


bool InitGlue() {
    create_enum_types(sNamedEnumTypes);
    create_all_props(sNamedProperties);
    init_widget_class(sWidgetClasses);
    init_event_ids(sNamedEventIds);
    return true;
}


extern "C" unsigned long RunV8Script(const char* script, const char* filename);

unsigned long RunScript(const char* script, const char* filename) {

    const char* str_ext = strrchr(filename, '.');
    if (str_ext && strcmp(str_ext, ".js") == 0) {
        return RunV8Script(script, filename);
    }

    ALOGE("MiniGUI Script", "unkwown script type:%s", filename);
    return 0;
}


}
