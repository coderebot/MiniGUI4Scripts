#ifndef MIUI_GET_ICUDATA_NAME
#define MIUI_GET_ICUDATA_NAME


#undef U_ICUDATA_NAME

#ifdef __cplusplus
#include <map>
#include <string>

namespace hybrid_j2v8 {
class IcuName {
    std::string name;
    typedef unsigned int HashKey;
    typedef std::map<HashKey, std::string> Map;
    typedef Map::iterator MapIterator;

    Map suffixes;

    static HashKey hash(const char* sufix);

    IcuName();

public:
    operator const char* () const {
        return name.c_str();
    }

    const char* operator + (const char* sufix) {

        if (sufix == NULL || sufix[0] == '\0') {
            return name.c_str();
        }

        HashKey key = hash(sufix);
        if (key == 0) {
            return *this;
        }

        std::string& find = suffixes[key];
        if (find.empty()) {
            find = name + sufix;
        }

        return find.c_str();

    }

    static IcuName& getInstance();

};

}

#define ICUDATA_EXTERN extern "C"

#define U_ICUDATA_NAME hybrid_j2v8::IcuName::getInstance() + ""

#else

#define ICUDATA_EXTERN extern

#define U_ICUDATA_NAME get_icudata_name("")

/*
 * if code like: uprv_strcmp(inBasename, U_ICUDATA_NAME".dat")
 * we should change it to U_ICUDATA_NAME_X(".dat")
 */
#define U_ICUDATA_NAME_X(x) get_icudata_name(x)
#endif

ICUDATA_EXTERN const char* get_icudata_name(const char* surfix);

#endif


#ifndef __cplusplus
#ifdef U_ICUDATA_LANG
#undef U_ICUDATA_LANG
#define U_ICUDATA_LANG  get_icudata_name(U_TREE_SEPARATOR_STRING "lang")
#endif

#ifdef U_ICUDATA_REGION
#undef U_ICUDATA_REGION
#define U_ICUDATA_REGION get_icudata_name(U_TREE_SEPARATOR_STRING "region")
#endif

#ifdef U_ICUDATA_CURR
#undef U_ICUDATA_CURR
#define U_ICUDATA_CURR get_icudata_name(U_TREE_SEPARATOR_STRING "curr")
#endif

#ifdef U_ICUDATA_ZONE
#undef U_ICUDATA_ZONE
#define U_ICUDATA_ZONE get_icudata_name(U_TREE_SEPARATOR_STRING "zone")
#endif

#ifdef U_ICUDATA_UNIT
#undef U_ICUDATA_UNIT
#define U_ICUDATA_UNIT get_icudata_name(U_TREE_SEPARATOR_STRING "unit")
#endif

#endif

