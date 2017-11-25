from parsewidgets  import readWidgets
import os,sys

#usage genv8code path out_file.cpp

widget_dirs = sys.argv[1]
out_file = sys.argv[2]

widgets = readWidgets(widget_dirs)

PropDefineTemplate = '''
    props.push_back(new Property(
                "%s(prop_name)s",
                %s(prop_type)s,
                %(prop_id)d,
                %(prop_access)d));
'''


WidgetClassTemplate = '''
{
    WidgetClassDefine * pwidget = new WidgetClassDefine("%s(className)s");        
    widgetClasses.push_back(pwidget);
    vector<Property*> props = pwidget->GetProperties();
    %s(prop_defines)s
}
'''

FrameworkTemplate = '''
static void init_widget_class(vector<WidgetClassDefine*> & widgetClasses) {
    %(widget_classes)s 
}
'''

