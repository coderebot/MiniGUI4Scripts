
MGNCS_PATH=$(LOCAL_PATH)/mgncs

mgncs_includes=$(MGNCS_PATH) \
			   $(MGNCS_PATH)/src \
			   $(MGNCS_PATH)/include \
			   $(MGNCS_PATH)/include/pieces \

mgncs_cflags=

mgncs_subdirs=pieces renderer datasource


mgncs_src_sources= mobject.c mcomponent.c mwidget.c mresmanager.c \
			mstatic.c mimage.c mrect.c mgroupbox.c mbuttongroup.c \
			mbutton.c mcheckbutton.c mradiobutton.c mimagebutton.c \
			mctrls.c mscroll_widget.c mpanel.c mprogressbar.c \
			mitem.c mitem_manager.c mitemview.c mlistbox.c \
			mcombobox.c mspinbox.c mspinner.c mslider.c \
			mtrackbar.c mcontainer.c mpage.c mpropsheet.c \
			mcommon.c mmainwnd.c mdialog.c minvisible-component.c \
			mem-slab.c mtimer.c medit.c msledit.c mscrollview.c \
			miconview.c mlistcolumn.c mlistitem.c mlistview.c \
			mmledit.c  mscrollbar.c  \
			mmonthcalendar.c \
			mpopmenumgr.c \
			mtoolitem.c \
			mtoolbar.c \
			mmenubutton.c \
			mledstatic.c \
			mcolorbutton.c \
			manimate.c manimateframes.c mgifanimateframes.c mbmparrayanimateframes.c \
			mseparator.c \
			mdatabinding.c \
			mdatasource.c \
			mtype.c \
			mime-wordsel.c \
			mnode.c mradionode.c mchecknode.c mabstractlist.c mlist.c mlist_layout.c \
			mrdr.c hashtable.c

#			mbtree.c mtextedit.c

mgncs_pieces_sources= mbuttonboxpiece.c \
		mcontainerpiece.c \
		mhotpiece.c \
		piece-event.c \
		mimagepiece.c \
		mlabelpiece.c \
		mrenderablepiece.c \
		mstaticpiece.c \
		mpairpiece.c \
		mabstractbuttonpiece.c \
		mlayoutpiece.c \
		piece.c \
		mpushbuttonpiece.c \
		mimagelabelpiece.c \
		msliderpiece.c \
		mthumbboxpiece.c \
		mtrackbarpiece.c \
		mcheckbuttonpiece.c \
		mcheckboxpiece.c \
		mradioboxpiece.c \
		mscrollbarpiece.c \
		marrowpiece.c \
		mleftarrowpiece.c \
		mrightarrowpiece.c \
		muparrowpiece.c \
		mdownarrowpiece.c \
		mboxlayoutpiece.c \
		mhboxlayoutpiece.c \
		mvboxlayoutpiece.c  \
		marrowbuttonpiece.c	\
		manimatepiece.c \
		mspinboxpiece.c \
		mspinnerpiece.c \
		mhspinboxpiece.c \
		mvspinboxpiece.c \
		mgridboxpiece.c \
		mdaygridpiece.c \
		mmonthpiece.c  \
		mnumspinnedpiece.c \
		mlistspinnedpiece.c \
		mweekheadpiece.c \
		mtoolimgpiece.c \
		mtoolimgitempiece.c \
		mseparatorpiece.c \
		mlinelayoutpiece.c \
		mwidgetwrapperpiece.c \
		mrectpiece.c \
		mprogresspiece.c \
		mscrollthumbboxpiece.c \
		msimplelistpiece.c     \
		mledstaticpiece.c 

mgncs_renderer_classic_sources= classic.c \
	classic_widget.c \
	classic_combobox.c \
	classic_groupbox.c \
	classic_propsheet.c \
	classic_listview.c \
	classic_boxpieces.c

mgncs_renderer_skin_sources=skin.c \
			skin_widget.c \
			skin_boxpieces.c \
			skin_combobox.c \
			skin_groupbox.c \
			skin_propsheet.c skin_listview.c 

mgncs_renderer_flat_sources=flat.c \
	flat_boxpieces.c \
	flat_widget.c \
	flat_combobox.c \
	flat_listview.c \
	flat_propsheet.c \
	flat_groupbox.c

mgncs_rdrs=classic flat skin

mgncs_renderer_sources=$(foreach rdr,$(mgncs_rdrs),$(addprefix $(rdr)/,$(mgncs_renderer_$(rdr)_sources)))

mgncs_datasource_sources=mstaticds.c comm_parser.c mtextds.c minids.c msqlds.c mxmlds.c xquery.c


mgncs_sources=$(addprefix mgncs/src/,$(mgncs_src_sources)) \
			  $(foreach subdir,$(mgncs_subdirs),$(addprefix mgncs/src/$(subdir)/,$(mgncs_$(subdir)_sources)))

