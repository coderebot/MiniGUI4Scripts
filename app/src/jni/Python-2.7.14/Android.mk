
LOCAL_PATH:=$(call my-dir)

include $(CLEAR_VARS)

PSRCS=		\
		Parser/acceler.c \
		Parser/grammar1.c \
		Parser/listnode.c \
		Parser/node.c \
		Parser/parser.c \
		Parser/parsetok.c \
		Parser/bitset.c \
		Parser/metagrammar.c \
		Parser/firstsets.c \
		Parser/grammar.c \
		Parser/pgen.c

PGSRCS=		\
		Objects/obmalloc.c \
		Python/mysnprintf.c \
		Python/pyctype.c \
		Parser/tokenizer_pgen.c \
		Parser/printgrammar.c \
		Parser/pgenmain.c

DYNLOADFILE:=dynload_shlib.c
THREADOBJ:=Python/thread.c

##########################################################################
PYTHON_OBJS=	\
		Python/_warnings.c \
		Python/Python-ast.c \
		Python/asdl.c \
		Python/ast.c \
		Python/bltinmodule.c \
		Python/ceval.c \
		Python/compile.c \
		Python/codecs.c \
		Python/errors.c \
		Python/frozen.c \
		Python/frozenmain.c \
		Python/future.c \
		Python/getargs.c \
		Python/getcompiler.c \
		Python/getcopyright.c \
		Python/getplatform.c \
		Python/getversion.c \
		Python/graminit.c \
		Python/import.c \
		Python/importdl.c \
		Python/marshal.c \
		Python/modsupport.c \
		Python/mystrtoul.c \
		Python/mysnprintf.c \
		Python/peephole.c \
		Python/pyarena.c \
		Python/pyctype.c \
		Python/pyfpe.c \
		Python/pymath.c \
		Python/pystate.c \
		Python/pythonrun.c \
        Python/random.c \
		Python/structmember.c \
		Python/symtable.c \
		Python/sysmodule.c \
		Python/traceback.c \
		Python/getopt.c \
		Python/pystrcmp.c \
		Python/pystrtod.c \
		Python/dtoa.c \
		Python/formatter_unicode.c \
		Python/formatter_string.c \
		Python/$(DYNLOADFILE) \
		$(LIBOBJS) \
		$(MACHDEP_OBJS) \
		$(THREADOBJ)


##########################################################################
# Objects
UNICODE_OBJS:=Objects/unicodeobject.c Objects/unicodectype.c
OBJECT_OBJS=	\
		Objects/abstract.c \
		Objects/boolobject.c \
		Objects/bufferobject.c \
		Objects/bytes_methods.c \
		Objects/bytearrayobject.c \
		Objects/capsule.c \
		Objects/cellobject.c \
		Objects/classobject.c \
		Objects/cobject.c \
		Objects/codeobject.c \
		Objects/complexobject.c \
		Objects/descrobject.c \
		Objects/enumobject.c \
		Objects/exceptions.c \
		Objects/genobject.c \
		Objects/fileobject.c \
		Objects/floatobject.c \
		Objects/frameobject.c \
		Objects/funcobject.c \
		Objects/intobject.c \
		Objects/iterobject.c \
		Objects/listobject.c \
		Objects/longobject.c \
		Objects/dictobject.c \
		Objects/memoryobject.c \
		Objects/methodobject.c \
		Objects/moduleobject.c \
		Objects/object.c \
		Objects/obmalloc.c \
		Objects/rangeobject.c \
		Objects/setobject.c \
		Objects/sliceobject.c \
		Objects/stringobject.c \
		Objects/structseq.c \
		Objects/tupleobject.c \
		Objects/typeobject.c \
		Objects/weakrefobject.c \
		$(UNICODE_OBJS)




LOCAL_MODULE:=libpython_static

LOCAL_SRC_FILES:= $(PSRCS) $(PGSRCS) $(PYTHON_OBJS) $(OBJECT_OBJS)

LOCAL_C_INCLUDES:=$(LOCAL_PATH) $(LOCAL_PATH)/Include

include $(BUILD_STATIC_LIBRARY)
