MGUTILS_PATH=$(LOCAL_PATH)/mgutils

mgutils_includes=$(MGUTILS_PATH) $(MGUTILS_PATH)/include

mgutils_cflags=

mgutils_subdirs=mywins skin templates

mgutils_mywins_sources=mywindows.c helpwin.c colordlg.c colorspace.c newfiledlg.c

mgutils_skin_sources=skin.c  \
	cmdbutton.c  chkbutton.c  label.c    bmplabel.c  slider.c   rotslider.c   mgcontrol.c

mgutils_vcongui_sources=child.c error.c       \
    key.c paint.c terminal.c       \
    vc.c vcongui.c vt.c defkeymap.c

mgutils_templates_sources=commdlg.c filedlg.c colordlg.c fontdlg.c infodlg.c colorpanel.c

mgutils_sources=$(foreach subdir, $(mgutils_subdirs),$(addprefix mgutils/src/$(subdir)/,$(mgutils_$(subdir)_sources)))

