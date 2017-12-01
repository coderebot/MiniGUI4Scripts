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

namespace glue {

class PropType {
public:
    PropType() { }
    virtual ~PropType() { }

    virtual int getType() = 0;

    virtual DWORD from(const char* str) { return (DWORD)str; }
    virtual DWORD from(int val) { return (DWORD)val; }

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
    //PropValue *defValue;

    bool readable() { (access & READ) == READ; }
    bool writeable() { (access & WRITE) == WRITE; }

    Property(const char* name, PropType *type, int id, int access) {
        this->name = name;
        this->type = type;
        this->id = id;
        this->access = access;
    }

    static Property* getProperty(const char* name);
};

class WidgetClassDefine {

    mWidgetClass * ownerClass;
    string className;
    vector<Property*> properties;

    void * glueObject;

public:
    WidgetClassDefine(const char* name, mWidgetClass* ownerClass) {
        className = name;
        this->ownerClass = ownerClass;
        glueObject = NULL;
    }
    ~WidgetClassDefine() {
    }

    mWidgetClass * getOwnerClass() { return ownerClass; }

    const char* getClassName() {
        return className.c_str();
    }
    WidgetClassDefine * getParent() {
        return NULL;
    }

    vector<Property*>& getProperties() {
        return properties;
    }

    void addProperty(Property* prop) {
        properties.push_back(prop);
    }

    template<typename T>
    T* getGlueObject() {
        return (T*)glueObject;
    }

    template<typename T>
    void setGlueObject(T * obj) {
        glueObject = (void*)obj;
    }

    static WidgetClassDefine * getClassDefine(mWidgetClass* ownerClass);

private:
    static void addClassDefine(WidgetClassDefine* define);

    static map<mWidgetClass*, WidgetClassDefine*> widgetMaps;
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

