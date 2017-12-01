
from lxml import etree
import os
import sys
import re

def read_enum(e):
    enum = {}
    if 'name' in e.attrib:
        enum['#name'] = e.attrib['name']

    idx = 0
    for op in e:
        if not 'name' in op.attrib:
            continue
        name = op.attrib['name']
        if 'value' in op.attrib:
            value = eval(op.attrib['value'])
        else:
            value = idx
        idx = idx + 1
        enum[name] = value
    return enum

def read_property(e, types, lastid):
    a = e.attrib
    name = a['name']
    tp = a['type']
    pid = lastid + 1
    default = None
    if 'id' in a:
        pid = eval(a['id'])
    if 'default' in a:
        default = a['default']
    tps = tp.split(' ')
    if tps[0] == 'enum':
        if len(tps) > 1:
            tp_value = types[tps[1]]
        else:
            tp_value = read_enum(e.getchildren()[0])
    else:
        tp_value = tps[0]

    prop = {'name': name, 'id': pid, 'type':tp_value}
    if default:
        prop['default'] = default
    return prop

def read_types(path):
    types = {}
    d = etree.parse(path)
    root = d.getroot()
    if root.tag != 'compound-type':
        return types

    for e in root:
        if e.tag != 'enum':
            continue
        enum = read_enum(e)
        if enum and '#name' in enum:
            types[enum['#name']] = enum
    return types

def read_event(ev, lastid):
    event = {}
    pid = lastid + 1
    if 'id' in ev.attrib:
        pid = eval(ev.attrib['id'])

    event['id'] = pid

    for e in ev:
        if e.text:
            text = e.text.strip()
            if text and len(text) > 0:
                event[e.tag] = text
    return event


def readWidgets(widgets_dir):
    doms = []
    widgets = {}
    types = read_types(os.path.join(widgets_dir, 'compund-type.def'))

    for dirpath,dirnames,filenames in os.walk(widgets_dir):
        for fl in filenames:
            ext_name = os.path.splitext(fl)[1]
            if ext_name == '.xml':
                filepath = os.path.join(dirpath, fl)
                doms.append(etree.parse(filepath))

    while len(doms) > 0:
        for d in doms:
            root = d.getroot()
            if root.tag != 'window':
                doms.remove(d)
                continue

            if not 'class' in root.attrib:
                doms.remove(d)
                continue

            clazz = root.attrib['class']

            if clazz in widgets:
                doms.remove(d)
                continue

            extends=None

            if 'extends' in root.attrib:
                extends = root.attrib['extends']

            if extends and (not extends in widgets):
                continue

            widget = {'class': clazz }
            if extends:
                widget['extends'] = widgets[extends]

            readonlies = {}
            defaults = {}
            properties={}
            events = []
            lastid = 0
            for e in root:
                if e.tag == 'property':
                    prop = read_property(e, types, lastid)
                    properties[prop['name']] = prop
                    lastid = prop['id']
                elif e.tag == 'set-property':
                    name = e.attrib['name']
                    value = e.attrib['value']
                    readonlies[name] = value
                elif e.tag == 'default-property':
                    name = e.attrib['name']
                    value = e.attrib['value']
                    defaults[name] = value
                elif e.tag == 'event':
                    event = read_event(e, lastid)
                    events.append(event)
                    lastid = event['id']

            widget['readonly'] = readonlies
            widget['defaults'] = defaults
            widget['props'] = properties
            widget['events'] = events
            widgets[clazz] = widget

    return widgets,types

######################################################
PropDefineTemplate = '''
    propMaps["%(prop_name)s"] = new Property(
                "%(prop_name)s",
                %(prop_type)s,
                %(prop_id)d,
                %(prop_access)s);
'''


WidgetClassTemplate = '''\
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("%(className)s", (mWidgetClass*)(&(Class(m%(className)s))));
    widgetClasses.push_back(pwidget);
    %(prop_defines)s
}
'''

FrameworkTemplate = '''\
static void init_widget_class(vector<WidgetClassDefine*> & widgetClasses) {
    %(widget_classes)s
}
'''

PropFrameBase = '''
static void create_all_props(map<string, Property*> &propMaps) {
    %(prop_defines)s
}
'''

PropTypeFrameBase = '''\
static void create_enum_types(map<string, EnumType*>& enumMaps) {
    %(enum_types)s
}
'''

PropTypeTemplate = '''
    enumMaps["%(name)s"] = EnumType::create("%(name)s", %(options_list)s NULL);
'''

EventFrameBase = '''
static void init_event_ids(map<string, int>& eventMaps) {
    %(event_defines)s
}
'''
EventTemplate = '''
    eventMaps["%(handler)s"] = %(code)s;
'''

widget_excluede = set(['TextEditor', 'PhoneBar', 'IMWordSel'])

def getOptionList(enum):
    options = ''
    for opn, opv in enum.items():
        if opn != '#name':
            option = '"%s",0x%x'%(opn, opv)
            options = options + option + ','
    return options


def genEnumTypes(types):
    enum_types = ''
    for n,t in types.items():
        args = {'name':n}
        args['options_list'] = getOptionList(t)
        enum_types = enum_types + PropTypeTemplate % args
    return PropTypeFrameBase%{"enum_types":enum_types}


def add_widget(w, sorted_widgets, founded):
    if w['class'] in founded:
        return
    if 'extends' in w:
        add_widget(w['extends'], sorted_widgets, founded)
    sorted_widgets.append(w)
    founded[w['class']] = True

def sort_widgets(widgets):
    founded = {}
    sorted_widgets = []
    for n,w in widgets.items():
        if n in widget_excluede:
            continue
        add_widget(w, sorted_widgets, founded)

    return sorted_widgets


def dump_sorted_widgets(sorted_widgets):
    for w in sorted_widgets:
        print(w['class'])

def gen_events(widget):
    return ''

def gen_prop_access(widget, prop):
    name = prop['name']
    if name in widget['readonly']:
        return 'Property::READ'
    else:
        return 'Property::RDWT'

base_type_map = {
    'int' : 'PropType::INT',
    'string': 'PropType::STRING',
    'color': 'PropType::COLOR',
    'text': 'PropType::TEXT',
    'image': 'PropType::IMAGE',
    'group': 'PropType::GROUP',
    'renderer':'PropType::RDR',
    'font':'PropType::FONT'
}

def gen_prop_type(tp):
    if type(tp) == str:
        if tp in base_type_map:
            return 'getPropType(%s)'%base_type_map[tp]
        else:
            return "getPropType(-1)"
    elif type(tp) == dict:
        if '#name' in tp:
            return 'getNamedEnumType("%s")' % tp['#name']
        else:
            return 'EnumType::create(NULL, %s NULL)' % (getOptionList(tp))

def gen_prop(prop):
    args = {}
    args['prop_name'] = prop['name']
    args['prop_id'] = prop['id']
    args['prop_access'] = 'Property::RDWT' #gen_prop_access(widget, prop)
    args['prop_type'] = gen_prop_type(prop['type'])

    return PropDefineTemplate % args

def gen_widget_prop(prop, widget):
    return '\tpwidget->addProperty(getNamedProperty("%s"));\n' % (prop['name'])


def genAllProps(widgets):
    props = {}
    prop_strs = ''
    for w in widgets:
        for k, p in w['props'].items():
            if k not in props:
                props[k] = p
                prop_strs = prop_strs + gen_prop(p)
    return PropFrameBase % {'prop_defines': prop_strs}


def gen_event(handler, evt_code):
    return EventTemplate % {'handler':handler, 'code':evt_code}

def getHandler(handler):
    m = re.match("\w+\s+(\w+)\([^\)]*\)",handler)
    if m:
        return m.group(1)
    return handler


def genAllEvents(widgets):
    events = {}
    event_strs = ''
    for w in widgets:
        for evt in w['events']:
            handler = getHandler(evt['handler'])
            evt_code = evt['code']
            if handler in events:
                continue
            events[handler] = evt_code
            event_strs = event_strs + gen_event(handler, evt_code)
    return EventFrameBase % {'event_defines':event_strs}


def gen_props(widget):
    props_str = ''
    for k, prop in widget['props'].items():
        props_str = props_str + gen_widget_prop(prop, widget)
    return props_str


def genWidget(widget):
    args = {'className': widget['class']}
    args['prop_defines'] = gen_props(widget)
    args['event_defines'] = gen_events(widget)
    return WidgetClassTemplate % args

def genWidgets(widgets):
    codes = ''
    for w in widgets:
        codes = codes + genWidget(w)
    return FrameworkTemplate % {'widget_classes':codes}


def genCodes(widget_dirs, out_file):
    fout = open(out_file, "wt")
    widgets, types = readWidgets(widget_dirs)
    typecodes = genEnumTypes(types)
    fout.write(typecodes)
    sorted_widgets = sort_widgets(widgets)
    all_props = genAllProps(sorted_widgets)
    fout.write(all_props)
    all_events = genAllEvents(sorted_widgets)
    fout.write(all_events)
    #dump_sorted_widgets(sorted_widgets)
    widgets_code = genWidgets(sorted_widgets)
    fout.write(widgets_code)
    fout.close()



if __name__ == '__main__':
    genCodes(sys.argv[1], sys.argv[2])
