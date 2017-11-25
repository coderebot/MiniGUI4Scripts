#!/usr/bin/python

import os
import sys
import re

#print "Usage %s v8-<version>" % sys.argv[0]

v8_version_str = sys.argv[1]
v8_version = float(v8_version_str)
out_path = sys.argv[2]
source_mk = '%s/v8src-%s.mk' % (out_path, v8_version_str)

v8_src="%s/v8-%s" % (out_path, v8_version_str)

v8_tar_files = {
    # to add the new tars file
    5.0 : 'v8.git-5.0-lkgr.tar.gz',
}

def unpack_v8_source():
    os.system("mkdir -p %(v8_src)s && tar -zxf %(v8_tar_file)s -C %(v8_src)s"
            % {'v8_src':v8_src, 'v8_tar_file':"%s/v8-tars/%s"%(out_path, v8_tar_files[v8_version])})

if not os.path.exists(v8_src):
    #unpack the source
    unpack_v8_source()


if v8_version >= 5.2:
    v8_gyp = "%s/src/v8.gyp" % v8_src
    d8_gyp = "%s/src/d8.gyp" % v8_src
    v8_src_trans = lambda x : 'src/' + x
else:
    v8_gyp = "%s/tools/gyp/v8.gyp" % v8_src
    d8_gyp = "%s/src/d8.gyp" % v8_src
    v8_src_trans = lambda x : x.replace('../../', '')

arch_list = ["arm", "arm64", "ia32", "x87", "mips", "mipsel", "mips64el", "x64", "x32"]

def dump(prefix, x):
    if type(x) == dict:
        for k,v in x.items():
            print(prefix, k, '{')
            dump(prefix + '\t', v)
            print(prefix, '}');
    elif type(x) == list:
        for v in x:
            print(prefix, '[');
            dump(prefix + '\t',v)
            print(prefix, ']');
    else:
        print(prefix, x)


def read_gyp(gyp):
    content = open(gyp).read()
    return eval(content)


def find_target(gyp_list, target_name):
    targets = gyp_list['targets']
    for target in targets:
        if target['target_name'] == target_name:
            return target
    print("cannot file the target:%s" % target_name)
    return None


# target_info = {
#   'target':<target_name>'
#   'sources':[names]
#   'env': { ... }
#   'src_trans': lambda
# }
def process_target_element(ele, target_info, env, source_list, key_name):
    if type(ele) == dict:
        for k,v in ele.items():
            if k == 'conditions':
                for arch in arch_list:
                    new_env = dict(env, **{'v8_target_arch':arch})
                    for c in v:
                        try:
                            if eval(c[0], new_env):
                                new_key_name = key_name + '_' + arch
                                process_target_element(c[1], target_info, env, source_list, new_key_name)
                        except:
                            pass

            elif k in target_info['sources'] and type(v) == list:
                #get data
                key = key_name
                if target_info.has_key('source_names') and target_info['source_names'].has_key(k):
                    key = key + '_' + target_info['source_names'][k]
                if source_list.has_key(key):
                    source_list[key] = source_list[key] + v
                else:
                    source_list[key] = [] + v
            else:
                process_target_element(v, target_info, env, source_list, key_name)

defualt_env = {
            'want_separate_host_toolset':0,
            'OS':'linux',
            'v8_enable_i18n_support':1,
            'cu_use_data_file_flag':0}

def find_sources(gyp_list, target_info):
    tmp_target = target_info['target']
    if type(tmp_target) == list:
        targets = tmp_target
    else:
        targets = [tmp_target]
    
    source_mk = ''
    for target in targets:
        target_list = find_target(gyp_list, target)
        source_list = { }
        env = defualt_env.copy()
        if target_info.has_key('env'):
            env = dict(env, **target_info['env'])
        process_target_element(target_list, target_info, env, source_list, target)

        for name, sources in source_list.items():
            src_mk = '%s_src =' % name
            src_trans = target_info['src_trans'] if target_info.has_key('src_trans') else lambda x : x
            for source in sources:
                if not target_info.has_key('accept') or \
                    (target_info.has_key('accept') and target_info['accept'](source)):
                    src_mk = src_mk + ' \\\n\t' + src_trans(source)
            source_mk = source_mk + '\n\n' + src_mk
    return source_mk

def accept_c_source(x):
    return os.path.splitext(x)[1][1:] in ['c', 'cc', 'cpp']

v8_c_target_file = {
    'gyp' : v8_gyp,
    'target' : ['v8_base', 'v8_libbase'],
    'sources': ['sources'],
    'src_trans': v8_src_trans,
    'accept':accept_c_source
}

v8_js_target_file = {
    'gyp' : v8_gyp,
    'target' : 'js2c',
    'sources' : ['library_files','experimental_library_files', 'i18n_library_files', 'code_stub_library_files'],
    'src_trans': v8_src_trans,
    'source_names':{'experimental_library_files':'experimental', 'code_stub_library_files':'code_stub'},
}

v8_nosnapshot_target_file = {
    'gyp' : v8_gyp,
    'target' : 'v8_nosnapshot',
    'sources' : ['sources'],
    'src_trans': v8_src_trans,
    'accept':lambda x: False if x.find('SHARED_INTERMEDIATE_DIR') >= 0 else True
}

d8_c_target_file = {
    'gyp' : d8_gyp,
    'target' : 'd8',
    'sources': ['sources'],
    'src_trans': lambda x : 'src/' + x,
    'accept':accept_c_source
}

d8_js_target_file = {
    'gyp' : d8_gyp,
    'target' : 'd8_js2c',
    'sources': ['js_files'],
    'src_trans' : lambda x : 'src/' + x,
}

platform_target_file = {
    'gyp' : v8_gyp,
    'target' : 'v8_libplatform',
    'sources' : ['sources'],
    'src_trans': v8_src_trans,
    'accept':accept_c_source
}

sampler_target_file = {
    'gyp' : v8_gyp,
    'target' : 'v8_libsampler',
    'sources' : ['sources'],
    'src_trans': v8_src_trans,
    'accept':accept_c_source
}

mkpeephole_target_file = {
    'gyp' : v8_gyp,
    'target' : 'mkpeephole',
    'sources' : ['sources'],
    'src_trans' : v8_src_trans,
    'accept' : accept_c_source
}


def generate_source_mk(target_infos):
    source_mk = ''
    for target_info in target_infos:
        source_mk = source_mk + '\n\n' + find_sources(read_gyp(target_info['gyp']), target_info)
    return source_mk


v8_target_files = []
v8_target_files.append(v8_c_target_file)
v8_target_files.append(v8_js_target_file)
v8_target_files.append(d8_c_target_file)
v8_target_files.append(d8_js_target_file)
v8_target_files.append(v8_nosnapshot_target_file)
v8_target_files.append(platform_target_file)

if v8_version >= 5.5:
    v8_target_files.append(sampler_target_file)
    v8_target_files.append(mkpeephole_target_file)

#print(generate_source_mk(v8_target_files))
open(source_mk, 'wt').write(generate_source_mk(v8_target_files))
