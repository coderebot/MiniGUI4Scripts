#ifndef GLUE_COMMON_H
#define GLUE_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <map>
#include "common.h"
#include "minigui.h"
#include "gdi.h"
#include "window.h"
#include "mgncs.h"


using std::vector;
using std::string;
using std::map;

class WndTemplateBuilder;

namespace glue {

class PropType {
public:
    PropType() { }
    virtual ~PropType() { }

    virtual int getType() = 0;

    virtual DWORD from(const char* str) { return (DWORD)str; }
    virtual DWORD from(long val) { return (DWORD)val; }

    enum {
        INT,
        STRING,
        COLOR,
        TEXT,
        IMAGE,
        GROUP,
        FONT,
        RDR,
        ENUM,
    };
};

template<const int TYPE>
class TPropType : public PropType {
public:
    TPropType() { }

    virtual int getType() { return TYPE; }
};

class EnumType : public TPropType<PropType::ENUM> {
    map<string, int> valMaps;
    string name;
    EnumType() { }
public:

    static EnumType* create(const char* name, ...);

    virtual DWORD from(const char* str) {
        map<string, int>::iterator it = valMaps.find(str);
        if (it == valMaps.end()) {
            return (DWORD)(-1);
        }

        return it->second;
    }

    string getNameByValue(DWORD style) {
        DWORD dwMask = getMask();

        for(map<string, int>::iterator it = valMaps.begin(); it != valMaps.end(); ++it) {
            if (it->second == (dwMask & style)) {
                return it->first;
            }
        }
        return string();
    }

    DWORD getMask() {
        DWORD dwMask = 0;
        for(map<string, int>::iterator it = valMaps.begin(); it != valMaps.end(); ++it) {
            dwMask |= it->second;
        }
        return dwMask;
    }

};

template<const int Type>
class IntTypeBase : public TPropType<Type> {
public:
    IntTypeBase() { }

    virtual DWORD from(const char* str) {
        return (DWORD)strtol(str, NULL, 0);
    }
};


typedef IntTypeBase<PropType::INT> IntType;
typedef IntTypeBase<PropType::COLOR> ColorType;
typedef IntTypeBase<PropType::GROUP> GroupType;
typedef IntTypeBase<PropType::RDR> RdrType;

typedef TPropType<PropType::STRING> StringType;
typedef TPropType<PropType::TEXT> TextType;
typedef TPropType<PropType::IMAGE> ImageType;
typedef TPropType<PropType::FONT> FontType;

class Property;

struct PropValue {
    union {
        char *strVal;
        long iVal;
    }v;

    Property * prop;

    PropValue() {
        prop = NULL;
        v.iVal = 0;
    }
    PropValue(Property * p) {
        prop = p;
        v.iVal = 0;
    }

    PropValue(Property* p, long val) {
        setValue(p, val);
    }
    void setValue(Property* p, long val) {
        prop = p;
        v.iVal = val;
    }

    PropValue(Property* p, const char* strval) {
        setValue(p, strval);
    }

    void setValue(Property* p, const char* strval) {
        prop = p;
        v.strVal = strval ? strdup(strval) : NULL;
    }

    ~PropValue() {
        int vt = getValueType();
        if (vt == STR_VAL && v.strVal != NULL) {
            free(v.strVal);
        }
    }

    DWORD to() {
        PropType* type = getPropType();
        if (type == NULL) {
            return 0;
        }

        int vt = getValueType();
        if (vt == STR_VAL) {
            return type->from(v.strVal);
        }
        return type->from(v.iVal);
    }

private:
    enum {
        INT_VAL,
        STR_VAL
    };

    int getValueType() {
        PropType *type = getPropType();
        if (type == NULL) return INT_VAL;

        switch(type->getType()) {
        case PropType::STRING:
        case PropType::TEXT:
        case PropType::IMAGE:
        case PropType::FONT:
        case PropType::ENUM:
            return STR_VAL;
        }
        return INT_VAL;
    }

    PropType* getPropType();
};


struct Property {
    enum {
        READ = 1,
        WRITE = 2,
        RDWT = READ|WRITE
    };
    string name;
    PropType *type;
    int id;
    int access;
    PropValue* pDefValue;

    bool readable() { (access & READ) == READ; }
    bool writeable() { (access & WRITE) == WRITE; }

    Property(const char* name, PropType *type, int id, int access, PropValue* pdefVal = NULL) {
        this->name = name;
        this->type = type;
        this->id = id;
        this->access = access;
        pDefValue = pdefVal;
        if (pDefValue) {
            pDefValue->prop = this;
        }
    }

    template<typename T>
    void setDefValue(const T& val) {
        if (pDefValue == NULL) {
            pDefValue = new PropValue();
        }
        pDefValue->setValue(this, val);
    }

    bool hasDefValue() const { return pDefValue != NULL; }
    DWORD getDefValue() { return hasDefValue() ? pDefValue->to() : 0; }

    static Property* getProperty(const char* name);
};


class WidgetClassDefine {

    WidgetClassDefine * parent;
    mWidgetClass * ownerClass;
    string className;
    map<string, Property*> properties;
    map<string, PropValue*> defValues;

    void * glueObject;

public:
    WidgetClassDefine(const char* name, mWidgetClass* ownerClass) {
        className = name;
        this->ownerClass = ownerClass;
        glueObject = NULL;
        parent = getClassDefine((mWidgetClass*)(ownerClass->super));
        addClassDefine(this);
    }
    ~WidgetClassDefine() {
        //TODO
    }

    mWidgetClass * getOwnerClass() { return ownerClass; }

    const char* getClassName() {
        return className.c_str();
    }
    WidgetClassDefine * getParent() {
        return parent;
    }

    map<string, Property*>& getProperties() {
        return properties;
    }

    void addProperty(Property* prop) {
        properties[prop->name] = prop;
    }

    Property* getProperty(const char* name) {
        map<string, Property*>::iterator it = properties.find(name);
        if ( it == properties.end()) {
            if (getParent()) {
                return getParent()->getProperty(name);
            }
            return NULL;
        }
        return it->second;
    }

    template<typename T>
    void setDefPropValue(const char* name, const T& val) {
        Property * prop = getProperty(name);
        if (prop == NULL) {
            //TODO Error
            return ;
        }
        defValues[name] = new PropValue(prop, val);
    }

    void setDefPropValue(const char* name, PropValue * pdefVal) {
        Property * prop = getProperty(name);
        if (prop == NULL) {
            //TODO Error
            return ;
        }

        pdefVal->prop = prop;
        defValues[name] = pdefVal;
    }

    template<typename T>
    T* getGlueObject() {
        return (T*)glueObject;
    }

    template<typename T>
    void setGlueObject(T * obj) {
        glueObject = (void*)obj;
    }

    void initWndTemplateByDefaults(WndTemplateBuilder* pbuilder);

    static WidgetClassDefine * getClassDefine(mWidgetClass* ownerClass);
    static WidgetClassDefine * getClassDefine(const char* name);

private:
    static void addClassDefine(WidgetClassDefine* define);

    static map<mWidgetClass*, WidgetClassDefine*>* sWidgetMaps;
    static map<string, WidgetClassDefine*>* sNamedWidgetMaps;
};

enum {
    WIDGET_KEY = 0x7FAC2300,
    WIDGET_KEY_MAX
};


int GetEventIdByName(const char* name);

bool InitGlue();

void* GetWidgetEventHandlers(mWidget *widget);
void SetWidgetEventHandlers(mWidget* widget, void *pt);

unsigned long RunScript(const char* script, const char* filename);

}


#endif

