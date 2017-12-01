static void create_enum_types(map<string, EnumType*>& enumMaps) {
    
    enumMaps["Direction"] = EnumType::create("Direction", "Vert",0x0,"Horz",0x2, NULL);

    enumMaps["Align"] = EnumType::create("Align", "Right",0x1,"Center",0x2,"Left",0x0, NULL);

    enumMaps["Boolean"] = EnumType::create("Boolean", "False",0x0,"True",0x1, NULL);

    enumMaps["VAlign"] = EnumType::create("VAlign", "Middle",0x2,"Top",0x0,"Bottom",0x1, NULL);

    enumMaps["DrawMode"] = EnumType::create("DrawMode", "Scaled",0x1,"Tiled",0x2,"Normal",0x0, NULL);

    enumMaps["GradientMode"] = EnumType::create("GradientMode", "Vert",0x1,"Horz",0x0, NULL);

}

static void create_all_props(map<string, Property*> &propMaps) {
    
    propMaps["Transparent"] = new Property(
                "Transparent",
                EnumType::create(NULL, "False",0x0,"True",0x2000, NULL),
                3000,
                Property::RDWT);

    propMaps["BgColor"] = new Property(
                "BgColor",
                getPropType(PropType::COLOR),
                1007,
                Property::RDWT);

    propMaps["Visible"] = new Property(
                "Visible",
                EnumType::create(NULL, "False",0x0,"True",0x8000000, NULL),
                2002,
                Property::RDWT);

    propMaps["Text"] = new Property(
                "Text",
                getPropType(PropType::TEXT),
                1005,
                Property::RDWT);

    propMaps["TabStop"] = new Property(
                "TabStop",
                EnumType::create(NULL, "False",0x0,"True",0x10000, NULL),
                2004,
                Property::RDWT);

    propMaps["BgImage"] = new Property(
                "BgImage",
                getPropType(PropType::IMAGE),
                3,
                Property::RDWT);

    propMaps["Enabled"] = new Property(
                "Enabled",
                EnumType::create(NULL, "False",0x4000000,"True",0x0, NULL),
                2003,
                Property::RDWT);

    propMaps["Height"] = new Property(
                "Height",
                getPropType(PropType::INT),
                1004,
                Property::RDWT);

    propMaps["Width"] = new Property(
                "Width",
                getPropType(PropType::INT),
                1003,
                Property::RDWT);

    propMaps["BgImageMode"] = new Property(
                "BgImageMode",
                getNamedEnumType("DrawMode"),
                4,
                Property::RDWT);

    propMaps["Renderer"] = new Property(
                "Renderer",
                getPropType(PropType::RDR),
                1006,
                Property::RDWT);

    propMaps["Y"] = new Property(
                "Y",
                getPropType(PropType::INT),
                1002,
                Property::RDWT);

    propMaps["X"] = new Property(
                "X",
                getPropType(PropType::INT),
                1001,
                Property::RDWT);

    propMaps["Font"] = new Property(
                "Font",
                getPropType(PropType::FONT),
                1008,
                Property::RDWT);

    propMaps["Border"] = new Property(
                "Border",
                EnumType::create(NULL, "False",0x0,"True",0x400000, NULL),
                2001,
                Property::RDWT);

    propMaps["Class"] = new Property(
                "Class",
                getPropType(PropType::STRING),
                1000,
                Property::RDWT);

    propMaps["Notify"] = new Property(
                "Notify",
                getNamedEnumType("Boolean"),
                2000,
                Property::RDWT);

    propMaps["LineStep"] = new Property(
                "LineStep",
                getPropType(PropType::INT),
                10,
                Property::RDWT);

    propMaps["Direction"] = new Property(
                "Direction",
                getNamedEnumType("Direction"),
                2005,
                Property::RDWT);

    propMaps["MinPos"] = new Property(
                "MinPos",
                getPropType(PropType::INT),
                8,
                Property::RDWT);

    propMaps["CurPos"] = new Property(
                "CurPos",
                getPropType(PropType::INT),
                9,
                Property::RDWT);

    propMaps["AutoLoop"] = new Property(
                "AutoLoop",
                EnumType::create(NULL, "False",0x0,"True",0x4, NULL),
                2006,
                Property::RDWT);

    propMaps["MaxPos"] = new Property(
                "MaxPos",
                getPropType(PropType::INT),
                7,
                Property::RDWT);

    propMaps["VStepValue"] = new Property(
                "VStepValue",
                getPropType(PropType::INT),
                12,
                Property::RDWT);

    propMaps["ScrollBarMode"] = new Property(
                "ScrollBarMode",
                EnumType::create(NULL, "Always",0x1,"Never",0x2,"Automatic",0x0, NULL),
                16,
                Property::RDWT);

    propMaps["VScroll"] = new Property(
                "VScroll",
                EnumType::create(NULL, "False",0x0,"True",0x80000, NULL),
                2049,
                Property::RDWT);

    propMaps["ContentY"] = new Property(
                "ContentY",
                getPropType(PropType::INT),
                10,
                Property::RDWT);

    propMaps["ContentX"] = new Property(
                "ContentX",
                getPropType(PropType::INT),
                9,
                Property::RDWT);

    propMaps["HScroll"] = new Property(
                "HScroll",
                EnumType::create(NULL, "False",0x0,"True",0x40000, NULL),
                2048,
                Property::RDWT);

    propMaps["HStepValue"] = new Property(
                "HStepValue",
                getPropType(PropType::INT),
                11,
                Property::RDWT);

    propMaps["Sort"] = new Property(
                "Sort",
                EnumType::create(NULL, "False",0x0,"True",0x4, NULL),
                2006,
                Property::RDWT);

    propMaps["Case"] = new Property(
                "Case",
                EnumType::create(NULL, "Upper",0x40,"Lower",0x80,"NoCase",0x0, NULL),
                2010,
                Property::RDWT);

    propMaps["ItemHeight"] = new Property(
                "ItemHeight",
                getPropType(PropType::INT),
                21,
                Property::RDWT);

    propMaps["EditorHasBoder"] = new Property(
                "EditorHasBoder",
                EnumType::create(NULL, "False",0x8,"True",0x0, NULL),
                2007,
                Property::RDWT);

    propMaps["DropDownHeight"] = new Property(
                "DropDownHeight",
                getPropType(PropType::INT),
                8,
                Property::RDWT);

    propMaps["EditorHasBaseLine"] = new Property(
                "EditorHasBaseLine",
                EnumType::create(NULL, "False",0x0,"True",0x10, NULL),
                2008,
                Property::RDWT);

    propMaps["ReadOnly"] = new Property(
                "ReadOnly",
                EnumType::create(NULL, "False",0x0,"True",0x20, NULL),
                2009,
                Property::RDWT);

    propMaps["AutoFocus"] = new Property(
                "AutoFocus",
                EnumType::create(NULL, "False",0x1,"True",0x100, NULL),
                2011,
                Property::RDWT);

    propMaps["Type"] = new Property(
                "Type",
                EnumType::create(NULL, "Simple",0x0,"DropDownList",0x2, NULL),
                2005,
                Property::RDWT);

    propMaps["TextLimit"] = new Property(
                "TextLimit",
                getPropType(PropType::INT),
                9,
                Property::RDWT);

    propMaps["PageStep"] = new Property(
                "PageStep",
                getPropType(PropType::INT),
                11,
                Property::RDWT);

    propMaps["Ruler"] = new Property(
                "Ruler",
                EnumType::create(NULL, "False",0x8,"True",0x0, NULL),
                2006,
                Property::RDWT);

    propMaps["ThumbHeight"] = new Property(
                "ThumbHeight",
                getPropType(PropType::INT),
                14,
                Property::RDWT);

    propMaps["ThumbWidth"] = new Property(
                "ThumbWidth",
                getPropType(PropType::INT),
                13,
                Property::RDWT);

    propMaps["Align"] = new Property(
                "Align",
                getNamedEnumType("Align"),
                7,
                Property::RDWT);

    propMaps["AutoWrap"] = new Property(
                "AutoWrap",
                getNamedEnumType("Boolean"),
                9,
                Property::RDWT);

    propMaps["Valign"] = new Property(
                "Valign",
                getNamedEnumType("VAlign"),
                8,
                Property::RDWT);

    propMaps["LedColor"] = new Property(
                "LedColor",
                getPropType(PropType::COLOR),
                11,
                Property::RDWT);

    propMaps["LedGap"] = new Property(
                "LedGap",
                getPropType(PropType::INT),
                14,
                Property::RDWT);

    propMaps["LedWidth"] = new Property(
                "LedWidth",
                getPropType(PropType::INT),
                12,
                Property::RDWT);

    propMaps["LedHeight"] = new Property(
                "LedHeight",
                getPropType(PropType::INT),
                13,
                Property::RDWT);

    propMaps["BlockChunk"] = new Property(
                "BlockChunk",
                EnumType::create(NULL, "False",0x0,"True",0x4, NULL),
                2005,
                Property::RDWT);

    propMaps["ShowPercent"] = new Property(
                "ShowPercent",
                EnumType::create(NULL, "False",0x0,"True",0x8, NULL),
                2006,
                Property::RDWT);

    propMaps["YRadius"] = new Property(
                "YRadius",
                getPropType(PropType::INT),
                11,
                Property::RDWT);

    propMaps["EdgeColor"] = new Property(
                "EdgeColor",
                getPropType(PropType::COLOR),
                8,
                Property::RDWT);

    propMaps["EdgeSize"] = new Property(
                "EdgeSize",
                getPropType(PropType::INT),
                7,
                Property::RDWT);

    propMaps["FillColor"] = new Property(
                "FillColor",
                getPropType(PropType::COLOR),
                9,
                Property::RDWT);

    propMaps["XRadius"] = new Property(
                "XRadius",
                getPropType(PropType::INT),
                10,
                Property::RDWT);

    propMaps["AutoSort"] = new Property(
                "AutoSort",
                EnumType::create(NULL, "False",0x0,"True",0x4, NULL),
                2006,
                Property::RDWT);

    propMaps["Loop"] = new Property(
                "Loop",
                EnumType::create(NULL, "False",0x0,"True",0x8, NULL),
                2007,
                Property::RDWT);

    propMaps["BaseLine"] = new Property(
                "BaseLine",
                EnumType::create(NULL, "False",0x0,"True",0x800, NULL),
                2013,
                Property::RDWT);

    propMaps["NeedIME"] = new Property(
                "NeedIME",
                EnumType::create(NULL, "False",0x1,"True",0x0, NULL),
                3001,
                Property::RDWT);

    propMaps["MaxLength"] = new Property(
                "MaxLength",
                getPropType(PropType::INT),
                22,
                Property::RDWT);

    propMaps["HideSel"] = new Property(
                "HideSel",
                EnumType::create(NULL, "False",0x200,"True",0x0, NULL),
                2011,
                Property::RDWT);

    propMaps["HaveTitle"] = new Property(
                "HaveTitle",
                EnumType::create(NULL, "False",0x0,"True",0x4000, NULL),
                2015,
                Property::RDWT);

    propMaps["LineSeparator"] = new Property(
                "LineSeparator",
                getPropType(-1),
                25,
                Property::RDWT);

    propMaps["CaretShap"] = new Property(
                "CaretShap",
                EnumType::create(NULL, "Line",0x0,"Block",0x1, NULL),
                26,
                Property::RDWT);

    propMaps["LineHeight"] = new Property(
                "LineHeight",
                getPropType(PropType::INT),
                23,
                Property::RDWT);

    propMaps["HaveArrows"] = new Property(
                "HaveArrows",
                EnumType::create(NULL, "False",0x0,"True",0x8, NULL),
                2006,
                Property::RDWT);

    propMaps["DoubleArrows"] = new Property(
                "DoubleArrows",
                EnumType::create(NULL, "None",0x0,"Right",0x20,"Left",0x10, NULL),
                2007,
                Property::RDWT);

    propMaps["Month"] = new Property(
                "Month",
                getPropType(PropType::INT),
                8,
                Property::RDWT);

    propMaps["Day"] = new Property(
                "Day",
                getPropType(PropType::INT),
                9,
                Property::RDWT);

    propMaps["Year"] = new Property(
                "Year",
                getPropType(PropType::INT),
                7,
                Property::RDWT);

    propMaps["HeadHeight"] = new Property(
                "HeadHeight",
                getPropType(PropType::INT),
                21,
                Property::RDWT);

    propMaps["GridLineColor"] = new Property(
                "GridLineColor",
                getPropType(PropType::COLOR),
                26,
                Property::RDWT);

    propMaps["WithIcon"] = new Property(
                "WithIcon",
                EnumType::create(NULL, "False",0x0,"True",0x100, NULL),
                2012,
                Property::RDWT);

    propMaps["Tree"] = new Property(
                "Tree",
                EnumType::create(NULL, "False",0x0,"True",0x80, NULL),
                2011,
                Property::RDWT);

    propMaps["GridLineWidth"] = new Property(
                "GridLineWidth",
                getPropType(PropType::INT),
                25,
                Property::RDWT);

    propMaps["HeadVisible"] = new Property(
                "HeadVisible",
                getNamedEnumType("Boolean"),
                23,
                Property::RDWT);

    propMaps["LabelType"] = new Property(
                "LabelType",
                EnumType::create(NULL, "Image",0x2,"VertImageLabel",0x60,"ImageLabel",0x20,"Label",0x0, NULL),
                2005,
                Property::RDWT);

    propMaps["Image"] = new Property(
                "Image",
                getPropType(PropType::IMAGE),
                10,
                Property::RDWT);

    propMaps["AutoCheck"] = new Property(
                "AutoCheck",
                EnumType::create(NULL, "False",0x0,"True",0x8, NULL),
                2007,
                Property::RDWT);

    propMaps["ThreeDCheck"] = new Property(
                "ThreeDCheck",
                EnumType::create(NULL, "False",0x0,"True",0x10, NULL),
                2008,
                Property::RDWT);

    propMaps["GroupId"] = new Property(
                "GroupId",
                getPropType(PropType::GROUP),
                13,
                Property::RDWT);

    propMaps["WordWrap"] = new Property(
                "WordWrap",
                getNamedEnumType("Boolean"),
                9,
                Property::RDWT);

    propMaps["CheckState"] = new Property(
                "CheckState",
                EnumType::create(NULL, "Checked",0x2,"UnChecked",0x0,"HalfChecked",0x1, NULL),
                11,
                Property::RDWT);

    propMaps["VAlign"] = new Property(
                "VAlign",
                getNamedEnumType("VAlign"),
                8,
                Property::RDWT);

    propMaps["Checkable"] = new Property(
                "Checkable",
                EnumType::create(NULL, "False",0x0,"True",0x4, NULL),
                2006,
                Property::RDWT);

    propMaps["ImageSizePercent"] = new Property(
                "ImageSizePercent",
                getPropType(PropType::INT),
                12,
                Property::RDWT);

    propMaps["imageIndent"] = new Property(
                "imageIndent",
                getPropType(PropType::INT),
                25,
                Property::RDWT);

    propMaps["textIndent"] = new Property(
                "textIndent",
                getPropType(PropType::INT),
                24,
                Property::RDWT);

    propMaps["defNodeW"] = new Property(
                "defNodeW",
                getPropType(PropType::INT),
                28,
                Property::RDWT);

    propMaps["layout"] = new Property(
                "layout",
                EnumType::create(NULL, "iconh",0x2,"iconv",0x1,"cboxh",0x3,"list",0x0,"group",0x4, NULL),
                27,
                Property::RDWT);

    propMaps["defNodeH"] = new Property(
                "defNodeH",
                getPropType(PropType::INT),
                29,
                Property::RDWT);

    propMaps["ContentType"] = new Property(
                "ContentType",
                EnumType::create(NULL, "Number",0x0,"String",0x8, NULL),
                2007,
                Property::RDWT);

    propMaps["EditBorder"] = new Property(
                "EditBorder",
                EnumType::create(NULL, "False",0x20,"True",0x0, NULL),
                2009,
                Property::RDWT);

    propMaps["EditBaseLine"] = new Property(
                "EditBaseLine",
                EnumType::create(NULL, "False",0x0,"True",0x40, NULL),
                2010,
                Property::RDWT);

    propMaps["PasswordChar"] = new Property(
                "PasswordChar",
                getPropType(-1),
                26,
                Property::RDWT);

    propMaps["Password"] = new Property(
                "Password",
                EnumType::create(NULL, "False",0x0,"True",0x2000, NULL),
                2014,
                Property::RDWT);

    propMaps["HasToolTip"] = new Property(
                "HasToolTip",
                EnumType::create(NULL, "False",0x0,"True",0x8000, NULL),
                2016,
                Property::RDWT);

    propMaps["ToolTip"] = new Property(
                "ToolTip",
                getPropType(PropType::TEXT),
                25,
                Property::RDWT);

    propMaps["AutoSelect"] = new Property(
                "AutoSelect",
                EnumType::create(NULL, "False",0x0,"True",0x4000, NULL),
                2015,
                Property::RDWT);

    propMaps["HasCaption"] = new Property(
                "HasCaption",
                EnumType::create(NULL, "False",0x0,"True",0x20000000, NULL),
                2005,
                Property::RDWT);

    propMaps["MaximizeBox"] = new Property(
                "MaximizeBox",
                EnumType::create(NULL, "False",0x0,"True",0x10000, NULL),
                2009,
                Property::RDWT);

    propMaps["MinimizeBox"] = new Property(
                "MinimizeBox",
                EnumType::create(NULL, "False",0x0,"True",0x20000, NULL),
                2008,
                Property::RDWT);

    propMaps["TopMost"] = new Property(
                "TopMost",
                EnumType::create(NULL, "False",0x0,"True",0x4000, NULL),
                3002,
                Property::RDWT);

    propMaps["BorderType"] = new Property(
                "BorderType",
                EnumType::create(NULL, "Simple",0x0,"DlgFrame",0x800000,"Thick",0x200000,"Thin",0x100000, NULL),
                2007,
                Property::RDWT);

    propMaps["ToolWindow"] = new Property(
                "ToolWindow",
                EnumType::create(NULL, "False",0x0,"True",0x8000, NULL),
                3003,
                Property::RDWT);

    propMaps["CloseBox"] = new Property(
                "CloseBox",
                EnumType::create(NULL, "False",0x400000,"True",0x0, NULL),
                3004,
                Property::RDWT);

    propMaps["SystemMenu"] = new Property(
                "SystemMenu",
                EnumType::create(NULL, "False",0x1,"True",0x10000000, NULL),
                2006,
                Property::RDWT);

    propMaps["SecondaryDC"] = new Property(
                "SecondaryDC",
                EnumType::create(NULL, "False",0x0,"True",0x1000, NULL),
                3001,
                Property::RDWT);

    propMaps["MouseFollow"] = new Property(
                "MouseFollow",
                EnumType::create(NULL, "False",0x0,"True",0x20, NULL),
                2009,
                Property::RDWT);

    propMaps["Multi"] = new Property(
                "Multi",
                EnumType::create(NULL, "False",0x0,"True",0x10, NULL),
                2008,
                Property::RDWT);

    propMaps["AutoCheckBox"] = new Property(
                "AutoCheckBox",
                EnumType::create(NULL, "AutoCheck",0x180,"CheckBox",0x80,"None",0x0, NULL),
                2011,
                Property::RDWT);

    propMaps["UseBitmap"] = new Property(
                "UseBitmap",
                EnumType::create(NULL, "False",0x0,"True",0x40, NULL),
                2010,
                Property::RDWT);

    propMaps["CurColor"] = new Property(
                "CurColor",
                getPropType(PropType::COLOR),
                7,
                Property::RDWT);

    propMaps["DrawMode"] = new Property(
                "DrawMode",
                getNamedEnumType("DrawMode"),
                13,
                Property::RDWT);

    propMaps["Scale"] = new Property(
                "Scale",
                EnumType::create(NULL, "None",0x0,"AutoFit",0x20,"ImageScaled",0x10, NULL),
                2006,
                Property::RDWT);

    propMaps["Interval"] = new Property(
                "Interval",
                getPropType(PropType::INT),
                13,
                Property::RDWT);

    propMaps["GIFFile"] = new Property(
                "GIFFile",
                getPropType(-1),
                11,
                Property::RDWT);

    propMaps["Dir"] = new Property(
                "Dir",
                getPropType(-1),
                12,
                Property::RDWT);

    propMaps["Autoplay"] = new Property(
                "Autoplay",
                EnumType::create(NULL, "False",0x0,"True",0x40, NULL),
                2008,
                Property::RDWT);

}

static void init_event_ids(map<string, int>& eventMaps) {
    
    eventMaps["onCreate"] = MSG_CREATE;

    eventMaps["onSizeChanging"] = MSG_SIZECHANGING;

    eventMaps["onSizeChanged"] = MSG_SIZECHANGED;

    eventMaps["onCSizeChanged"] = MSG_CSIZECHANGED;

    eventMaps["onFontChanging"] = MSG_FONTCHANGING;

    eventMaps["onFontChanged"] = MSG_FONTCHANGED;

    eventMaps["onEraseBkgrnd"] = MSG_ERASEBKGND;

    eventMaps["onPaint"] = MSG_PAINT;

    eventMaps["onClose"] = MSG_CLOSE;

    eventMaps["onKeyDown"] = MSG_KEYDOWN;

    eventMaps["onKeyUp"] = MSG_KEYUP;

    eventMaps["onChar"] = MSG_CHAR;

    eventMaps["onSysKeyDown"] = MSG_SYSKEYDOWN;

    eventMaps["onSysKeyUp"] = MSG_SYSKEYUP;

    eventMaps["onSysChar"] = MSG_SYSCHAR;

    eventMaps["onKeyLongPress"] = MSG_KEYLONGPRESS;

    eventMaps["onKeyAlwaysPress"] = MSG_KEYALWAYSPRESS;

    eventMaps["onLButtonDown"] = MSG_LBUTTONDOWN;

    eventMaps["onLButtonUp"] = MSG_LBUTTONUP;

    eventMaps["onLButtonDblClk"] = MSG_LBUTTONDBLCLK;

    eventMaps["onMouseMove"] = MSG_MOUSEMOVE;

    eventMaps["onRButtonDown"] = MSG_RBUTTONDOWN;

    eventMaps["onRButtonUp"] = MSG_RBUTTONUP;

    eventMaps["onRButtonDblClk"] = MSG_RBUTTONDBLCLK;

    eventMaps["onHitTest"] = MSG_HITTEST;

    eventMaps["onCommand"] = MSG_COMMAND;

    eventMaps["onDestroy"] = MSG_DESTROY;

    eventMaps["onSetFocus"] = MSG_SETFOCUS;

    eventMaps["onKillFocus"] = MSG_KILLFOCUS;

    eventMaps["onActive"] = MSG_ACTIVE;

    eventMaps["onPosChanged"] = NCS_NOTIFY_CODE(NCSN_SPNR_CHANGED);

    eventMaps["onReachMax"] = NCS_NOTIFY_CODE(NCSN_SPNR_REACHMAX);

    eventMaps["onReachMin"] = NCS_NOTIFY_CODE(NCSN_SPNR_REACHMIN);

    eventMaps["onHScroll"] = MSG_HSCROLL;

    eventMaps["onVScroll"] = MSG_VSCROLL;

    eventMaps["onSelChanged"] = NCS_NOTIFY_CODE(NCSN_CMBOX_SELCHANGE);

    eventMaps["onEditChange"] = NCS_NOTIFY_CODE(NCSN_CMBOX_EDITCHANGE);

    eventMaps["onDropDown"] = NCS_NOTIFY_CODE(NCSN_CMBOX_DROPDOWN);

    eventMaps["onCloseUp"] = NCS_NOTIFY_CODE(NCSN_CMBOX_CLOSEUP);

    eventMaps["onSelectOK"] = NCS_NOTIFY_CODE(NCSN_CMBOX_SELECTOK);

    eventMaps["onSelectCancel"] = NCS_NOTIFY_CODE(NCSN_CMBOX_SELECTCANCEL);

    eventMaps["onSelChanging"] = NCS_NOTIFY_CODE(NCSN_ITEMV_SELCHANGING);

    eventMaps["onClicked"] = NCS_NOTIFY_CODE(NCSN_WIDGET_CLICKED);

    eventMaps["onEnter"] = NCS_NOTIFY_CODE(NCSN_ITEMV_ENTER);

    eventMaps["onGainedFocus"] = NCS_NOTIFY_CODE(NCSN_ITEMV_SETFOCUS);

    eventMaps["onLostFocus"] = NCS_NOTIFY_CODE(NCSN_ITEMV_KILLFOCUS);

    eventMaps["onEditChanged"] = NCS_NOTIFY_CODE(NCSN_EDIT_CHANGE);

    eventMaps["onContentChanged"] = NCS_NOTIFY_CODE(NCSN_EDIT_CONTCHANGED);

    eventMaps["onUpdate"] = NCS_NOTIFY_CODE(NCSN_EDIT_UPDATE);

    eventMaps["onSelTextChanged"] = NCS_NOTIFY_CODE(NCSN_EDIT_SELCHANGED);

    eventMaps["onTextReachMax"] = NCS_NOTIFY_CODE(NCSN_EDIT_MAXTEXT);

    eventMaps["onYearChanged"] = NCS_NOTIFY_CODE(NCSN_CDR_YEAR_CHANGED);

    eventMaps["onMonthChanged"] = NCS_NOTIFY_CODE(NCSN_CDR_MONTH_CHANGED);

    eventMaps["onDayChanged"] = NCS_NOTIFY_CODE(NCSN_CDR_DAY_CHANGED);

    eventMaps["onItemRDown"] = NCS_NOTIFY_CODE(NCSN_LISTV_ITEMRDOWN);

    eventMaps["onItemRUp"] = NCS_NOTIFY_CODE(NCSN_LISTV_ITEMRUP);

    eventMaps["onHdrRDown"] = NCS_NOTIFY_CODE(NCSN_LISTV_HDRRDOWN);

    eventMaps["onHdrRUp"] = NCS_NOTIFY_CODE(NCSN_LISTV_HDRRUP);

    eventMaps["onItemDBClk"] = NCS_NOTIFY_CODE(NCSN_LISTV_ITEMDBCLK);

    eventMaps["onFoldItem"] = NCS_NOTIFY_CODE(NCSN_LISTV_FOLDITEM);

    eventMaps["onUnfoldItem"] = NCS_NOTIFY_CODE(NCSN_LISTV_UNFOLDITEM);

    eventMaps["onPushed"] = NCS_NOTIFY_CODE(NCSN_BUTTON_PUSHED);

    eventMaps["onStateChanged"] = NCS_NOTIFY_CODE(NCSN_BUTTON_STATE_CHANGED);

    eventMaps["onItemChanged"] = NCS_NOTIFY_CODE(NCSN_MNUBTN_ITEMCHANGED);

    eventMaps["onBackSpace"] = NCS_NOTIFY_CODE(NCSN_LIST_BACKSPACE);

    eventMaps["onEnterSubList"] = NCS_NOTIFY_CODE(NCSN_LIST_ENTERSUBLIST);

    eventMaps["onBackUpList"] = NCS_NOTIFY_CODE(NCSN_LIST_BACKUPLIST);

    eventMaps["onRootNodeChanged"] = NCS_NOTIFY_CODE(NCSN_LIST_ROOTNODECHANGED);

    eventMaps["onRadioButtonChanged"] = NCS_NOTIFY_CODE(NCSN_BTNGRP_SELCHANGED);

    eventMaps["onErrSpace"] = NCS_NOTIFY_CODE(NCSN_LSTBOX_ERRSPACE);

    eventMaps["onDBLClick"] = NCS_NOTIFY_CODE(NCSN_LSTBOX_DBCLK);

    eventMaps["onCheckMarkCancel"] = NCS_NOTIFY_CODE(NCSN_LSTBOX_CMCANCEL);

    eventMaps["onClickCheckMark"] = NCS_NOTIFY_CODE(NCSN_LSTBOX_CLKCHKMARK);

    eventMaps["onInitPage"] = MSG_INITPAGE;

    eventMaps["onShowPage"] = MSG_SHOWPAGE;

    eventMaps["onSheetCmd"] = MSG_SHEETCMD;

    eventMaps["onColorChanged"] = NCS_NOTIFY_CODE(NCSN_CLRBTN_COLORCHANGED);

}
static void init_widget_class(vector<WidgetClassDefine*> & widgetClasses) {
    {
    WidgetClassDefine * pwidget = new WidgetClassDefine("Widget", (mWidgetClass*)(&(Class(mWidget))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("Transparent"));
	pwidget->addProperty(getNamedProperty("BgColor"));
	pwidget->addProperty(getNamedProperty("Visible"));
	pwidget->addProperty(getNamedProperty("Text"));
	pwidget->addProperty(getNamedProperty("TabStop"));
	pwidget->addProperty(getNamedProperty("BgImage"));
	pwidget->addProperty(getNamedProperty("Enabled"));
	pwidget->addProperty(getNamedProperty("Height"));
	pwidget->addProperty(getNamedProperty("Width"));
	pwidget->addProperty(getNamedProperty("BgImageMode"));
	pwidget->addProperty(getNamedProperty("Renderer"));
	pwidget->addProperty(getNamedProperty("Y"));
	pwidget->addProperty(getNamedProperty("X"));
	pwidget->addProperty(getNamedProperty("Font"));
	pwidget->addProperty(getNamedProperty("Border"));
	pwidget->addProperty(getNamedProperty("Class"));
	pwidget->addProperty(getNamedProperty("Notify"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Spinner", (mWidgetClass*)(&(Class(mSpinner))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("LineStep"));
	pwidget->addProperty(getNamedProperty("Direction"));
	pwidget->addProperty(getNamedProperty("MinPos"));
	pwidget->addProperty(getNamedProperty("CurPos"));
	pwidget->addProperty(getNamedProperty("AutoLoop"));
	pwidget->addProperty(getNamedProperty("MaxPos"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("VSpinner", (mWidgetClass*)(&(Class(mVSpinner))));
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("ScrollWidget", (mWidgetClass*)(&(Class(mScrollWidget))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("VStepValue"));
	pwidget->addProperty(getNamedProperty("ScrollBarMode"));
	pwidget->addProperty(getNamedProperty("VScroll"));
	pwidget->addProperty(getNamedProperty("ContentY"));
	pwidget->addProperty(getNamedProperty("ContentX"));
	pwidget->addProperty(getNamedProperty("HScroll"));
	pwidget->addProperty(getNamedProperty("HStepValue"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Container", (mWidgetClass*)(&(Class(mContainer))));
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Panel", (mWidgetClass*)(&(Class(mPanel))));
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Combobox", (mWidgetClass*)(&(Class(mCombobox))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("Sort"));
	pwidget->addProperty(getNamedProperty("Case"));
	pwidget->addProperty(getNamedProperty("ItemHeight"));
	pwidget->addProperty(getNamedProperty("EditorHasBoder"));
	pwidget->addProperty(getNamedProperty("DropDownHeight"));
	pwidget->addProperty(getNamedProperty("EditorHasBaseLine"));
	pwidget->addProperty(getNamedProperty("ReadOnly"));
	pwidget->addProperty(getNamedProperty("AutoFocus"));
	pwidget->addProperty(getNamedProperty("Type"));
	pwidget->addProperty(getNamedProperty("TextLimit"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Separator", (mWidgetClass*)(&(Class(mSeparator))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("Direction"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("VSeparator", (mWidgetClass*)(&(Class(mVSeparator))));
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Slider", (mWidgetClass*)(&(Class(mSlider))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("LineStep"));
	pwidget->addProperty(getNamedProperty("Direction"));
	pwidget->addProperty(getNamedProperty("MinPos"));
	pwidget->addProperty(getNamedProperty("PageStep"));
	pwidget->addProperty(getNamedProperty("CurPos"));
	pwidget->addProperty(getNamedProperty("MaxPos"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("TrackBar", (mWidgetClass*)(&(Class(mTrackBar))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("Ruler"));
	pwidget->addProperty(getNamedProperty("ThumbHeight"));
	pwidget->addProperty(getNamedProperty("ThumbWidth"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("VTrackBar", (mWidgetClass*)(&(Class(mVTrackBar))));
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Static", (mWidgetClass*)(&(Class(mStatic))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("Align"));
	pwidget->addProperty(getNamedProperty("AutoWrap"));
	pwidget->addProperty(getNamedProperty("Valign"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("LEDLabel", (mWidgetClass*)(&(Class(mLEDLabel))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("LedColor"));
	pwidget->addProperty(getNamedProperty("LedGap"));
	pwidget->addProperty(getNamedProperty("LedWidth"));
	pwidget->addProperty(getNamedProperty("LedHeight"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("ProgressBar", (mWidgetClass*)(&(Class(mProgressBar))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("LineStep"));
	pwidget->addProperty(getNamedProperty("Direction"));
	pwidget->addProperty(getNamedProperty("MinPos"));
	pwidget->addProperty(getNamedProperty("BlockChunk"));
	pwidget->addProperty(getNamedProperty("CurPos"));
	pwidget->addProperty(getNamedProperty("ShowPercent"));
	pwidget->addProperty(getNamedProperty("MaxPos"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("VProgressBar", (mWidgetClass*)(&(Class(mVProgressBar))));
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Rectangle", (mWidgetClass*)(&(Class(mRectangle))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("YRadius"));
	pwidget->addProperty(getNamedProperty("EdgeColor"));
	pwidget->addProperty(getNamedProperty("EdgeSize"));
	pwidget->addProperty(getNamedProperty("FillColor"));
	pwidget->addProperty(getNamedProperty("XRadius"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("ItemView", (mWidgetClass*)(&(Class(mItemView))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("AutoSort"));
	pwidget->addProperty(getNamedProperty("Loop"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("ScrollView", (mWidgetClass*)(&(Class(mScrollView))));
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Edit", (mWidgetClass*)(&(Class(mEdit))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("Case"));
	pwidget->addProperty(getNamedProperty("BaseLine"));
	pwidget->addProperty(getNamedProperty("Align"));
	pwidget->addProperty(getNamedProperty("NeedIME"));
	pwidget->addProperty(getNamedProperty("ReadOnly"));
	pwidget->addProperty(getNamedProperty("MaxLength"));
	pwidget->addProperty(getNamedProperty("HideSel"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("MlEdit", (mWidgetClass*)(&(Class(mMlEdit))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("HaveTitle"));
	pwidget->addProperty(getNamedProperty("LineSeparator"));
	pwidget->addProperty(getNamedProperty("AutoWrap"));
	pwidget->addProperty(getNamedProperty("CaretShap"));
	pwidget->addProperty(getNamedProperty("LineHeight"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("ScrollBar", (mWidgetClass*)(&(Class(mScrollBar))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("HaveArrows"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("HScrollBar", (mWidgetClass*)(&(Class(mHScrollBar))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("DoubleArrows"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("MonthCalendar", (mWidgetClass*)(&(Class(mMonthCalendar))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("Month"));
	pwidget->addProperty(getNamedProperty("Day"));
	pwidget->addProperty(getNamedProperty("Year"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("HSpinner", (mWidgetClass*)(&(Class(mHSpinner))));
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("HSeparator", (mWidgetClass*)(&(Class(mHSeparator))));
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("GroupBox", (mWidgetClass*)(&(Class(mGroupBox))));
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("ListView", (mWidgetClass*)(&(Class(mListView))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("HeadHeight"));
	pwidget->addProperty(getNamedProperty("GridLineColor"));
	pwidget->addProperty(getNamedProperty("WithIcon"));
	pwidget->addProperty(getNamedProperty("Tree"));
	pwidget->addProperty(getNamedProperty("GridLineWidth"));
	pwidget->addProperty(getNamedProperty("HeadVisible"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Button", (mWidgetClass*)(&(Class(mButton))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("LabelType"));
	pwidget->addProperty(getNamedProperty("Image"));
	pwidget->addProperty(getNamedProperty("AutoCheck"));
	pwidget->addProperty(getNamedProperty("ThreeDCheck"));
	pwidget->addProperty(getNamedProperty("GroupId"));
	pwidget->addProperty(getNamedProperty("WordWrap"));
	pwidget->addProperty(getNamedProperty("CheckState"));
	pwidget->addProperty(getNamedProperty("VAlign"));
	pwidget->addProperty(getNamedProperty("Checkable"));
	pwidget->addProperty(getNamedProperty("ImageSizePercent"));
	pwidget->addProperty(getNamedProperty("Align"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("MenuButton", (mWidgetClass*)(&(Class(mMenuButton))));
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("AbstractList", (mWidgetClass*)(&(Class(mAbstractList))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("imageIndent"));
	pwidget->addProperty(getNamedProperty("AutoSort"));
	pwidget->addProperty(getNamedProperty("textIndent"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("List", (mWidgetClass*)(&(Class(mList))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("defNodeW"));
	pwidget->addProperty(getNamedProperty("layout"));
	pwidget->addProperty(getNamedProperty("Loop"));
	pwidget->addProperty(getNamedProperty("defNodeH"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("SpinBox", (mWidgetClass*)(&(Class(mSpinBox))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("Sort"));
	pwidget->addProperty(getNamedProperty("Case"));
	pwidget->addProperty(getNamedProperty("ContentType"));
	pwidget->addProperty(getNamedProperty("ReadOnly"));
	pwidget->addProperty(getNamedProperty("EditBorder"));
	pwidget->addProperty(getNamedProperty("EditBaseLine"));
	pwidget->addProperty(getNamedProperty("AutoFocus"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("CheckButton", (mWidgetClass*)(&(Class(mCheckButton))));
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("RadioButton", (mWidgetClass*)(&(Class(mRadioButton))));
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("ButtonGroup", (mWidgetClass*)(&(Class(mButtonGroup))));
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("SlEdit", (mWidgetClass*)(&(Class(mSlEdit))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("PasswordChar"));
	pwidget->addProperty(getNamedProperty("Password"));
	pwidget->addProperty(getNamedProperty("HasToolTip"));
	pwidget->addProperty(getNamedProperty("ToolTip"));
	pwidget->addProperty(getNamedProperty("AutoSelect"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("HTrackBar", (mWidgetClass*)(&(Class(mHTrackBar))));
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("MainWnd", (mWidgetClass*)(&(Class(mMainWnd))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("HasCaption"));
	pwidget->addProperty(getNamedProperty("MaximizeBox"));
	pwidget->addProperty(getNamedProperty("MinimizeBox"));
	pwidget->addProperty(getNamedProperty("TopMost"));
	pwidget->addProperty(getNamedProperty("BorderType"));
	pwidget->addProperty(getNamedProperty("ToolWindow"));
	pwidget->addProperty(getNamedProperty("CloseBox"));
	pwidget->addProperty(getNamedProperty("SystemMenu"));
	pwidget->addProperty(getNamedProperty("SecondaryDC"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("DialogBox", (mWidgetClass*)(&(Class(mDialogBox))));
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("ListBox", (mWidgetClass*)(&(Class(mListBox))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("MouseFollow"));
	pwidget->addProperty(getNamedProperty("Multi"));
	pwidget->addProperty(getNamedProperty("ItemHeight"));
	pwidget->addProperty(getNamedProperty("AutoCheckBox"));
	pwidget->addProperty(getNamedProperty("UseBitmap"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Page", (mWidgetClass*)(&(Class(mPage))));
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("HProgressBar", (mWidgetClass*)(&(Class(mHProgressBar))));
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("VScrollBar", (mWidgetClass*)(&(Class(mVScrollBar))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("DoubleArrows"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("ColorButton", (mWidgetClass*)(&(Class(mColorButton))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("CurColor"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Image", (mWidgetClass*)(&(Class(mImage))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("Image"));
	pwidget->addProperty(getNamedProperty("DrawMode"));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("IconView", (mWidgetClass*)(&(Class(mIconView))));
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Animate", (mWidgetClass*)(&(Class(mAnimate))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("Scale"));
	pwidget->addProperty(getNamedProperty("Interval"));
	pwidget->addProperty(getNamedProperty("Loop"));
	pwidget->addProperty(getNamedProperty("GIFFile"));
	pwidget->addProperty(getNamedProperty("Dir"));
	pwidget->addProperty(getNamedProperty("Autoplay"));

}

}
