
struct ObjectWrap {
    lua_State* L;
    int idx;
public:
    ObjectWrap() : L(NULL), idx(0) { }
    ~ObjectWrap() {
        if (idx > 0) {
            luaL_unref(L, LUA_REGISTRYINDEX, idx);
        }
    }

    void set(lua_State* L, int i) {
        this->L = L;
        lua_pushvalue(L, i);
        idx = luaL_ref(L, LUA_REGISTRYINDEX);
    }

    template<typename T>
    vector<T> toArray() {
        if (idx <= 0 || lua_istable(L, idx)) {
            return vector<T>();
        }
        vector<T> vec;
        lua_rawgeti(L, LUA_REGISTRYINDEX, idx);
        int val_idx = lua_gettop(L);
        int count = lua_objlen(L, val_idx);
        for (int i = 0; i < count; i ++) {
            T t;
            lua_rawgeti(L, val_idx, i+1);
            toValue(t, L, -1);
            vec.push_back(t);
        }
        return vec;
    }
};

static int pushvalue(lua_State* L, ObjectWrap& obj) {
    lua_pushvalue(L, obj.idx);
    return 1;
}

static void toValue(ObjectWrap& w, lua_State* L, int idx) {
    w.set(L, idx);
}

static int fromPropValue(lua_State* L, PropType* prop_type, PValue &val) {
    switch(prop_type->getType()) {
    case PropType::INT:
    case PropType::GROUP:
        return pushvalue(L, val.d.lval);
    case PropType::STRING:
    case PropType::TEXT:
    case PropType::IMAGE:
    case PropType::FONT:
        return pushvalue(L, val.d.strval);
    case PropType::ENUM: {
        string enum_value = ((EnumType*)prop_type)->getNameByValue((DWORD)val.d.uval);
        return pushvalue(L, enum_value);
    }
    case PropType::COLOR:
        return pushvalue(L, (double)val.d.uval);
    }

    lua_pushnil(L);
    return 1;
}


class MethodArgWrap {
    lua_State *L;
    int count;
    int objidx;

    int get(int index, const char* name) {
        if (objidx > 0) {
            lua_pushstring(L, name);
            lua_gettable(L, objidx);
            return -1;
        }

        if (index < count) {
            return index + 2;
        }
        return 0;
    }

public:
    MethodArgWrap(lua_State* L) {
        this->L = L;
        objidx = 0;
        count = lua_gettop(L) - 1;
        if (count > 0 && lua_istable(L, 2)) {
            objidx = 2;
        }
    }

    template<typename T>
    void getArg(int index, const char* name, T& val, const T& def) {
        int ret = get(index, name);
        if (ret == 0) {
            val = def;
        } else {
            toValue(val, L, ret);
            if (ret == -1) {
                lua_pop(L, 1);
            }
        }
    }
    void getArg(int index, const char* name, string& val, const char* def) {
        getArg(index, name, val, string(def));
    }
    void getArg(int index, const char* name, ObjectWrap& value, const ObjectWrap* pdef) {
        getArg(index, name, value, ObjectWrap());
    }
};


////////////////////////////////////////////////
//include the methods
typedef map<string, lua_CFunction> ClassMethodMap;
typedef map<mWidgetClass*, ClassMethodMap> WidgetMethodMap;

#define BEGIN_REGISTER_CLASSES \
static void register_widget_classes_methods(WidgetMethodMap& maps) {

#define END_REGISTER_CLASSES }

#define BEGIN_REGISTER_CLASS(name) \
{ \
    namespace x = name; \
    ClassMethodMap & class_methods = maps[(mWidgetClass*)(&Class(m##name))];

#define REGISTER_METHOD(method_name) \
    class_methods[#method_name] = (lua_CFunction)x::mg_##method_name;

#define END_REGISTER_CLASS(name) }

#define BEGIN_IMPLEMENT_CLASS_METHOD(classname, methodname) \
namespace classname { \
static int mg_##methodname(lua_State* L) { \
    MethodArgWrap arg_wrap(L); \
    m##classname *self = (m##classname*) getWrapWidget(L, 1); \
    int ret_count = 0;

#define END_IMPLEMENT_CLASS_METHOD \
    return ret_count; \
}}

#define DECLARE_RETURN(ret_type) ret_type ret;

#define SET_RETURN(ret_type) ret_count = pushvalue(L, ret);

#define DECLARE_PARAMTER(Type, index, name, defval) \
    Type name; arg_wrap.getArg(index, #name, name, defval);


#include "widget_methods_define.cpp"

static WidgetMethodMap  *gWidgetMethodEntries;

static void init_widget_classes_methods() {
    if (gWidgetMethodEntries == NULL) {
        gWidgetMethodEntries = new WidgetMethodMap;
        register_widget_classes_methods(*gWidgetMethodEntries);
    }
}
static ClassMethodMap* get_widget_method_entries(mWidgetClass* widget_class) {
    init_widget_classes_methods();
    if (gWidgetMethodEntries == NULL) {
        return NULL;
    }

    WidgetMethodMap::iterator it = gWidgetMethodEntries->find(widget_class);
    if (it == gWidgetMethodEntries->end()) {
        return NULL;
    }

    return &it->second;
}

//////////////////////////////////////////////

template<class T>
T* getUpvalue(lua_State* L, int idx) {
    T * p = NULL;
    int val_idx = lua_upvalueindex(idx);
    if (lua_islightuserdata(L, val_idx)) {
        p = (T*)lua_touserdata(L, val_idx);
    }

    return p;
}

static int get_index(lua_State* L, mWidget* widget, const string& key, WidgetClassDefine* widget_define, ClassMethodMap* method_map) {

    map<string,Property*> &props = widget_define->getProperties();
    map<string,Property*>::iterator propit = props.find(key);
    if (propit != props.end()) {
        Property* prop = propit->second;

        PValue val;
        if (getWidgetProperty(widget, prop, val)) {
            return fromPropValue(L, prop->type, val);
        } else {
            lua_pushnil(L);
            return 1;
        }
    }

    //try get the function
    if (method_map) {
        ClassMethodMap::iterator mthid = method_map->find(key);
        if (mthid != method_map->end()) {
            lua_pushcfunction(L, mthid->second);
            return 1;
        }
    }

    WidgetClassDefine* parent = widget_define->getParent();
    if (parent) {
        ClassMethodMap* parent_method_map =
            get_widget_method_entries(parent->getOwnerClass());

        return get_index(L, widget, key, parent, parent_method_map);
    }

    lua_pushnil(L);
    return 1;
}

static int lua_widget_get_index(lua_State* L) {
    mWidget * widget = getWrapWidget(L, 1);
    if (!widget) {
        lua_pushnil(L);
        return 1;
    }

    if (!lua_isstring(L, 2)) {
        lua_pushnil(L);
        return 1;
    }

    const char* key = lua_tostring(L, 2);

    WidgetClassDefine* widget_define = getUpvalue<WidgetClassDefine>(L, 1);
    ClassMethodMap* method_map = getUpvalue<ClassMethodMap>(L, 2);

    return get_index(L, widget, key, widget_define, method_map);
}

static void set_widget_index(lua_State* L, mWidget* widget, WidgetClassDefine* widget_define, const string& key, int val_idx) {

    map<string,Property*> &props = widget_define->getProperties();
    map<string,Property*>::iterator propit = props.find(key);
    if (propit != props.end()) {
        Property* prop = propit->second;

        DWORD dwVal = toPropValue(prop->type, L, val_idx);

        setWidgetProperty(widget, prop, dwVal);
        return ;
    }

    WidgetClassDefine* parent = widget_define->getParent();
    if (parent) {
        set_widget_index(L, widget, parent, key, val_idx);
    }
}

static int lua_widget_new_index(lua_State* L) {
    mWidget * widget = getWrapWidget(L, 1);
    if (!widget) {
        return 0;
    }

    if (!lua_isstring(L, 2)) {
        return 0;
    }

    ThreadLocalHolder localHolder;
    const char* key = lua_tostring(L, 2);

    WidgetClassDefine* widget_define = getUpvalue<WidgetClassDefine>(L, 1);

    set_widget_index(L, widget, widget_define, key, 3);

    return 0;
}


static int createWidgetClass(lua_State*L, WidgetClassDefine* widget) {
    lua_newtable(L);
    int table = lua_gettop(L);

    lua_pushliteral(L, "__index");
    lua_pushlightuserdata(L, widget);
    lua_pushlightuserdata(L, get_widget_method_entries(widget->getOwnerClass()));
    lua_pushcclosure(L, lua_widget_get_index, 2);
    lua_rawset(L, table);

    lua_pushliteral(L, "__newindex");
    lua_pushlightuserdata(L, widget);
    lua_pushcclosure(L, lua_widget_new_index, 1);
    lua_rawset(L, table);

    //mg[1][widget] = table
    lua_getglobal(L, "mg");
    lua_rawgeti(L, -1, 1);
    lua_pushlightuserdata(L, widget->getOwnerClass());
    lua_pushvalue(L, table);
    lua_rawset(L, -3);

    lua_pop(L, lua_gettop(L) - table);

    return 1;
}

static int getAndInstallWidgetClass(lua_State* L, mWidgetClass* widget_class) {
    //return mg[1][widget_class]
    lua_getglobal(L, "mg");
    lua_rawgeti(L, -1, 1);
    if (!lua_istable(L, -1)) {
        lua_pop(L, 3); //pop value, mg[1], mg
        return createWidgetClass(L, WidgetClassDefine::getClassDefine(widget_class));
    }

    lua_pushlightuserdata(L, widget_class);
    lua_rawget(L, -2);
    if (!lua_istable(L, -1)) {
        lua_pop(L, 3); //pop value, mg[1], mg
        return createWidgetClass(L, WidgetClassDefine::getClassDefine(widget_class));
    }

    return 1;
}


static int wrap_widget_object(lua_State* L, mWidget* widget) {
    int ret = getAndInstallWidgetClass(L, _c(widget));
    if (ret <= 0) return 0;

    int metatable = lua_gettop(L);

    mWidget** pwidget = (mWidget**)lua_newuserdata(L, sizeof(mWidget*));
    *pwidget = widget;

    lua_pushvalue(L, metatable);
    lua_setmetatable(L, -2);

    return 1;
}

