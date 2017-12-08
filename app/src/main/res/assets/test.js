
idx = 0;

tmpl = {
    "X" : 0,
    "Y" : 0,
    "Width" : 300,
    "Height" : 200,
    "Visible": "True",
    "HasCaption": "True",
    "Border": "True",
    "Text": "Hello From Js",
    "onCreate": function(message, widgetPtr) {
        print("Main Window Created:" + widgetPtr);
        return true;
    },
    "children": [
        {
            "Class": "Button",
            "id":100,
            "X" : 30,
            "Y" : 30,
            "Width" : 80,
            "Height" : 25,
            "Border":"True",
            "Text": "Button",
            "onClicked" : function(nc, widgetPtr, id) {
                print("button1 click:"+ widgetPtr+",nc="+nc+",id="+id);
                btn = mg.wrap(widgetPtr);
                print("button wrap obj="+btn);
                mg.MessageBox(widgetPtr, "button1 click: Text=" + btn.Text , "caption");
                btn.Text = "Button" + idx;
                idx ++;
            }
        },
        {
            "Class": "Button",
            "id":101,
            "X" : 150,
            "Y" : 30,
            "Width" : 80,
            "Height" : 25,
            "Text": "Button2"
        }
    ]
};


print("I in v8");
print("template:"+tmpl);
print("mg="+mg);
print("mg.createMainWindow="+mg.createMainWindow);

return mg.createMainWindow(tmpl);

