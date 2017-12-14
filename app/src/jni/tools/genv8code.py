import os,sys
import json
import re

#usage genv8code path out_file.cpp

widget_json = sys.argv[1]
out_file = sys.argv[2]

WIDIGET_TEMPLS='''
#define DEF_CONST(V) \\
%(consts_define)s

%(classes_implement)s

%(classes_register)s

'''
REGISTER_CLASS_TEMPLS='''
BEGIN_REGISTER_CLASS(%(classname)s)
%(method_registers)s
END_REGISTER_CLASS(%(classname)s)
'''

IMPLEMENT_CLASS_METHOD_TMPLS='''
BEGIN_IMPLEMENT_CLASS_METHOD(%(classname)s, %(methodname)s)
%(paramters_define)s
    %(return_define)s
    %(implements)s
    %(set_return)s
END_IMPLEMENT_CLASS_METHOD
'''
METHOD_PARAMTER_TMPL='''
    DECLARE_PARAMTER(%(type)s, %(index)s, %(name)s, %(default)s)
'''

def find_quote(s, pos):
    while True:
        idx = s.find('"', pos)
        if idx < 0:
            return -1;
        elif idx > 0:
                i = idx - 1
                while i > 0 and s[i] == '\\':
                    i = i - 1
                if (idx - i) % 2 == 1:
                    return idx
        else:
            return idx
        pos = idx + 1
    return -1


def get_quote_count(s):
    pos = 0
    count = 0
    while pos < len(s):
        pos = find_quote(s, pos)
        if pos < 0:
            break
        pos = pos + 1
        count = count + 1
    return count


def read_and_trans(file_name):
    f = open(file_name, "rt")
    s = ''
    tp = 0
    while True:
        line = f.readline()
        if not line: break
        if tp == 0:
            count = get_quote_count(line)
            if count > 0 and count % 2 == 1:
                tp = 1
        elif tp == 1:
            if find_quote(line, 0) > 0:
                tp = 0

        if tp == 1:
            s = s + line[:-1] + '\\n'
        else:
            s = s + line

    f.close()
    return s

def read_json(json_file):
    s = read_and_trans(json_file)
    return json.loads(s)

def genConst(const):
    if type(const) == unicode:
        return "\tV(\"%s\", %s) \\\n" % (const, const)
    elif type(const) == dict and "name" in const and "value" in const:
        return "\tV(\"%s\", %s) \\\n" % (const['name'], const['value'])
    else:
        return "\t\\\n"


def genConsts(consts):
    ret = ''
    for const in consts:
       ret = ret + genConst(const)
    return ret

def genMethodRegister(method):
    return '\tREGISTER_METHOD(%(name)s)\n' % method

def genClassRegister(clazz):
    if not 'methods' in clazz:
        return ''
    args = { }
    args['classname'] = clazz['name']
    meth_str = ''
    for method in clazz['methods']:
        meth_str = meth_str + genMethodRegister(method)
    args['method_registers'] = meth_str
    return REGISTER_CLASS_TEMPLS % args

def genClassesRegister(classes):
    ret = 'BEGIN_REGISTER_CLASSES\n'
    for clazz in classes:
        ret = ret + genClassRegister(clazz)
    return ret + "\nEND_REGISTER_CLASSES\n"

type_default_maps = {
    'int': '0',
    'float': '0.0',
    'double': '0.0',
    'string':'""',
    'long':'0',
    'bool':'false',
    'object':'(ObjectWrap*)NULL'
}

def getTypeDefault(type_str):
    return type_default_maps[type_str]


def genMethodParamters(paramters):
    index = 0
    paramter_str = ''
    for p in paramters:
        args = { }
        args['type'] = p['type'] == 'object' and 'ObjectWrap' or p['type']
        args['name'] = p['name']
        args['index'] = str(index)
        index = index + 1
        if 'default' in p:
            args['default'] = p['default']
        else:
            args['default'] = getTypeDefault(p['type'])
        paramter_str = paramter_str + METHOD_PARAMTER_TMPL % args
    return paramter_str

def genMethodReturn(ret_type):
    return "DECLARE_RETURN(%s)" % ret_type

def genMethodSetReturn(ret_type):
    return "SET_RETURN(%s)" % ret_type

def genMethodImplement(method, classname):
    args = {'classname':classname, 'methodname':method['name'], 'paramters':'', 'return_define':'', 'set_return':'', 'implements':'' }
    if 'paramters' in method:
        args['paramters_define'] = genMethodParamters(method['paramters'])
    if 'return' in method and method['return'] != 'void':
        args['return_define'] = genMethodReturn(method['return'])
        args['set_return'] = genMethodSetReturn(method['return'])
    args['implements'] = method['implements']
    return IMPLEMENT_CLASS_METHOD_TMPLS % args


def genClassImplement(clazz):
    if not 'methods' in clazz:
        return ''
    args = { }
    meth_str = ''
    for method in clazz['methods']:
        meth_str = meth_str + genMethodImplement(method, clazz['name'])
    return meth_str


def genClassesImplement(classes):
    ret = ''
    for clazz in classes:
        ret = ret + genClassImplement(clazz)
    return ret

def gen_widgets(widgets):
    args = {'consts_define':'', 'classes_register':'', 'classes_implement':'' }
    if 'consts' in widgets:
        args['consts_define'] = genConsts(widgets['consts'])
    if 'classes' in widgets:
        args['classes_register'] = genClassesRegister(widgets['classes'])
        args['classes_implement'] = genClassesImplement(widgets['classes'])
    return WIDIGET_TEMPLS % args

result = gen_widgets(read_json(widget_json))

#print result
fout = open(out_file, "wt")
fout.write(result)
fout.close()

