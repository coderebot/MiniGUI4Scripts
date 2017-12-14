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
                Property::RDWT,
                new PropValue(NULL, "False"));

    propMaps["BgColor"] = new Property(
                "BgColor",
                getPropType(PropType::COLOR),
                1007,
                Property::RDWT,
                new PropValue(NULL, (long)0xFFCED3D6));

    propMaps["Visible"] = new Property(
                "Visible",
                EnumType::create(NULL, "False",0x0,"True",0x8000000, NULL),
                2002,
                Property::RDWT,
                new PropValue(NULL, "True"));

    propMaps["Text"] = new Property(
                "Text",
                getPropType(PropType::TEXT),
                1005,
                Property::RDWT,
                NULL);

    propMaps["TabStop"] = new Property(
                "TabStop",
                EnumType::create(NULL, "False",0x0,"True",0x10000, NULL),
                2004,
                Property::RDWT,
                new PropValue(NULL, "True"));

    propMaps["BgImage"] = new Property(
                "BgImage",
                getPropType(PropType::IMAGE),
                3,
                Property::RDWT,
                NULL);

    propMaps["Enabled"] = new Property(
                "Enabled",
                EnumType::create(NULL, "False",0x4000000,"True",0x0, NULL),
                2003,
                Property::RDWT,
                new PropValue(NULL, "True"));

    propMaps["Height"] = new Property(
                "Height",
                getPropType(PropType::INT),
                1004,
                Property::RDWT,
                new PropValue(NULL, (long)30));

    propMaps["Width"] = new Property(
                "Width",
                getPropType(PropType::INT),
                1003,
                Property::RDWT,
                new PropValue(NULL, (long)80));

    propMaps["BgImageMode"] = new Property(
                "BgImageMode",
                getNamedEnumType("DrawMode"),
                4,
                Property::RDWT,
                new PropValue(NULL, "Normal"));

    propMaps["Renderer"] = new Property(
                "Renderer",
                getPropType(PropType::RDR),
                1006,
                Property::RDWT,
                NULL);

    propMaps["Y"] = new Property(
                "Y",
                getPropType(PropType::INT),
                1002,
                Property::RDWT,
                new PropValue(NULL, (long)0));

    propMaps["X"] = new Property(
                "X",
                getPropType(PropType::INT),
                1001,
                Property::RDWT,
                new PropValue(NULL, (long)0));

    propMaps["Font"] = new Property(
                "Font",
                getPropType(PropType::FONT),
                1008,
                Property::RDWT,
                NULL);

    propMaps["Border"] = new Property(
                "Border",
                EnumType::create(NULL, "False",0x0,"True",0x400000, NULL),
                2001,
                Property::RDWT,
                new PropValue(NULL, "False"));

    propMaps["Class"] = new Property(
                "Class",
                getPropType(PropType::STRING),
                1000,
                Property::RDWT,
                new PropValue(NULL, "widget"));

    propMaps["Notify"] = new Property(
                "Notify",
                getNamedEnumType("Boolean"),
                2000,
                Property::RDWT,
                new PropValue(NULL, "True"));

    propMaps["VStepValue"] = new Property(
                "VStepValue",
                getPropType(PropType::INT),
                12,
                Property::RDWT,
                new PropValue(NULL, (long)10));

    propMaps["ScrollBarMode"] = new Property(
                "ScrollBarMode",
                EnumType::create(NULL, "Always",0x1,"Never",0x2,"Automatic",0x0, NULL),
                16,
                Property::RDWT,
                new PropValue(NULL, "Automatic"));

    propMaps["VScroll"] = new Property(
                "VScroll",
                EnumType::create(NULL, "False",0x0,"True",0x80000, NULL),
                2049,
                Property::RDWT,
                new PropValue(NULL, "True"));

    propMaps["ContentY"] = new Property(
                "ContentY",
                getPropType(PropType::INT),
                10,
                Property::RDWT,
                NULL);

    propMaps["ContentX"] = new Property(
                "ContentX",
                getPropType(PropType::INT),
                9,
                Property::RDWT,
                NULL);

    propMaps["HScroll"] = new Property(
                "HScroll",
                EnumType::create(NULL, "False",0x0,"True",0x40000, NULL),
                2048,
                Property::RDWT,
                new PropValue(NULL, "True"));

    propMaps["HStepValue"] = new Property(
                "HStepValue",
                getPropType(PropType::INT),
                11,
                Property::RDWT,
                new PropValue(NULL, (long)10));

    propMaps["Sort"] = new Property(
                "Sort",
                EnumType::create(NULL, "False",0x0,"True",0x4, NULL),
                2006,
                Property::RDWT,
                new PropValue(NULL, "False"));

    propMaps["Case"] = new Property(
                "Case",
                EnumType::create(NULL, "Upper",0x40,"Lower",0x80,"NoCase",0x0, NULL),
                2010,
                Property::RDWT,
                NULL);

    propMaps["ItemHeight"] = new Property(
                "ItemHeight",
                getPropType(PropType::INT),
                21,
                Property::RDWT,
                new PropValue(NULL, (long)16));

    propMaps["EditorHasBoder"] = new Property(
                "EditorHasBoder",
                EnumType::create(NULL, "False",0x8,"True",0x0, NULL),
                2007,
                Property::RDWT,
                new PropValue(NULL, "True"));

    propMaps["DropDownHeight"] = new Property(
                "DropDownHeight",
                getPropType(PropType::INT),
                8,
                Property::RDWT,
                new PropValue(NULL, (long)60));

    propMaps["EditorHasBaseLine"] = new Property(
                "EditorHasBaseLine",
                EnumType::create(NULL, "False",0x0,"True",0x10, NULL),
                2008,
                Property::RDWT,
                NULL);

    propMaps["ReadOnly"] = new Property(
                "ReadOnly",
                EnumType::create(NULL, "False",0x0,"True",0x20, NULL),
                2009,
                Property::RDWT,
                NULL);

    propMaps["AutoFocus"] = new Property(
                "AutoFocus",
                EnumType::create(NULL, "False",0x1,"True",0x100, NULL),
                2011,
                Property::RDWT,
                new PropValue(NULL, "False"));

    propMaps["Type"] = new Property(
                "Type",
                EnumType::create(NULL, "Simple",0x0,"DropDownList",0x2, NULL),
                2005,
                Property::RDWT,
                new PropValue(NULL, "DropDownList"));

    propMaps["TextLimit"] = new Property(
                "TextLimit",
                getPropType(PropType::INT),
                9,
                Property::RDWT,
                new PropValue(NULL, (long)-1));

    propMaps["LineStep"] = new Property(
                "LineStep",
                getPropType(PropType::INT),
                10,
                Property::RDWT,
                new PropValue(NULL, (long)5));

    propMaps["Direction"] = new Property(
                "Direction",
                getNamedEnumType("Direction"),
                2005,
                Property::RDWT,
                NULL);

    propMaps["MinPos"] = new Property(
                "MinPos",
                getPropType(PropType::INT),
                8,
                Property::RDWT,
                new PropValue(NULL, (long)0));

    propMaps["CurPos"] = new Property(
                "CurPos",
                getPropType(PropType::INT),
                9,
                Property::RDWT,
                new PropValue(NULL, (long)0));

    propMaps["AutoLoop"] = new Property(
                "AutoLoop",
                EnumType::create(NULL, "False",0x0,"True",0x4, NULL),
                2006,
                Property::RDWT,
                NULL);

    propMaps["MaxPos"] = new Property(
                "MaxPos",
                getPropType(PropType::INT),
                7,
                Property::RDWT,
                new PropValue(NULL, (long)100));

    propMaps["Align"] = new Property(
                "Align",
                getNamedEnumType("Align"),
                7,
                Property::RDWT,
                new PropValue(NULL, "Center"));

    propMaps["AutoWrap"] = new Property(
                "AutoWrap",
                getNamedEnumType("Boolean"),
                9,
                Property::RDWT,
                new PropValue(NULL, "False"));

    propMaps["Valign"] = new Property(
                "Valign",
                getNamedEnumType("VAlign"),
                8,
                Property::RDWT,
                new PropValue(NULL, "Middle"));

    propMaps["LedColor"] = new Property(
                "LedColor",
                getPropType(PropType::COLOR),
                11,
                Property::RDWT,
                new PropValue(NULL, (long)0xFFFFFFFF));

    propMaps["LedGap"] = new Property(
                "LedGap",
                getPropType(PropType::INT),
                14,
                Property::RDWT,
                new PropValue(NULL, (long)5));

    propMaps["LedWidth"] = new Property(
                "LedWidth",
                getPropType(PropType::INT),
                12,
                Property::RDWT,
                new PropValue(NULL, (long)20));

    propMaps["LedHeight"] = new Property(
                "LedHeight",
                getPropType(PropType::INT),
                13,
                Property::RDWT,
                new PropValue(NULL, (long)30));

    propMaps["YRadius"] = new Property(
                "YRadius",
                getPropType(PropType::INT),
                11,
                Property::RDWT,
                new PropValue(NULL, (long)0));

    propMaps["EdgeColor"] = new Property(
                "EdgeColor",
                getPropType(PropType::COLOR),
                8,
                Property::RDWT,
                new PropValue(NULL, (long)0xFF000000));

    propMaps["EdgeSize"] = new Property(
                "EdgeSize",
                getPropType(PropType::INT),
                7,
                Property::RDWT,
                new PropValue(NULL, (long)1));

    propMaps["FillColor"] = new Property(
                "FillColor",
                getPropType(PropType::COLOR),
                9,
                Property::RDWT,
                new PropValue(NULL, (long)0xFFFFFFFF));

    propMaps["XRadius"] = new Property(
                "XRadius",
                getPropType(PropType::INT),
                10,
                Property::RDWT,
                new PropValue(NULL, (long)0));

    propMaps["Month"] = new Property(
                "Month",
                getPropType(PropType::INT),
                8,
                Property::RDWT,
                NULL);

    propMaps["Day"] = new Property(
                "Day",
                getPropType(PropType::INT),
                9,
                Property::RDWT,
                NULL);

    propMaps["Year"] = new Property(
                "Year",
                getPropType(PropType::INT),
                7,
                Property::RDWT,
                NULL);

    propMaps["PageStep"] = new Property(
                "PageStep",
                getPropType(PropType::INT),
                11,
                Property::RDWT,
                new PropValue(NULL, (long)3));

    propMaps["HaveArrows"] = new Property(
                "HaveArrows",
                EnumType::create(NULL, "False",0x0,"True",0x8, NULL),
                2006,
                Property::RDWT,
                new PropValue(NULL, "True"));

    propMaps["AutoSort"] = new Property(
                "AutoSort",
                EnumType::create(NULL, "False",0x0,"True",0x4, NULL),
                2006,
                Property::RDWT,
                NULL);

    propMaps["Loop"] = new Property(
                "Loop",
                EnumType::create(NULL, "False",0x0,"True",0x8, NULL),
                2007,
                Property::RDWT,
                NULL);

    propMaps["HeadHeight"] = new Property(
                "HeadHeight",
                getPropType(PropType::INT),
                21,
                Property::RDWT,
                new PropValue(NULL, (long)25));

    propMaps["GridLineColor"] = new Property(
                "GridLineColor",
                getPropType(PropType::COLOR),
                26,
                Property::RDWT,
                new PropValue(NULL, (long)0x777777));

    propMaps["WithIcon"] = new Property(
                "WithIcon",
                EnumType::create(NULL, "False",0x0,"True",0x100, NULL),
                2012,
                Property::RDWT,
                new PropValue(NULL, "False"));

    propMaps["Tree"] = new Property(
                "Tree",
                EnumType::create(NULL, "False",0x0,"True",0x80, NULL),
                2011,
                Property::RDWT,
                new PropValue(NULL, "False"));

    propMaps["GridLineWidth"] = new Property(
                "GridLineWidth",
                getPropType(PropType::INT),
                25,
                Property::RDWT,
                new PropValue(NULL, (long)0));

    propMaps["HeadVisible"] = new Property(
                "HeadVisible",
                getNamedEnumType("Boolean"),
                23,
                Property::RDWT,
                new PropValue(NULL, "True"));

    propMaps["LabelType"] = new Property(
                "LabelType",
                EnumType::create(NULL, "Image",0x2,"VertImageLabel",0x60,"ImageLabel",0x20,"Label",0x0, NULL),
                2005,
                Property::RDWT,
                new PropValue(NULL, "Label"));

    propMaps["Image"] = new Property(
                "Image",
                getPropType(PropType::IMAGE),
                10,
                Property::RDWT,
                NULL);

    propMaps["AutoCheck"] = new Property(
                "AutoCheck",
                EnumType::create(NULL, "False",0x0,"True",0x8, NULL),
                2007,
                Property::RDWT,
                new PropValue(NULL, "False"));

    propMaps["ThreeDCheck"] = new Property(
                "ThreeDCheck",
                EnumType::create(NULL, "False",0x0,"True",0x10, NULL),
                2008,
                Property::RDWT,
                new PropValue(NULL, "False"));

    propMaps["GroupId"] = new Property(
                "GroupId",
                getPropType(PropType::GROUP),
                13,
                Property::RDWT,
                new PropValue(NULL, (long)-1));

    propMaps["WordWrap"] = new Property(
                "WordWrap",
                getNamedEnumType("Boolean"),
                9,
                Property::RDWT,
                new PropValue(NULL, "False"));

    propMaps["CheckState"] = new Property(
                "CheckState",
                EnumType::create(NULL, "Checked",0x2,"UnChecked",0x0,"HalfChecked",0x1, NULL),
                11,
                Property::RDWT,
                new PropValue(NULL, "UnChecked"));

    propMaps["VAlign"] = new Property(
                "VAlign",
                getNamedEnumType("VAlign"),
                8,
                Property::RDWT,
                new PropValue(NULL, "Middle"));

    propMaps["Checkable"] = new Property(
                "Checkable",
                EnumType::create(NULL, "False",0x0,"True",0x4, NULL),
                2006,
                Property::RDWT,
                new PropValue(NULL, "False"));

    propMaps["ImageSizePercent"] = new Property(
                "ImageSizePercent",
                getPropType(PropType::INT),
                12,
                Property::RDWT,
                new PropValue(NULL, (long)25));

    propMaps["BaseLine"] = new Property(
                "BaseLine",
                EnumType::create(NULL, "False",0x0,"True",0x800, NULL),
                2013,
                Property::RDWT,
                new PropValue(NULL, "False"));

    propMaps["NeedIME"] = new Property(
                "NeedIME",
                EnumType::create(NULL, "False",0x1,"True",0x0, NULL),
                3001,
                Property::RDWT,
                new PropValue(NULL, "True"));

    propMaps["MaxLength"] = new Property(
                "MaxLength",
                getPropType(PropType::INT),
                22,
                Property::RDWT,
                new PropValue(NULL, (long)-1));

    propMaps["HideSel"] = new Property(
                "HideSel",
                EnumType::create(NULL, "False",0x200,"True",0x0, NULL),
                2011,
                Property::RDWT,
                new PropValue(NULL, "True"));

    propMaps["imageIndent"] = new Property(
                "imageIndent",
                getPropType(PropType::INT),
                25,
                Property::RDWT,
                new PropValue(NULL, (long)-1));

    propMaps["textIndent"] = new Property(
                "textIndent",
                getPropType(PropType::INT),
                24,
                Property::RDWT,
                new PropValue(NULL, (long)-1));

    propMaps["defNodeW"] = new Property(
                "defNodeW",
                getPropType(PropType::INT),
                28,
                Property::RDWT,
                new PropValue(NULL, (long)64));

    propMaps["layout"] = new Property(
                "layout",
                EnumType::create(NULL, "iconh",0x2,"iconv",0x1,"cboxh",0x3,"list",0x0,"group",0x4, NULL),
                27,
                Property::RDWT,
                NULL);

    propMaps["defNodeH"] = new Property(
                "defNodeH",
                getPropType(PropType::INT),
                29,
                Property::RDWT,
                new PropValue(NULL, (long)24));

    propMaps["ContentType"] = new Property(
                "ContentType",
                EnumType::create(NULL, "Number",0x0,"String",0x8, NULL),
                2007,
                Property::RDWT,
                NULL);

    propMaps["EditBorder"] = new Property(
                "EditBorder",
                EnumType::create(NULL, "False",0x20,"True",0x0, NULL),
                2009,
                Property::RDWT,
                new PropValue(NULL, "True"));

    propMaps["EditBaseLine"] = new Property(
                "EditBaseLine",
                EnumType::create(NULL, "False",0x0,"True",0x40, NULL),
                2010,
                Property::RDWT,
                NULL);

    propMaps["PasswordChar"] = new Property(
                "PasswordChar",
                getPropType(PropType::INT),
                26,
                Property::RDWT,
                new PropValue(NULL, (long)'*'));

    propMaps["Password"] = new Property(
                "Password",
                EnumType::create(NULL, "False",0x0,"True",0x2000, NULL),
                2014,
                Property::RDWT,
                new PropValue(NULL, "False"));

    propMaps["HasToolTip"] = new Property(
                "HasToolTip",
                EnumType::create(NULL, "False",0x0,"True",0x8000, NULL),
                2016,
                Property::RDWT,
                new PropValue(NULL, "False"));

    propMaps["ToolTip"] = new Property(
                "ToolTip",
                getPropType(PropType::TEXT),
                25,
                Property::RDWT,
                NULL);

    propMaps["AutoSelect"] = new Property(
                "AutoSelect",
                EnumType::create(NULL, "False",0x0,"True",0x4000, NULL),
                2015,
                Property::RDWT,
                new PropValue(NULL, "False"));

    propMaps["HasCaption"] = new Property(
                "HasCaption",
                EnumType::create(NULL, "False",0x0,"True",0x20000000, NULL),
                2005,
                Property::RDWT,
                new PropValue(NULL, "True"));

    propMaps["MaximizeBox"] = new Property(
                "MaximizeBox",
                EnumType::create(NULL, "False",0x0,"True",0x10000, NULL),
                2009,
                Property::RDWT,
                new PropValue(NULL, "True"));

    propMaps["MinimizeBox"] = new Property(
                "MinimizeBox",
                EnumType::create(NULL, "False",0x0,"True",0x20000, NULL),
                2008,
                Property::RDWT,
                new PropValue(NULL, "True"));

    propMaps["TopMost"] = new Property(
                "TopMost",
                EnumType::create(NULL, "False",0x0,"True",0x4000, NULL),
                3002,
                Property::RDWT,
                new PropValue(NULL, "False"));

    propMaps["BorderType"] = new Property(
                "BorderType",
                EnumType::create(NULL, "Simple",0x0,"DlgFrame",0x800000,"Thick",0x200000,"Thin",0x100000, NULL),
                2007,
                Property::RDWT,
                new PropValue(NULL, "Simple"));

    propMaps["ToolWindow"] = new Property(
                "ToolWindow",
                EnumType::create(NULL, "False",0x0,"True",0x8000, NULL),
                3003,
                Property::RDWT,
                new PropValue(NULL, "False"));

    propMaps["CloseBox"] = new Property(
                "CloseBox",
                EnumType::create(NULL, "False",0x400000,"True",0x0, NULL),
                3004,
                Property::RDWT,
                new PropValue(NULL, "True"));

    propMaps["SystemMenu"] = new Property(
                "SystemMenu",
                EnumType::create(NULL, "False",0x1,"True",0x10000000, NULL),
                2006,
                Property::RDWT,
                new PropValue(NULL, "True"));

    propMaps["SecondaryDC"] = new Property(
                "SecondaryDC",
                EnumType::create(NULL, "False",0x0,"True",0x1000, NULL),
                3001,
                Property::RDWT,
                new PropValue(NULL, "False"));

    propMaps["MouseFollow"] = new Property(
                "MouseFollow",
                EnumType::create(NULL, "False",0x0,"True",0x20, NULL),
                2009,
                Property::RDWT,
                new PropValue(NULL, "False"));

    propMaps["Multi"] = new Property(
                "Multi",
                EnumType::create(NULL, "False",0x0,"True",0x10, NULL),
                2008,
                Property::RDWT,
                NULL);

    propMaps["AutoCheckBox"] = new Property(
                "AutoCheckBox",
                EnumType::create(NULL, "AutoCheck",0x180,"CheckBox",0x80,"None",0x0, NULL),
                2011,
                Property::RDWT,
                new PropValue(NULL, "None"));

    propMaps["UseBitmap"] = new Property(
                "UseBitmap",
                EnumType::create(NULL, "False",0x0,"True",0x40, NULL),
                2010,
                Property::RDWT,
                new PropValue(NULL, "False"));

    propMaps["CurColor"] = new Property(
                "CurColor",
                getPropType(PropType::COLOR),
                7,
                Property::RDWT,
                new PropValue(NULL, (long)0xFFFFFFFF));

    propMaps["DrawMode"] = new Property(
                "DrawMode",
                getNamedEnumType("DrawMode"),
                13,
                Property::RDWT,
                new PropValue(NULL, "Normal"));

    propMaps["BlockChunk"] = new Property(
                "BlockChunk",
                EnumType::create(NULL, "False",0x0,"True",0x4, NULL),
                2005,
                Property::RDWT,
                new PropValue(NULL, "False"));

    propMaps["ShowPercent"] = new Property(
                "ShowPercent",
                EnumType::create(NULL, "False",0x0,"True",0x8, NULL),
                2006,
                Property::RDWT,
                new PropValue(NULL, "False"));

    propMaps["Ruler"] = new Property(
                "Ruler",
                EnumType::create(NULL, "False",0x8,"True",0x0, NULL),
                2006,
                Property::RDWT,
                new PropValue(NULL, "True"));

    propMaps["ThumbHeight"] = new Property(
                "ThumbHeight",
                getPropType(PropType::INT),
                14,
                Property::RDWT,
                NULL);

    propMaps["ThumbWidth"] = new Property(
                "ThumbWidth",
                getPropType(PropType::INT),
                13,
                Property::RDWT,
                NULL);

    propMaps["Scale"] = new Property(
                "Scale",
                EnumType::create(NULL, "None",0x0,"AutoFit",0x20,"ImageScaled",0x10, NULL),
                2006,
                Property::RDWT,
                new PropValue(NULL, "None"));

    propMaps["Interval"] = new Property(
                "Interval",
                getPropType(PropType::INT),
                13,
                Property::RDWT,
                new PropValue(NULL, (long)100));

    propMaps["GIFFile"] = new Property(
                "GIFFile",
                getPropType(-1),
                11,
                Property::RDWT,
                NULL);

    propMaps["Dir"] = new Property(
                "Dir",
                getPropType(-1),
                12,
                Property::RDWT,
                NULL);

    propMaps["Autoplay"] = new Property(
                "Autoplay",
                EnumType::create(NULL, "False",0x0,"True",0x40, NULL),
                2008,
                Property::RDWT,
                new PropValue(NULL, "True"));

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

    
    
    pwidget->addEvent("onCreate", MSG_CREATE);

    pwidget->addEvent("onSizeChanging", MSG_SIZECHANGING);

    pwidget->addEvent("onSizeChanged", MSG_SIZECHANGED);

    pwidget->addEvent("onCSizeChanged", MSG_CSIZECHANGED);

    pwidget->addEvent("onFontChanging", MSG_FONTCHANGING);

    pwidget->addEvent("onFontChanged", MSG_FONTCHANGED);

    pwidget->addEvent("onEraseBkgrnd", MSG_ERASEBKGND);

    pwidget->addEvent("onPaint", MSG_PAINT);

    pwidget->addEvent("onClose", MSG_CLOSE);

    pwidget->addEvent("onKeyDown", MSG_KEYDOWN);

    pwidget->addEvent("onKeyUp", MSG_KEYUP);

    pwidget->addEvent("onChar", MSG_CHAR);

    pwidget->addEvent("onSysKeyDown", MSG_SYSKEYDOWN);

    pwidget->addEvent("onSysKeyUp", MSG_SYSKEYUP);

    pwidget->addEvent("onSysChar", MSG_SYSCHAR);

    pwidget->addEvent("onKeyLongPress", MSG_KEYLONGPRESS);

    pwidget->addEvent("onKeyAlwaysPress", MSG_KEYALWAYSPRESS);

    pwidget->addEvent("onLButtonDown", MSG_LBUTTONDOWN);

    pwidget->addEvent("onLButtonUp", MSG_LBUTTONUP);

    pwidget->addEvent("onLButtonDblClk", MSG_LBUTTONDBLCLK);

    pwidget->addEvent("onMouseMove", MSG_MOUSEMOVE);

    pwidget->addEvent("onRButtonDown", MSG_RBUTTONDOWN);

    pwidget->addEvent("onRButtonUp", MSG_RBUTTONUP);

    pwidget->addEvent("onRButtonDblClk", MSG_RBUTTONDBLCLK);

    pwidget->addEvent("onHitTest", MSG_HITTEST);

    pwidget->addEvent("onCommand", MSG_COMMAND);

    pwidget->addEvent("onDestroy", MSG_DESTROY);

    pwidget->addEvent("onSetFocus", MSG_SETFOCUS);

    pwidget->addEvent("onKillFocus", MSG_KILLFOCUS);

    pwidget->addEvent("onActive", MSG_ACTIVE);

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

    
    
    pwidget->addEvent("onHScroll", MSG_HSCROLL);

    pwidget->addEvent("onVScroll", MSG_VSCROLL);

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Container", (mWidgetClass*)(&(Class(mContainer))));
    widgetClasses.push_back(pwidget);
    
    	pwidget->setDefPropValue("Width", new PropValue(NULL, (long)200));
	pwidget->setDefPropValue("Border", new PropValue(NULL, "True"));
	pwidget->setDefPropValue("Height", new PropValue(NULL, (long)200));

    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Panel", (mWidgetClass*)(&(Class(mPanel))));
    widgetClasses.push_back(pwidget);
    
    	pwidget->setDefPropValue("Width", new PropValue(NULL, (long)200));
	pwidget->setDefPropValue("Height", new PropValue(NULL, (long)200));

    
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

    	pwidget->setDefPropValue("Width", new PropValue(NULL, (long)120));
	pwidget->setDefPropValue("Font", new PropValue(NULL, ""));
	pwidget->setDefPropValue("Height", new PropValue(NULL, (long)25));

    
    pwidget->addEvent("onSelChanged", NCS_NOTIFY_CODE(NCSN_CMBOX_SELCHANGE));

    pwidget->addEvent("onSetFocus", NCS_NOTIFY_CODE(NCSN_CMBOX_SETFOCUS));

    pwidget->addEvent("onKillFocus", NCS_NOTIFY_CODE(NCSN_CMBOX_KILLFOCUS));

    pwidget->addEvent("onEditChange", NCS_NOTIFY_CODE(NCSN_CMBOX_EDITCHANGE));

    pwidget->addEvent("onDropDown", NCS_NOTIFY_CODE(NCSN_CMBOX_DROPDOWN));

    pwidget->addEvent("onCloseUp", NCS_NOTIFY_CODE(NCSN_CMBOX_CLOSEUP));

    pwidget->addEvent("onSelectOK", NCS_NOTIFY_CODE(NCSN_CMBOX_SELECTOK));

    pwidget->addEvent("onSelectCancel", NCS_NOTIFY_CODE(NCSN_CMBOX_SELECTCANCEL));

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

    
    
    pwidget->addEvent("onPosChanged", NCS_NOTIFY_CODE(NCSN_SPNR_CHANGED));

    pwidget->addEvent("onReachMax", NCS_NOTIFY_CODE(NCSN_SPNR_REACHMAX));

    pwidget->addEvent("onReachMin", NCS_NOTIFY_CODE(NCSN_SPNR_REACHMIN));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Static", (mWidgetClass*)(&(Class(mStatic))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("Align"));
	pwidget->addProperty(getNamedProperty("AutoWrap"));
	pwidget->addProperty(getNamedProperty("Valign"));

    	pwidget->setDefPropValue("Width", new PropValue(NULL, (long)100));
	pwidget->setDefPropValue("TabStop", new PropValue(NULL, "False"));
	pwidget->setDefPropValue("Height", new PropValue(NULL, (long)25));

    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("LEDLabel", (mWidgetClass*)(&(Class(mLEDLabel))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("LedColor"));
	pwidget->addProperty(getNamedProperty("LedGap"));
	pwidget->addProperty(getNamedProperty("LedWidth"));
	pwidget->addProperty(getNamedProperty("LedHeight"));

    	pwidget->setDefPropValue("Height", new PropValue(NULL, (long)30));

    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Rectangle", (mWidgetClass*)(&(Class(mRectangle))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("YRadius"));
	pwidget->addProperty(getNamedProperty("EdgeColor"));
	pwidget->addProperty(getNamedProperty("EdgeSize"));
	pwidget->addProperty(getNamedProperty("FillColor"));
	pwidget->addProperty(getNamedProperty("XRadius"));

    	pwidget->setDefPropValue("Width", new PropValue(NULL, (long)100));
	pwidget->setDefPropValue("Border", new PropValue(NULL, "False"));
	pwidget->setDefPropValue("Height", new PropValue(NULL, (long)30));

    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("MonthCalendar", (mWidgetClass*)(&(Class(mMonthCalendar))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("Month"));
	pwidget->addProperty(getNamedProperty("Day"));
	pwidget->addProperty(getNamedProperty("Year"));

    	pwidget->setDefPropValue("Width", new PropValue(NULL, (long)250));
	pwidget->setDefPropValue("Border", new PropValue(NULL, "True"));
	pwidget->setDefPropValue("Height", new PropValue(NULL, (long)150));

    
    pwidget->addEvent("onYearChanged", NCS_NOTIFY_CODE(NCSN_CDR_YEAR_CHANGED));

    pwidget->addEvent("onMonthChanged", NCS_NOTIFY_CODE(NCSN_CDR_MONTH_CHANGED));

    pwidget->addEvent("onDayChanged", NCS_NOTIFY_CODE(NCSN_CDR_DAY_CHANGED));

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

    	pwidget->setDefPropValue("Width", new PropValue(NULL, (long)100));
	pwidget->setDefPropValue("Height", new PropValue(NULL, (long)100));

    
    pwidget->addEvent("onPosChanged", NCS_NOTIFY_CODE(NCSN_SLIDER_CHANGED));

    pwidget->addEvent("onReachMax", NCS_NOTIFY_CODE(NCSN_SLIDER_REACHMAX));

    pwidget->addEvent("onReachMin", NCS_NOTIFY_CODE(NCSN_SLIDER_REACHMIN));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("ScrollBar", (mWidgetClass*)(&(Class(mScrollBar))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("HaveArrows"));

    
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("GroupBox", (mWidgetClass*)(&(Class(mGroupBox))));
    widgetClasses.push_back(pwidget);
    
    	pwidget->setDefPropValue("Width", new PropValue(NULL, (long)100));
	pwidget->setDefPropValue("Border", new PropValue(NULL, "False"));
	pwidget->setDefPropValue("Height", new PropValue(NULL, (long)100));

    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("ItemView", (mWidgetClass*)(&(Class(mItemView))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("AutoSort"));
	pwidget->addProperty(getNamedProperty("Loop"));

    	pwidget->setDefPropValue("BgColor", new PropValue(NULL, (long)0xFFFFFFFF));

    
    pwidget->addEvent("onSelChanging", NCS_NOTIFY_CODE(NCSN_ITEMV_SELCHANGING));

    pwidget->addEvent("onClicked", NCS_NOTIFY_CODE(NCSN_WIDGET_CLICKED));

    pwidget->addEvent("onSelChanged", NCS_NOTIFY_CODE(NCSN_ITEMV_SELCHANGED));

    pwidget->addEvent("onEnter", NCS_NOTIFY_CODE(NCSN_ITEMV_ENTER));

    pwidget->addEvent("onGainedFocus", NCS_NOTIFY_CODE(NCSN_ITEMV_SETFOCUS));

    pwidget->addEvent("onLostFocus", NCS_NOTIFY_CODE(NCSN_ITEMV_KILLFOCUS));

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

    	pwidget->setDefPropValue("Width", new PropValue(NULL, (long)250));
	pwidget->setDefPropValue("Border", new PropValue(NULL, "True"));
	pwidget->setDefPropValue("Height", new PropValue(NULL, (long)250));

    
    pwidget->addEvent("onItemRDown", NCS_NOTIFY_CODE(NCSN_LISTV_ITEMRDOWN));

    pwidget->addEvent("onItemRUp", NCS_NOTIFY_CODE(NCSN_LISTV_ITEMRUP));

    pwidget->addEvent("onHdrRDown", NCS_NOTIFY_CODE(NCSN_LISTV_HDRRDOWN));

    pwidget->addEvent("onHdrRUp", NCS_NOTIFY_CODE(NCSN_LISTV_HDRRUP));

    pwidget->addEvent("onItemDBClk", NCS_NOTIFY_CODE(NCSN_LISTV_ITEMDBCLK));

    pwidget->addEvent("onFoldItem", NCS_NOTIFY_CODE(NCSN_LISTV_FOLDITEM));

    pwidget->addEvent("onUnfoldItem", NCS_NOTIFY_CODE(NCSN_LISTV_UNFOLDITEM));

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

    	pwidget->setDefPropValue("Width", new PropValue(NULL, (long)80));
	pwidget->setDefPropValue("Border", new PropValue(NULL, "False"));
	pwidget->setDefPropValue("Height", new PropValue(NULL, (long)30));

    
    pwidget->addEvent("onClicked", NCS_NOTIFY_CODE(NCSN_WIDGET_CLICKED));

    pwidget->addEvent("onPushed", NCS_NOTIFY_CODE(NCSN_BUTTON_PUSHED));

    pwidget->addEvent("onStateChanged", NCS_NOTIFY_CODE(NCSN_BUTTON_STATE_CHANGED));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("MenuButton", (mWidgetClass*)(&(Class(mMenuButton))));
    widgetClasses.push_back(pwidget);
    
    	pwidget->setDefPropValue("Width", new PropValue(NULL, (long)120));
	pwidget->setDefPropValue("Height", new PropValue(NULL, (long)30));

    
    pwidget->addEvent("onItemChanged", NCS_NOTIFY_CODE(NCSN_MNUBTN_ITEMCHANGED));

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

    	pwidget->setDefPropValue("BgColor", new PropValue(NULL, (long)0xFFFFFFFF));
	pwidget->setDefPropValue("Border", new PropValue(NULL, "True"));

    
    pwidget->addEvent("onEditChanged", NCS_NOTIFY_CODE(NCSN_EDIT_CHANGE));

    pwidget->addEvent("onContentChanged", NCS_NOTIFY_CODE(NCSN_EDIT_CONTCHANGED));

    pwidget->addEvent("onUpdate", NCS_NOTIFY_CODE(NCSN_EDIT_UPDATE));

    pwidget->addEvent("onSelTextChanged", NCS_NOTIFY_CODE(NCSN_EDIT_SELCHANGED));

    pwidget->addEvent("onTextReachMax", NCS_NOTIFY_CODE(NCSN_EDIT_MAXTEXT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("AbstractList", (mWidgetClass*)(&(Class(mAbstractList))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("imageIndent"));
	pwidget->addProperty(getNamedProperty("AutoSort"));
	pwidget->addProperty(getNamedProperty("textIndent"));

    	pwidget->setDefPropValue("BgColor", new PropValue(NULL, (long)0xFFFFFFFF));

    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("List", (mWidgetClass*)(&(Class(mList))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("defNodeW"));
	pwidget->addProperty(getNamedProperty("layout"));
	pwidget->addProperty(getNamedProperty("Loop"));
	pwidget->addProperty(getNamedProperty("defNodeH"));

    	pwidget->setDefPropValue("Width", new PropValue(NULL, (long)130));
	pwidget->setDefPropValue("Border", new PropValue(NULL, "True"));
	pwidget->setDefPropValue("BgColor", new PropValue(NULL, (long)0xFFFFFFFF));
	pwidget->setDefPropValue("Height", new PropValue(NULL, (long)100));

    
    pwidget->addEvent("onSelChanged", NCS_NOTIFY_CODE(NCSN_LIST_SELCHANGED));

    pwidget->addEvent("onEnter", NCS_NOTIFY_CODE(NCSN_LIST_ENTER));

    pwidget->addEvent("onBackSpace", NCS_NOTIFY_CODE(NCSN_LIST_BACKSPACE));

    pwidget->addEvent("onEnterSubList", NCS_NOTIFY_CODE(NCSN_LIST_ENTERSUBLIST));

    pwidget->addEvent("onBackUpList", NCS_NOTIFY_CODE(NCSN_LIST_BACKUPLIST));

    pwidget->addEvent("onRootNodeChanged", NCS_NOTIFY_CODE(NCSN_LIST_ROOTNODECHANGED));

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

    	pwidget->setDefPropValue("Width", new PropValue(NULL, (long)70));
	pwidget->setDefPropValue("Font", new PropValue(NULL, ""));
	pwidget->setDefPropValue("Height", new PropValue(NULL, (long)32));

    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("CheckButton", (mWidgetClass*)(&(Class(mCheckButton))));
    widgetClasses.push_back(pwidget);
    
    	pwidget->setDefPropValue("Width", new PropValue(NULL, (long)160));
	pwidget->setDefPropValue("Align", new PropValue(NULL, "Left"));
	pwidget->setDefPropValue("AutoCheck", new PropValue(NULL, "True"));
	pwidget->setDefPropValue("Transparent", new PropValue(NULL, "False"));
	pwidget->setDefPropValue("Height", new PropValue(NULL, (long)25));

    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("RadioButton", (mWidgetClass*)(&(Class(mRadioButton))));
    widgetClasses.push_back(pwidget);
    
    	pwidget->setDefPropValue("GroupId", new PropValue(NULL, (long)-1));

    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Separator", (mWidgetClass*)(&(Class(mSeparator))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("Direction"));

    	pwidget->setDefPropValue("Transparent", new PropValue(NULL, "True"));
	pwidget->setDefPropValue("Notify", new PropValue(NULL, "False"));

    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("ButtonGroup", (mWidgetClass*)(&(Class(mButtonGroup))));
    widgetClasses.push_back(pwidget);
    
    	pwidget->setDefPropValue("Width", new PropValue(NULL, (long)100));
	pwidget->setDefPropValue("Height", new PropValue(NULL, (long)100));

    
    pwidget->addEvent("onRadioButtonChanged", NCS_NOTIFY_CODE(NCSN_BTNGRP_SELCHANGED));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("SlEdit", (mWidgetClass*)(&(Class(mSlEdit))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("PasswordChar"));
	pwidget->addProperty(getNamedProperty("Password"));
	pwidget->addProperty(getNamedProperty("HasToolTip"));
	pwidget->addProperty(getNamedProperty("ToolTip"));
	pwidget->addProperty(getNamedProperty("AutoSelect"));

    	pwidget->setDefPropValue("Width", new PropValue(NULL, (long)200));
	pwidget->setDefPropValue("Height", new PropValue(NULL, (long)30));

    
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

    	pwidget->setDefPropValue("Width", new PropValue(NULL, (long)200));
	pwidget->setDefPropValue("Border", new PropValue(NULL, "True"));
	pwidget->setDefPropValue("Class", new PropValue(NULL, "mainwnd"));
	pwidget->setDefPropValue("Height", new PropValue(NULL, (long)200));

    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("DialogBox", (mWidgetClass*)(&(Class(mDialogBox))));
    widgetClasses.push_back(pwidget);
    
    	pwidget->setDefPropValue("Width", new PropValue(NULL, (long)200));
	pwidget->setDefPropValue("Height", new PropValue(NULL, (long)200));

    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("ListBox", (mWidgetClass*)(&(Class(mListBox))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("MouseFollow"));
	pwidget->addProperty(getNamedProperty("Multi"));
	pwidget->addProperty(getNamedProperty("ItemHeight"));
	pwidget->addProperty(getNamedProperty("AutoCheckBox"));
	pwidget->addProperty(getNamedProperty("UseBitmap"));

    	pwidget->setDefPropValue("Width", new PropValue(NULL, (long)130));
	pwidget->setDefPropValue("Border", new PropValue(NULL, "True"));
	pwidget->setDefPropValue("BgColor", new PropValue(NULL, (long)0xFFFFFFFF));
	pwidget->setDefPropValue("Height", new PropValue(NULL, (long)100));

    
    pwidget->addEvent("onErrSpace", NCS_NOTIFY_CODE(NCSN_LSTBOX_ERRSPACE));

    pwidget->addEvent("onDBLClick", NCS_NOTIFY_CODE(NCSN_LSTBOX_DBCLK));

    pwidget->addEvent("onCheckMarkCancel", NCS_NOTIFY_CODE(NCSN_LSTBOX_CMCANCEL));

    pwidget->addEvent("onClickCheckMark", NCS_NOTIFY_CODE(NCSN_LSTBOX_CLKCHKMARK));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Page", (mWidgetClass*)(&(Class(mPage))));
    widgetClasses.push_back(pwidget);
    
    	pwidget->setDefPropValue("Text", new PropValue(NULL, "0"));
	pwidget->setDefPropValue("Border", new PropValue(NULL, "False"));

    
    pwidget->addEvent("onInitPage", MSG_INITPAGE);

    pwidget->addEvent("onShowPage", MSG_SHOWPAGE);

    pwidget->addEvent("onSheetCmd", MSG_SHEETCMD);

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("ColorButton", (mWidgetClass*)(&(Class(mColorButton))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("CurColor"));

    	pwidget->setDefPropValue("Width", new PropValue(NULL, (long)80));
	pwidget->setDefPropValue("Border", new PropValue(NULL, "False"));
	pwidget->setDefPropValue("Height", new PropValue(NULL, (long)30));

    
    pwidget->addEvent("onColorChanged", NCS_NOTIFY_CODE(NCSN_CLRBTN_COLORCHANGED));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Image", (mWidgetClass*)(&(Class(mImage))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("Image"));
	pwidget->addProperty(getNamedProperty("DrawMode"));

    	pwidget->setDefPropValue("Width", new PropValue(NULL, (long)100));
	pwidget->setDefPropValue("Border", new PropValue(NULL, "False"));
	pwidget->setDefPropValue("Height", new PropValue(NULL, (long)100));

    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("IconView", (mWidgetClass*)(&(Class(mIconView))));
    widgetClasses.push_back(pwidget);
    
    	pwidget->setDefPropValue("Width", new PropValue(NULL, (long)150));
	pwidget->setDefPropValue("Border", new PropValue(NULL, "True"));
	pwidget->setDefPropValue("Height", new PropValue(NULL, (long)150));

    
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

    	pwidget->setDefPropValue("Border", new PropValue(NULL, "True"));

    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("TrackBar", (mWidgetClass*)(&(Class(mTrackBar))));
    widgetClasses.push_back(pwidget);
    	pwidget->addProperty(getNamedProperty("Ruler"));
	pwidget->addProperty(getNamedProperty("ThumbHeight"));
	pwidget->addProperty(getNamedProperty("ThumbWidth"));

    
    
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

    	pwidget->setDefPropValue("Width", new PropValue(NULL, (long)100));
	pwidget->setDefPropValue("Border", new PropValue(NULL, "False"));
	pwidget->setDefPropValue("Height", new PropValue(NULL, (long)100));

    
}

}
