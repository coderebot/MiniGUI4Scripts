/*************************************************
* This is inside mode of system res configuation *
* It's generated by the mgcfg-trans, version 1.0 *
* author : dongjunjie in feynman                 *
* please donnot modify this file, if you want,   *
* please change your input file and regenerate   *
* this file                                      *
*************************************************/
#include <stdio.h>
#include "common.h"
#include "minigui.h"

#ifdef _MGINCORE_RES

// Section: classic
static char* _classic_keys[]={ 
    "fgc_window",
    "bgc_window",
    "fgc_3dbody",
    "bgc_3dbody",
    "fgc_selected_item",
    "bgc_selected_item",
    "bgc_selected_nofocus",
    "fgc_disabled_item",
    "bgc_disabled_item",
    "fgc_hilight_item",
    "bgc_hilight_item",
    "mode_bkimage"
};
static char* _classic_values[]={ 
    "0xFF000000",
    "0xFFFFFFFF",
    "0xFF000000",
    "0xFFCED3D6",
    "0xFFFFFFFF",
    "0xFF6B2408",
    "0xFFBDA69C",
    "0xFF848284",
    "0xFFCED3D6",
    "0xFFFFFFFF",
    "0xFF6B2408",
    "0"
};
// Section: fashion
static char* _fashion_keys[]={ 
    "metrics_3dbody_roundx",
    "metrics_3dbody_roundy",
    "mode_bgc",
    "fgc_3dbody",
    "bgc_3dbody",
    "mode_useflat",
    "fgc_window",
    "bgc_window",
    "mode_item",
    "fgc_selected_item",
    "bgc_selected_item",
    "bgc_selected_nofocus",
    "fgc_disabled_item",
    "bgc_disabled_item",
    "fgc_hilight_item",
    "bgc_hilight_item"
};
static char* _fashion_values[]={ 
    "3",
    "3",
    "0",
    "0xFF000000",
    "0xFFFF3000",
    "0",
    "0xFF000000",
    "0xFFFFFFFF",
    "1",
    "0xFFFFFFFF",
    "0xFFF54512",
    "0xFFBDA69C",
    "0xFF99A8AC",
    "0xFFFFFFFF",
    "0xFFFFFFFF",
    "0xFFFFBC00"
};
// Section: fashion.Button
static char* _fashion_Button_keys[]={ 
    "mode_bgc"
};
static char* _fashion_Button_values[]={ 
    "1"
};
// Section: fashion.RadioButton
static char* _fashion_RadioButton_keys[]={ 
    "mode_bgc"
};
static char* _fashion_RadioButton_values[]={ 
    "1"
};
// Section: fashion.CheckButton
static char* _fashion_CheckButton_keys[]={ 
    "mode_bgc"
};
static char* _fashion_CheckButton_values[]={ 
    "1"
};
// Section: fashion.MenuButton
static char* _fashion_MenuButton_keys[]={ 
    "mode_bgc"
};
static char* _fashion_MenuButton_values[]={ 
    "1"
};
// Section: fashion.ColorButton
static char* _fashion_ColorButton_keys[]={ 
    "mode_bgc"
};
static char* _fashion_ColorButton_values[]={ 
    "1"
};
// Section: fashion.ProgressBar
static char* _fashion_ProgressBar_keys[]={ 
    "bgc_pb_chunk"
};
static char* _fashion_ProgressBar_values[]={ 
    "0xFFC56A31"
};
// Section: fashion.TrackBar
static char* _fashion_TrackBar_keys[]={ 
    "metrics_3dbody_roundx",
    "metrics_3dbody_roundy",
    "bgc_tb_slider"
};
static char* _fashion_TrackBar_values[]={ 
    "1",
    "1",
    "0xFFC56A31"
};
// Section: fashion.SpinBox
static char* _fashion_SpinBox_keys[]={ 
    "metrics_3dbody_roundx",
    "metrics_3dbody_roundy"
};
static char* _fashion_SpinBox_values[]={ 
    "2",
    "2"
};
// Section: fashion.Spinner
static char* _fashion_Spinner_keys[]={ 
    "metrics_3dbody_roundx",
    "metrics_3dbody_roundy"
};
static char* _fashion_Spinner_values[]={ 
    "1",
    "1"
};
// Section: fashion.ListView
static char* _fashion_ListView_keys[]={ 
    "bgc_3dbody",
    "mode_bgc",
    "metrics_3dbody_roundx",
    "metrics_3dbody_roundy"
};
static char* _fashion_ListView_values[]={ 
    "0xFFFF5620",
    "1",
    "0",
    "0"
};
// Section: skin
static char* _skin_keys[]={ 
    "fgc_window",
    "bgc_window",
    "fgc_3dbody",
    "bgc_3dbody",
    "fgc_selected_item",
    "bgc_selected_item",
    "bgc_selected_nofocus",
    "fgc_disabled_item",
    "bgc_disabled_item",
    "fgc_hilight_item",
    "bgc_hilight_item",
    "image_bkgnd",
    "image_arrows",
    "image_arrowshell"
};
static char* _skin_values[]={ 
    "0xFF000000",
    "0xFFFFFFFF",
    "0xFF000000",
    "0xFFD8E9EC",
    "0xFFFFFFFF",
    "0xFFC56A31",
    "0xFFBDA69C",
    "0xFF99A8AC",
    "0xFFFFFFFF",
    "0xFFFFFFFF",
    "0xFFC56A31",
    "skin_bkgnd.bmp",
    "skin_arrows.gif",
    "skin_arrows_shell.bmp"
};
// Section: skin.Button
static char* _skin_Button_keys[]={ 
    "image_pushbutton"
};
static char* _skin_Button_values[]={ 
    "skin_pushbtn.gif"
};
// Section: skin.CheckButton
static char* _skin_CheckButton_keys[]={ 
    "image_checkbutton"
};
static char* _skin_CheckButton_values[]={ 
    "skin_checkbtn.bmp"
};
// Section: skin.RadioButton
static char* _skin_RadioButton_keys[]={ 
    "image_radiobutton"
};
static char* _skin_RadioButton_values[]={ 
    "skin_radiobtn.gif"
};
// Section: skin.MenuButton
static char* _skin_MenuButton_keys[]={ 
    "image_pushbutton"
};
static char* _skin_MenuButton_values[]={ 
    "skin_pushbtn.gif"
};
// Section: skin.ColorButton
static char* _skin_ColorButton_keys[]={ 
    "image_pushbutton"
};
static char* _skin_ColorButton_values[]={ 
    "skin_pushbtn.gif"
};
// Section: skin.ListView
static char* _skin_ListView_keys[]={ 
    "image_tree",
    "image_header"
};
static char* _skin_ListView_values[]={ 
    "skin_tree.bmp",
    "skin_header.bmp"
};
// Section: skin.PropSheet
static char* _skin_PropSheet_keys[]={ 
    "image_tab"
};
static char* _skin_PropSheet_values[]={ 
    "skin_tab.gif"
};
// Section: skin.ProgressBar
static char* _skin_ProgressBar_keys[]={ 
    "image_pb_hchunk",
    "image_pb_vchunk",
    "image_pb_hslider",
    "image_pb_vslider"
};
static char* _skin_ProgressBar_values[]={ 
    "skin_pb_htruck.bmp",
    "skin_pb_vtruck.bmp",
    "skin_pb_htrack.gif",
    "skin_pb_vtrack.gif"
};
// Section: skin.TrackBar
static char* _skin_TrackBar_keys[]={ 
    "image_tb_hslider",
    "image_tb_vslider",
    "image_tb_hthumb",
    "image_tb_vthumb"
};
static char* _skin_TrackBar_values[]={ 
    "skin_tbslider_h.gif",
    "skin_tbslider_v.gif",
    "skin_tb_horz.gif",
    "skin_tb_vert.gif"
};
// Section: flat
static char* _flat_keys[]={ 
    "fgc_window",
    "bgc_window",
    "fgc_3dbody",
    "bgc_3dbody",
//metrics_3dbody_roundx=3
//metrics_3dbody_roundy=3
    "fgc_hilight_item",
    "bgc_hilight_item",
    "fgc_disabled_item",
    "bgc_disabled_item",
    "fgc_selected_item",
    "bgc_selected_item",
    "bgc_selected_nofocus",
    "bgc_normal_tab"
};
static char* _flat_values[]={ 
    "0xFF000000",
    "0xFFFFFFFF",
    "0xFF000000",
    "0xFFFFFFFF",
//metrics_3dbody_roundx=3
//metrics_3dbody_roundy=3
    "0xFFFFFFFF",
    "0xFF000000",
    "0xFF99A8AC",
    "0xFFFFFFFF",
    "0xFFFFFFFF",
    "0xFF000000",
    "0xFFBDA69C",
    "0xFFC6D2CF"
};
// Section: flat.Spinner
static char* _flat_Spinner_keys[]={ 
};
static char* _flat_Spinner_values[]={ 
};
//metrics_3dbody_roundx=1
//metrics_3dbody_roundy=1
// Section: flat.ListView
static char* _flat_ListView_keys[]={ 
};
static char* _flat_ListView_values[]={ 
};
static ETCSECTION _etc_sections [] = { 
    {0, 12, "classic", _classic_keys,_classic_values }, 
    {0, 16, "fashion", _fashion_keys,_fashion_values }, 
    {0, 1, "fashion.Button", _fashion_Button_keys,_fashion_Button_values }, 
    {0, 1, "fashion.RadioButton", _fashion_RadioButton_keys,_fashion_RadioButton_values }, 
    {0, 1, "fashion.CheckButton", _fashion_CheckButton_keys,_fashion_CheckButton_values }, 
    {0, 1, "fashion.MenuButton", _fashion_MenuButton_keys,_fashion_MenuButton_values }, 
    {0, 1, "fashion.ColorButton", _fashion_ColorButton_keys,_fashion_ColorButton_values }, 
    {0, 1, "fashion.ProgressBar", _fashion_ProgressBar_keys,_fashion_ProgressBar_values }, 
    {0, 3, "fashion.TrackBar", _fashion_TrackBar_keys,_fashion_TrackBar_values }, 
    {0, 2, "fashion.SpinBox", _fashion_SpinBox_keys,_fashion_SpinBox_values }, 
    {0, 2, "fashion.Spinner", _fashion_Spinner_keys,_fashion_Spinner_values }, 
    {0, 4, "fashion.ListView", _fashion_ListView_keys,_fashion_ListView_values }, 
    {0, 14, "skin", _skin_keys,_skin_values }, 
    {0, 1, "skin.Button", _skin_Button_keys,_skin_Button_values }, 
    {0, 1, "skin.CheckButton", _skin_CheckButton_keys,_skin_CheckButton_values }, 
    {0, 1, "skin.RadioButton", _skin_RadioButton_keys,_skin_RadioButton_values }, 
    {0, 1, "skin.MenuButton", _skin_MenuButton_keys,_skin_MenuButton_values }, 
    {0, 1, "skin.ColorButton", _skin_ColorButton_keys,_skin_ColorButton_values }, 
    {0, 2, "skin.ListView", _skin_ListView_keys,_skin_ListView_values }, 
    {0, 1, "skin.PropSheet", _skin_PropSheet_keys,_skin_PropSheet_values }, 
    {0, 4, "skin.ProgressBar", _skin_ProgressBar_keys,_skin_ProgressBar_values }, 
    {0, 4, "skin.TrackBar", _skin_TrackBar_keys,_skin_TrackBar_values }, 
    {0, 12, "flat", _flat_keys,_flat_values }, 
    {0, 0, "flat.Spinner", _flat_Spinner_keys,_flat_Spinner_values }, 
    {0, 0, "flat.ListView", _flat_ListView_keys,_flat_ListView_values } 
};
//metrics_3dbody_roundx = 0
//metrics_3dbody_roundy = 0

///////////////////////////////////////////////////

static ETC_S _ETC = {
    0,
    sizeof(_etc_sections)/sizeof(ETCSECTION),
    _etc_sections
};

GHANDLE __mgncs_get_mgetc (void)
{
    return (GHANDLE) &_ETC;
}

#endif /* _MGINCORE_RES */

