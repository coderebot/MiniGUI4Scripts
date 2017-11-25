
MINIGUI_PATH=$(LOCAL_PATH)/minigui

minigui_includes=$(MINIGUI_PATH) \
				 $(MINIGUI_PATH)/include \
				 $(MINIGUI_PATH)/src/include \
				 $(MINIGUI_PATH)/src/control \
				 $(MINIGUI_PATH)/src/newgal \
				 $(MINIGUI_PATH)/src/font \
				 $(MINIGUI_PATH)/src/sysres

minigui_cflags=-D__MINIGUI_LIB__ -D__ANDROID__


subdirs= misc main kernel sysres ial control ex_ctrl font gui mybmp newgal newgdi textedit

misc_sources= misc.c systext.c rwops.c endianrw.c nposix.c clipboard.c \
			  math.c about.c license.c \
			  error.c sockio.c listenfd.c

main_sources=main.c


kernel_sources= timer.c \
                init.c init-lite.c \
                desktop-ths.c desktop-sa.c desktop-procs.c \
                cursor.c cursor-procs.c cursor-sa.c \
                sharedres.c \
             	message.c event.c \
                blockheap.c fixstr.c slotset.c zorder.c

sysres_bmp_sources = skin_arrows_shell.bmp.c \
						   skin_pb_htruck.bmp.c \
						   skin_sb_vshaft.bmp.c \
						   classic_check_button.bmp.c \
						   skin_bborder.bmp.c \
					 	   skin_pb_vtrack.gif.c \
						   skin_sb_vthumb.bmp.c \
						   classic_radio_button.bmp.c \
						   skin_bkgnd.bmp.c \
				   		   skin_pb_vtruck.bmp.c \
						   skin_tab.gif.c \
						   flat_check_button.bmp.c \
					   	   skin_caption.gif.c \
					 	   skin_pushbtn.gif.c \
						   skin_tb_horz.gif.c \
						   flat_radio_button.bmp.c \
					   	   skin_checkbtn.bmp.c \
					  	   skin_radiobtn.gif.c \
						   skin_tborder.bmp.c \
						   skin_cpn_btn.gif.c \
					 	   skin_rborder.bmp.c \
						   skin_tbslider_h.gif.c \
		  				   skin_header.bmp.c \
						   skin_sb_arrows.bmp.c \
						   skin_tbslider_v.gif.c \
					 	   skin_lborder.bmp.c \
					 	   skin_sb_hshaft.bmp.c \
						   skin_tb_vert.gif.c \
						   skin_arrows.gif.c \
				 		   skin_pb_htrack.gif.c \
					   	   skin_sb_hthumb.bmp.c \
						   skin_tree.bmp.c

sysres_icon_sources = excalmatory-flat.ico.c \
						   	folder.ico.c \
							help-flat.ico.c \
				   			warning-flat.ico.c \
							excalmatory.ico.c \
					  		fold-flat.ico.c \
						   	help.ico.c \
					  		textfile-flat.ico.c \
						   	warning.ico.c \
							failed-flat.ico.c \
					  		fold.ico.c \
						   	textfile.ico.c \
							failed.ico.c \
				 			form-flat.ico.c \
						    unfold-flat.ico.c \
							folder-flat.ico.c \
					  		form.ico.c \
					        unfold.ico.c


sysres_sources= resource.c incoreres.c mgetc.c resmgr.c \
				cursor/cursors.c \
				$(addprefix bmp/,$(sysres_bmp_sources)) \
				$(addprefix icon/,$(sysres_icon_sources))


ial_sources=ial.c dummy.c

control_sources= ctrlmisc.c \
              static.c button.c edit.c listbox.c progressbar.c \
              menubutton.c trackbar.c combobox.c propsheet.c \
              newtoolbar.c scrolled.c scrollwnd.c scrollview.c \
			  textedit.c listmodel.c scrollbar.c bidiedit.c

ex_ctrl_sources= \
	treeview.c \
	treeview_rdr.c \
	monthcalendar.c \
    spinbox.c \
	coolbar.c \
	listview.c \
    gif89a.c \
	animation.c \
	iconview.c \
	gridview.c \
	gridviewcelltype.c


font_sources = charset.c charset-arabic.c charset-bidi.c \
            sysfont.c logfont.c devfont.c fontname.c \
			rawbitmap.c varbitmap.c qpf.c upf.c \
            fontcache.c freetype.c freetype2.c font-engines.c\
            gbunimap.c gbkunimap.c gb18030unimap.c big5unimap.c \
            ujisunimap.c sjisunimap.c euckrunimap.c \
            textops.c mapunitogb.c mapunitogbk.c mapunitobig5.c mapunitogb18030.c \
            bitmapfont.c scripteasy.c \
			in-core/rbf_fixedsys_8x15.c \
			in-core/rbf_terminal_8x12.c  \
			in-core/rbf_vgaoem_8x8.c      \
			in-core/vbf_courier_8x13.c    \
			in-core/vbf_sansserif_11x13.c \
			in-core/vbf_system_14x16.c \
 			in-core/upf_times_12x10.c \
			in-core/upf_times_17x14.c

gui_sources= accelkey.c caret.c ctrlclass.c dialog.c menu.c window.c \
    keyboard.c \
    defkeymap.c fr-pc-kmap.c fr-kmap.c de-kmap.c de-latin1-kmap.c \
	hebrewkeymap.c arabickeymap.c \
    it-kmap.c es-kmap.c es-cp850-kmap.c \
	lf_manager.c lf_classic.c lf_flat.c lf_skin.c lf_common.c

mybmp_sources= mybmp.c winbmp.c gif.c jpeg.c png.c lbm.c pcx.c tga.c 

textedit_sources=object.c mbtree.c mtextedit.c mwidget.c

newgal_sources=\
    RLEaccel.c      \
    blit.c          \
    blit_0.c        \
    blit_1.c        \
    blit_A.c        \
    blit_N.c        \
    pixels.c        \
    surface.c       \
    stretch.c       \
    video.c         \
    newgal.c		\
	videomem-bucket.c \
	dummy/nullvideo.c


newgdi_sources=gdi.c attr.c clip.c map.c coor.c rect.c  \
              palette.c readbmp.c icon.c screen.c bitmap.c \
              pixel.c line.c arc.c pixel_ops.c \
              region.c generators.c polygon.c flood.c \
              advapi.c midash.c mispans.c miwideline.c \
              mifillarc.c mifpolycon.c miarc.c rotatebmp.c \
			  text.c glyph.c bidi.c \
			  textout.c tabbedtextout.c drawtext.c 

minigui_sources=$(foreach subdir,$(subdirs),$(addprefix minigui/src/$(subdir)/,$($(subdir)_sources)))

