#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "mg-base.h"
#include "glue_common.h"
#include "glue_utils.h"

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

static map<string, EnumType*> *sNamedEnumTypes;
static map<string, Property*> *sNamedProperties;

map<mWidgetClass*, WidgetClassDefine*>* WidgetClassDefine::sWidgetMaps;
map<string, WidgetClassDefine*>* WidgetClassDefine::sNamedWidgetMaps;

static PropType * getPropType(int type) {
    if (type >= 0 && type < PropType::ENUM) {
        return _base_types[type];
    }

    return NULL;
}

static Property* getNamedProperty(const char* name) {
    map<string, Property*>::iterator it = sNamedProperties->find(name);
    if (it == sNamedProperties->end()) {
        return NULL;
    }
    return it->second;
}

Property* Property::getProperty(const char* name) {
    return getNamedProperty(name);
}

static PropType * getNamedEnumType(const char* name) {
    map<string, EnumType*>::iterator it = sNamedEnumTypes->find(name);
    if (it == sNamedEnumTypes->end()) {
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
    return ptype;
}

PropType* PropValue::getPropType() {
    return prop ? prop->type : NULL;
}

void WidgetClassDefine::addClassDefine(WidgetClassDefine* define) {
    if (sWidgetMaps == NULL) {
        sWidgetMaps = new map<mWidgetClass*, WidgetClassDefine*>;
    }
    (*sWidgetMaps)[define->getOwnerClass()] = define;

    if (sNamedWidgetMaps == NULL) {
        sNamedWidgetMaps = new map<string, WidgetClassDefine*>;
    }
    (*sNamedWidgetMaps)[define->className] = define;
}

WidgetClassDefine* WidgetClassDefine::getClassDefine(mWidgetClass* ownerClass) {
    if (sWidgetMaps == NULL) return NULL;
    map<mWidgetClass*, WidgetClassDefine*>::iterator it =
            sWidgetMaps->find(ownerClass);
    if (it == sWidgetMaps->end()) {
        return NULL;
    }

    return it->second;
}

WidgetClassDefine* WidgetClassDefine::getClassDefine(const char* name) {
    if (sNamedWidgetMaps == NULL) return NULL;

    map<string, WidgetClassDefine*>::iterator it =
        sNamedWidgetMaps->find(name);
    if (it == sNamedWidgetMaps->end()) {
        return NULL;
    }
    return it->second;
}

void WidgetClassDefine::initWndTemplateByDefaults(WndTemplateBuilder* pbuilder) {
    if (getParent()) {
        getParent()->initWndTemplateByDefaults(pbuilder);
    }

    for(map<string, Property*>::iterator it = properties.begin();
            it != properties.end(); ++ it) {
        Property* prop = it->second;
        if (prop->hasDefValue()) {
            pbuilder->setProp(prop->id, prop->getDefValue());
        }
    }

    for(map<string, PropValue*>::iterator it = defValues.begin();
            it != defValues.end(); ++ it) {
        PropValue* defVal = it->second;
        pbuilder->setProp(defVal->prop->id, defVal->to());
    }

    pbuilder->setWndClassName(ownerClass->className);
}

void* GetWidgetEventHandlers(mWidget *widget) {
    BOOL ok = FALSE;
    return (void*)_c(widget)->getSpecificData(widget, WIDGET_KEY, &ok);
}

void SetWidgetEventHandlers(mWidget* widget, void *pt) {
    _c(widget)->setSpecificData(widget, WIDGET_KEY, (DWORD)pt, NULL/*TODO free*/);
}


//////////////////////////////////////////////////////////////////////////

#include "widget_classes_list.cpp"


bool InitGlue() {
    sNamedEnumTypes = new map<string, EnumType*>;
    sNamedProperties = new map<string, Property*>;
    create_enum_types(*sNamedEnumTypes);
    create_all_props(*sNamedProperties);
    init_widget_class(sWidgetClasses);
    return true;
}


extern "C" unsigned long RunV8Script(const char* script, const char* filename);
extern "C" unsigned long RunPythonScript(const char* script, const char* filename);
extern "C" unsigned long RunLuaScript(const char* script, const char* filename);

static struct {
    const char* ext;
    unsigned long(*run)(const char*,const char*);
} _script_dispatcher[] = {
    {"js", RunV8Script },
    {"py", RunPythonScript},
    {"lua", RunLuaScript},
    {NULL, NULL}
};


unsigned long RunScript(const char* script, const char* filename) {

    const char* str_ext = strrchr(filename, '.');
    for (int i = 0; _script_dispatcher[i].ext; i++) {
        if (strcmp(str_ext+1, _script_dispatcher[i].ext) == 0) {
            return _script_dispatcher[i].run(script, filename);
        }
    }

    ALOGE("MiniGUI Script", "unkwown script type:%s", filename);
    return 0;
}

void dumpWndTemplate(const NCS_WND_TEMPLATE* tmpl, char* prefix) {
    int len = strlen(prefix);
#define _D_TMP(format, member) ALOGI("MINIGUI", "\t%s"#member "= " format, prefix, tmpl->member)
    ALOGI("MINIGUI", "%s{", prefix);
    _D_TMP("%s", class_name);
    _D_TMP("%d", id);
    _D_TMP("%d", x);
    _D_TMP("%d", y);
    _D_TMP("%d", w);
    _D_TMP("%d", h);
    _D_TMP("0x%08X", style);
    _D_TMP("0x%08X", ex_style);
    _D_TMP("%s", caption);
    if (tmpl->props) {
        for (int i = 0; tmpl->props[i].id > 0; i++) {
            ALOGI("MINIGUI", "\t%sProp %d=0x%08x", prefix, tmpl->props[i].id, tmpl->props[i].value);
        }
    }
    if (tmpl->rdr_info) {
        ALOGI("MINIGUI", "\t%sglobal render=%s", tmpl->rdr_info->glb_rdr);
        ALOGI("MINIGUI", "\t%scontrol render=%s", tmpl->rdr_info->ctl_rdr);
        if (tmpl->rdr_info->elements) {
            for(int i = 0; tmpl->rdr_info->elements[i].id > 0; i++) {
                ALOGI("MINIGUI", "\t%sRdr %d=0x%08x", tmpl->rdr_info->elements[i].id, tmpl->rdr_info->elements[i].value);
            }
        }
    }

    if(tmpl->handlers) {
        for (int i = 0; tmpl->handlers[i].handler != NULL; i++) {
            ALOGI("MINIGUI", "\t%sHandler message 0x%08x: handler:%p", prefix, tmpl->handlers[i].message, tmpl->handlers[i].handler);
        }
    }

    _D_TMP("0x%08x", bk_color);
    _D_TMP("%s", font_name);

    if (tmpl->ctrls) {
        prefix[len] = '\t';
        prefix[len+1] = '\0';
        for (int i = 0; i < tmpl->count; i++) {
            dumpWndTemplate(&tmpl->ctrls[i], prefix);
        }
        prefix[len] = '\0';
    }

    ALOGI("MINIGUI", "%s}", prefix);
#undef _D_TMP
}


}
