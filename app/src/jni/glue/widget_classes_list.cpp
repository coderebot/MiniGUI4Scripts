static void create_enum_types(map<string, EnumType*>& enumMaps) {
    
    enumMaps["Direction"] = EnumType::create("Direction", "Vert",0x0,"Horz",0x2, NULL);

    enumMaps["Align"] = EnumType::create("Align", "Right",0x1,"Center",0x2,"Left",0x0, NULL);

    enumMaps["Boolean"] = EnumType::create("Boolean", "False",0x0,"True",0x1, NULL);

    enumMaps["VAlign"] = EnumType::create("VAlign", "Middle",0x2,"Top",0x0,"Bottom",0x1, NULL);

    enumMaps["DrawMode"] = EnumType::create("DrawMode", "Scaled",0x1,"Tiled",0x2,"Normal",0x0, NULL);

    enumMaps["GradientMode"] = EnumType::create("GradientMode", "Vert",0x1,"Horz",0x0, NULL);

}
static void init_widget_class(vector<WidgetClassDefine*> & widgetClasses) {
    {
    WidgetClassDefine * pwidget = new WidgetClassDefine("Widget");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "Transparent",
                EnumType::create(NULL, "False",0x0,"True",0x2000, NULL),
                3000,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "BgColor",
                getPropType(PropType::COLOR),
                1007,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Visible",
                EnumType::create(NULL, "False",0x0,"True",0x8000000, NULL),
                2002,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Text",
                getPropType(PropType::TEXT),
                1005,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "TabStop",
                EnumType::create(NULL, "False",0x0,"True",0x10000, NULL),
                2004,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "BgImage",
                getPropType(PropType::IMAGE),
                3,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Enabled",
                EnumType::create(NULL, "False",0x4000000,"True",0x0, NULL),
                2003,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Height",
                getPropType(PropType::INT),
                1004,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Width",
                getPropType(PropType::INT),
                1003,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "BgImageMode",
                getNamedEnumType("DrawMode"),
                4,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Renderer",
                getPropType(PropType::RDR),
                1006,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Y",
                getPropType(PropType::INT),
                1002,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "X",
                getPropType(PropType::INT),
                1001,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Font",
                getPropType(PropType::FONT),
                1008,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Border",
                EnumType::create(NULL, "False",0x0,"True",0x400000, NULL),
                2001,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Class",
                getPropType(PropType::STRING),
                1000,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Notify",
                getNamedEnumType("Boolean"),
                2000,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Spinner");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "LineStep",
                getPropType(PropType::INT),
                10,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Direction",
                getNamedEnumType("Direction"),
                2005,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "MinPos",
                getPropType(PropType::INT),
                8,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "CurPos",
                getPropType(PropType::INT),
                9,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "AutoLoop",
                EnumType::create(NULL, "False",0x0,"True",0x4, NULL),
                2006,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "MaxPos",
                getPropType(PropType::INT),
                7,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("VSpinner");
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("ScrollWidget");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "VStepValue",
                getPropType(PropType::INT),
                12,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "ScrollBarMode",
                EnumType::create(NULL, "Always",0x1,"Never",0x2,"Automatic",0x0, NULL),
                16,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "VScroll",
                EnumType::create(NULL, "False",0x0,"True",0x80000, NULL),
                2049,
                Property::READ));
    pwidget->addProperty(new Property(
                "ContentY",
                getPropType(PropType::INT),
                10,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "ContentX",
                getPropType(PropType::INT),
                9,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "HScroll",
                EnumType::create(NULL, "False",0x0,"True",0x40000, NULL),
                2048,
                Property::READ));
    pwidget->addProperty(new Property(
                "HStepValue",
                getPropType(PropType::INT),
                11,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Container");
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Panel");
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Combobox");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "Sort",
                EnumType::create(NULL, "False",0x0,"True",0x4, NULL),
                2006,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Case",
                EnumType::create(NULL, "Upper",0x40,"Lower",0x80,"NoCase",0x0, NULL),
                2010,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "ItemHeight",
                getPropType(PropType::INT),
                21,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "EditorHasBoder",
                EnumType::create(NULL, "False",0x8,"True",0x0, NULL),
                2007,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "DropDownHeight",
                getPropType(PropType::INT),
                8,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "EditorHasBaseLine",
                EnumType::create(NULL, "False",0x0,"True",0x10, NULL),
                2008,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "ReadOnly",
                EnumType::create(NULL, "False",0x0,"True",0x20, NULL),
                2009,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "AutoFocus",
                EnumType::create(NULL, "False",0x1,"True",0x100, NULL),
                2011,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Type",
                EnumType::create(NULL, "Simple",0x0,"DropDownList",0x2, NULL),
                2005,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "TextLimit",
                getPropType(PropType::INT),
                9,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("IMWordSel");
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Separator");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "Direction",
                EnumType::create(NULL, "Horz",0x0,"Vert",0x4, NULL),
                2005,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("VSeparator");
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Slider");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "LineStep",
                getPropType(PropType::INT),
                10,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Direction",
                EnumType::create(NULL, "Horz",0x0,"Vert",0x2, NULL),
                2005,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "MinPos",
                getPropType(PropType::INT),
                8,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "PageStep",
                getPropType(PropType::INT),
                11,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "CurPos",
                getPropType(PropType::INT),
                9,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "MaxPos",
                getPropType(PropType::INT),
                7,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("TrackBar");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "Ruler",
                EnumType::create(NULL, "False",0x8,"True",0x0, NULL),
                2006,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "ThumbHeight",
                getPropType(PropType::INT),
                14,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "ThumbWidth",
                getPropType(PropType::INT),
                13,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("VTrackBar");
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Static");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "Align",
                getNamedEnumType("Align"),
                7,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "AutoWrap",
                getNamedEnumType("Boolean"),
                9,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Valign",
                getNamedEnumType("VAlign"),
                8,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("LEDLabel");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "LedColor",
                getPropType(PropType::COLOR),
                11,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "LedGap",
                getPropType(PropType::INT),
                14,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "LedWidth",
                getPropType(PropType::INT),
                12,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "LedHeight",
                getPropType(PropType::INT),
                13,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("ProgressBar");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "LineStep",
                getPropType(PropType::INT),
                10,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Direction",
                EnumType::create(NULL, "Horz",0x0,"Vert",0x2, NULL),
                2004,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "MinPos",
                getPropType(PropType::INT),
                8,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "BlockChunk",
                EnumType::create(NULL, "False",0x0,"True",0x4, NULL),
                2005,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "CurPos",
                getPropType(PropType::INT),
                9,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "ShowPercent",
                EnumType::create(NULL, "False",0x0,"True",0x8, NULL),
                2006,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "MaxPos",
                getPropType(PropType::INT),
                7,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("VProgressBar");
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Rectangle");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "YRadius",
                getPropType(PropType::INT),
                11,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "EdgeColor",
                getPropType(PropType::COLOR),
                8,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "EdgeSize",
                getPropType(PropType::INT),
                7,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "FillColor",
                getPropType(PropType::COLOR),
                9,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "XRadius",
                getPropType(PropType::INT),
                10,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("ItemView");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "AutoSort",
                EnumType::create(NULL, "False",0x0,"True",0x4, NULL),
                2006,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Loop",
                EnumType::create(NULL, "False",0x0,"True",0x8, NULL),
                2007,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("ScrollView");
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Edit");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "Case",
                EnumType::create(NULL, "Upper",0x80,"None",0x0,"Lower",0x100, NULL),
                2010,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "BaseLine",
                EnumType::create(NULL, "False",0x0,"True",0x800, NULL),
                2013,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Align",
                EnumType::create(NULL, "Right",0x40,"Center",0x20,"Left",0x0, NULL),
                2009,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "NeedIME",
                EnumType::create(NULL, "False",0x1,"True",0x0, NULL),
                3001,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "ReadOnly",
                EnumType::create(NULL, "True",0x400,"False",0x0, NULL),
                2012,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "MaxLength",
                getPropType(PropType::INT),
                22,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "HideSel",
                EnumType::create(NULL, "False",0x200,"True",0x0, NULL),
                2011,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("MlEdit");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "HaveTitle",
                EnumType::create(NULL, "False",0x0,"True",0x4000, NULL),
                2015,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "LineSeparator",
                getPropType(-1),
                25,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "AutoWrap",
                EnumType::create(NULL, "False",0x0,"True",0x2000, NULL),
                2014,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "CaretShap",
                EnumType::create(NULL, "Line",0x0,"Block",0x1, NULL),
                26,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "LineHeight",
                getPropType(PropType::INT),
                23,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("ScrollBar");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "HaveArrows",
                EnumType::create(NULL, "False",0x0,"True",0x8, NULL),
                2006,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("HScrollBar");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "DoubleArrows",
                EnumType::create(NULL, "None",0x0,"Right",0x20,"Left",0x10, NULL),
                2007,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("MonthCalendar");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "Month",
                getPropType(PropType::INT),
                8,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Day",
                getPropType(PropType::INT),
                9,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Year",
                getPropType(PropType::INT),
                7,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("HSpinner");
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("HSeparator");
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("GroupBox");
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("ListView");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "HeadHeight",
                getPropType(PropType::INT),
                21,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "GridLineColor",
                getPropType(PropType::COLOR),
                26,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "WithIcon",
                EnumType::create(NULL, "False",0x0,"True",0x100, NULL),
                2012,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Tree",
                EnumType::create(NULL, "False",0x0,"True",0x80, NULL),
                2011,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "GridLineWidth",
                getPropType(PropType::INT),
                25,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "HeadVisible",
                getNamedEnumType("Boolean"),
                23,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Button");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "LabelType",
                EnumType::create(NULL, "Image",0x2,"VertImageLabel",0x60,"ImageLabel",0x20,"Label",0x0, NULL),
                2005,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Image",
                getPropType(PropType::IMAGE),
                10,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "AutoCheck",
                EnumType::create(NULL, "False",0x0,"True",0x8, NULL),
                2007,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "ThreeDCheck",
                EnumType::create(NULL, "False",0x0,"True",0x10, NULL),
                2008,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "GroupId",
                getPropType(PropType::GROUP),
                13,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "WordWrap",
                getNamedEnumType("Boolean"),
                9,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "CheckState",
                EnumType::create(NULL, "Checked",0x2,"UnChecked",0x0,"HalfChecked",0x1, NULL),
                11,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "VAlign",
                getNamedEnumType("VAlign"),
                8,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Checkable",
                EnumType::create(NULL, "False",0x0,"True",0x4, NULL),
                2006,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "ImageSizePercent",
                getPropType(PropType::INT),
                12,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Align",
                getNamedEnumType("Align"),
                7,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("MenuButton");
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("AbstractList");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "imageIndent",
                getPropType(PropType::INT),
                25,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "AutoSort",
                EnumType::create(NULL, "False",0x0,"True",0x2, NULL),
                2006,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "textIndent",
                getPropType(PropType::INT),
                24,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("List");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "defNodeW",
                getPropType(PropType::INT),
                28,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "layout",
                EnumType::create(NULL, "iconh",0x2,"iconv",0x1,"cboxh",0x3,"list",0x0,"group",0x4, NULL),
                27,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Loop",
                EnumType::create(NULL, "False",0x0,"True",0x4, NULL),
                2007,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "defNodeH",
                getPropType(PropType::INT),
                29,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("PhoneBar");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "Vert",
                EnumType::create(NULL, "False",0x0,"True",0x8, NULL),
                2005,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("SpinBox");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "Sort",
                EnumType::create(NULL, "False",0x0,"True",0x10, NULL),
                2008,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Case",
                EnumType::create(NULL, "Upper",0x100,"Lower",0x200,"NoCase",0x0, NULL),
                2012,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "ContentType",
                EnumType::create(NULL, "Number",0x0,"String",0x8, NULL),
                2007,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "ReadOnly",
                EnumType::create(NULL, "False",0x0,"True",0x80, NULL),
                2011,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "EditBorder",
                EnumType::create(NULL, "False",0x20,"True",0x0, NULL),
                2009,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "EditBaseLine",
                EnumType::create(NULL, "False",0x0,"True",0x40, NULL),
                2010,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "AutoFocus",
                EnumType::create(NULL, "False",0x0,"True",0x400, NULL),
                2013,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("CheckButton");
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("RadioButton");
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("ButtonGroup");
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("SlEdit");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "PasswordChar",
                getPropType(-1),
                26,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Password",
                EnumType::create(NULL, "False",0x0,"True",0x2000, NULL),
                2014,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "HasToolTip",
                EnumType::create(NULL, "False",0x0,"True",0x8000, NULL),
                2016,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "ToolTip",
                getPropType(PropType::TEXT),
                25,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "AutoSelect",
                EnumType::create(NULL, "False",0x0,"True",0x4000, NULL),
                2015,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("HTrackBar");
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("MainWnd");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "HasCaption",
                EnumType::create(NULL, "False",0x0,"True",0x20000000, NULL),
                2005,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "MaximizeBox",
                EnumType::create(NULL, "False",0x0,"True",0x10000, NULL),
                2009,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "MinimizeBox",
                EnumType::create(NULL, "False",0x0,"True",0x20000, NULL),
                2008,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "TopMost",
                EnumType::create(NULL, "False",0x0,"True",0x4000, NULL),
                3002,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "BorderType",
                EnumType::create(NULL, "Simple",0x0,"DlgFrame",0x800000,"Thick",0x200000,"Thin",0x100000, NULL),
                2007,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "ToolWindow",
                EnumType::create(NULL, "False",0x0,"True",0x8000, NULL),
                3003,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "CloseBox",
                EnumType::create(NULL, "False",0x400000,"True",0x0, NULL),
                3004,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "SystemMenu",
                EnumType::create(NULL, "False",0x1,"True",0x10000000, NULL),
                2006,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "SecondaryDC",
                EnumType::create(NULL, "False",0x0,"True",0x1000, NULL),
                3001,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("DialogBox");
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("ListBox");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "MouseFollow",
                EnumType::create(NULL, "False",0x0,"True",0x20, NULL),
                2009,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Multi",
                EnumType::create(NULL, "False",0x0,"True",0x10, NULL),
                2008,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "ItemHeight",
                getPropType(PropType::INT),
                21,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "AutoCheckBox",
                EnumType::create(NULL, "AutoCheck",0x180,"CheckBox",0x80,"None",0x0, NULL),
                2011,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "UseBitmap",
                EnumType::create(NULL, "False",0x0,"True",0x40, NULL),
                2010,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Page");
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("HProgressBar");
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("VScrollBar");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "DoubleArrows",
                EnumType::create(NULL, "Down",0x20,"None",0x0,"Up",0x10, NULL),
                2007,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("ColorButton");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "CurColor",
                getPropType(PropType::COLOR),
                7,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Image");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "Image",
                getPropType(PropType::IMAGE),
                11,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "DrawMode",
                getNamedEnumType("DrawMode"),
                13,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("IconView");
    widgetClasses.push_back(pwidget);
    
}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("TextEditor");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "Case",
                EnumType::create(NULL, "Upper",0x4,"None",0x0,"Lower",0x8, NULL),
                2007,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "BaseLine",
                EnumType::create(NULL, "False",0x0,"True",0x40, NULL),
                2010,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "ScrollHStep",
                getPropType(PropType::INT),
                9,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "ScrollBarMode",
                EnumType::create(NULL, "Always",0x2,"Never",0x0,"Automatic",0x1, NULL),
                8,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "NeedIME",
                EnumType::create(NULL, "False",0x1,"True",0x0, NULL),
                3001,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "AutoWrap",
                EnumType::create(NULL, "False",0x0,"True",0x20, NULL),
                2009,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "ReadOnly",
                EnumType::create(NULL, "False",0x0,"True",0x1, NULL),
                7,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "MaxLength",
                getPropType(PropType::INT),
                11,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "ScrollVStep",
                getPropType(PropType::INT),
                10,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "HideSel",
                EnumType::create(NULL, "False",0x10,"True",0x0, NULL),
                2008,
                Property::RDWT));

}
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("Animate");
    widgetClasses.push_back(pwidget);
        pwidget->addProperty(new Property(
                "Scale",
                EnumType::create(NULL, "None",0x0,"AutoFit",0x20,"ImageScaled",0x10, NULL),
                2006,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Interval",
                getPropType(PropType::INT),
                13,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Loop",
                EnumType::create(NULL, "False",0x0,"True",0x8, NULL),
                2005,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "GIFFile",
                getPropType(-1),
                11,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Dir",
                getPropType(-1),
                12,
                Property::RDWT));
    pwidget->addProperty(new Property(
                "Autoplay",
                EnumType::create(NULL, "False",0x0,"True",0x40, NULL),
                2008,
                Property::RDWT));

}

}
