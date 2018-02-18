idx = 0;

function check_button_status_changed(nc, ckbtnPtr, id) {
    print("check button status nc="+nc+",checkbutton ptr:"+ckbtnPtr+",id="+id);
    ckbtn = mg.wrap(ckbtnPtr);
    label = mg.findWndObject(ckbtnPtr, 200);
    if (label) {
        label.Text = ckbtn.Text + " State Is:" + ckbtn.CheckState;
    } else {
        print("cannot get the sub window 200");
    }
}

function month_calendar_changed(nc, mcPtr, id) {
    print("MonthCalendar mcPtr="+mcPtr+",nc="+nc);
    mc = mg.wrap(mcPtr);
    label = mg.findWndObject(mcPtr, 101);
    label.Text = "Calendar:"+mc.Year + "/" + mc.Month + "/" + mc.Day;
}

function init_combobox(message, cbPtr) {
    cb = mg.wrap(cbPtr);
    for(var i = 0; i < 10; i++) {
        print("combobox item"+i);
        cb.addItem("combobox item" + i);
    }
}

function listview_remove_all(listview) {
    for(var colcount = listview.getColumnCount() - 1;
            colcount >= 0; colcount --) {
        listview.delColumn(colcount);
    }

    listview.removeAll();
}

function scrollbar_value_changed(nc, scbPtr, id) {
    print("nc="+nc+",scb="+scbPtr+",id="+id);
    scbRed = mg.findWndObject(scbPtr, 100);
    scbGreen = mg.findWndObject(scbPtr, 101);
    scbBlue = mg.findWndObject(scbPtr, 102);
    rect = mg.findWndObject(scbPtr, 300);
    label = mg.findWndObject(scbPtr, 301);

    red = scbRed.CurPos;
    green = scbGreen.CurPos;
    blue = scbBlue.CurPos;

    print("(" + red + "," + green + "," + blue + ")");
    rect.FillColor = 0xff000000 | red | (green << 8) | (blue<<16);
    label.Text = "(" + red + "," + green + "," + blue + ")";
}

list_data1 = [
    ["Name", "Chinese", "Math", "English"],
    ["Tom", "70","65","56"],
    ["Jerry","98","97","100"],
    ["Jack", "80","89","85"],
    ["Mary", "98","100","100"],
    ["Tim", "99","98","100"],
    ["Jobs", "100","100","100"],
    ["Gates", "99","99","100"],
    ["Tom", "70","65","56"],
    ["Jerry","98","97","100"],
    ["Jack", "80","89","85"],
    ["Mary", "98","100","100"],
    ["Tim", "99","98","100"],
    ["Jobs", "100","100","100"],
    ["Gates", "99","99","100"],
    ["Tom", "70","65","56"],
    ["Jerry","98","97","100"],
    ["Jack", "80","89","85"],
    ["Mary", "98","100","100"],
    ["Tim", "99","98","100"],
    ["Jobs", "100","100","100"],
    ["Gates", "99","99","100"]
];

sub_tmpls = [
{
    "X" : 0,
    "Y" : 0,
    "Width" : 260,
    "Height" : 200,
    "Visible": "True",
    "HasCaption": "True",
    "Border": "True",
    "Text": "Button From Js",
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
},
{
    "X" : 0,
    "Y" : 0,
    "Width" : 260,
    "Height" : 180,
    "Visible": "True",
    "HasCaption": "True",
    "Border": "True",
    "Text": "CheckButton From JS",
    "children": [
        {
            "Class":"CheckButton",
            "id":100,
            "X": 20,
            "Y": 30,
            "Width": 120,
            "Height": 25,
            "Border": "False",
            "Visible": "True",
            "AutoCheck": "False",
            "Text": "option1",
            "onStateChanged": check_button_status_changed
        },
        {
            "Class":"CheckButton",
            "id":101,
            "X": 20,
            "Y": 60,
            "Width": 120,
            "Height": 25,
            "AutoCheck": "True",
            "Border": "False",
            "Visible": "True",
            "Text": "option2",
            "onStateChanged": check_button_status_changed
        },
        {
            "Class":"CheckButton",
            "id":101,
            "X": 20,
            "Y": 100,
            "Width": 120,
            "Height": 25,
            "AutoCheck": "True",
            "ThreeDCheck": "True",
            "Border": "True",
            "Visible": "True",
            "Text": "option3",
            "onStateChanged": check_button_status_changed
        },
        {
            "Class":"Static",
            "id":200,
            "X": 10,
            "Y": 0,
            "Width": 200,
            "Height":25,
            "Text":"No State"
        },
    ]
},
{
    "X" : 0,
    "Y" : 0,
    "Width" : 260,
    "Height" : 200,
    "Visible": "True",
    "HasCaption": "True",
    "Border": "True",
    "Text": "RadioButton From JS",
    "children": [
        {
            "Class":"ButtonGroup",
            "id":200,
            "X": 10,
            "Y": 10,
            "Width": 150,
            "Height":140,
            "Text":"Color",
            "onRadioButtonChanged": function(nc, bgPtr, id, sel_id) {
                rect = mg.findWndObject(bgPtr, 300);
                colors = [0xff0000ff, 0xff00ff00, 0xffff0000];
                rect.FillColor = colors[sel_id - 100];
            }
        },
        {
            "Class":"RadioButton",
            "id":100,
            "X": 20,
            "Y": 30,
            "Width": 120,
            "Height": 25,
            "Border": "False",
            "Visible": "True",
            "Text": "Red",
            "GroupId":200,
            "CheckState":"Checked"
        },
        {
            "Class":"RadioButton",
            "id":101,
            "X": 20,
            "Y": 60,
            "Width": 120,
            "Height": 25,
            "Border": "False",
            "Visible": "True",
            "Text": "Green",
            "GroupId":200
        },
        {
            "Class":"RadioButton",
            "id":102,
            "X": 20,
            "Y": 100,
            "Width": 120,
            "Height": 25,
            "Border": "True",
            "Visible": "True",
            "Text": "Blue",
            "GroupId":200
        },
        {
            "Class":"Rectangle",
            "id":300,
            "X" : 180,
            "Y" : 10,
            "Width": 60,
            "Height": 60,
            "FillColor":0xff0000ff
        }
    ]
},
{
    "X" : 0,
    "Y" : 0,
    "Width" : 240,
    "Height" : 200,
    "Visible": "True",
    "HasCaption": "True",
    "Border": "True",
    "Text": "Scrollbar From JS",
    "children": [
        {
            "Class":"ScrollBar",
            "id":100,
            "X": 10,
            "Y": 10,
            "Width": 200,
            "Height": 25,
            "Border": "False",
            "Visible": "True",
            "MaxPos": 255,
            "MinPos": 0,
            "onPosChanged": scrollbar_value_changed
        },
        {
            "Class":"ScrollBar",
            "id":101,
            "X": 10,
            "Y": 40,
            "Width": 200,
            "Height": 25,
            "Border": "False",
            "Visible": "True",
            "MaxPos": 255,
            "MinPos": 0,
            "onPosChanged": scrollbar_value_changed
        },
        {
            "Class":"ScrollBar",
            "id":102,
            "X": 10,
            "Y": 70,
            "Width": 200,
            "Height": 25,
            "Border": "False",
            "Visible": "True",
            "MaxPos": 255,
            "MinPos": 0,
            "onPosChanged": scrollbar_value_changed
        },
        {
            "Class":"Rectangle",
            "id":300,
            "X" : 10,
            "Y" : 100,
            "Width": 60,
            "Height": 60,
            "FillColor":0xff000000
        },
        {
            "Class":"Static",
            "id": 301,
            "X" : 100,
            "Y" : 100,
            "Width": 60,
            "Height":60,
            "Text":"(0,0,0)"
        }
    ]
},
{
    "X" : 0,
    "Y" : 0,
    "Width" : 240,
    "Height" : 200,
    "Visible": "True",
    "HasCaption": "True",
    "Border": "True",
    "Text": "TrackBar From JS",
    "children": [
        {
            "Class":"TrackBar",
            "id":100,
            "X": 10,
            "Y": 10,
            "Width": 200,
            "Height": 25,
            "Border": "False",
            "Visible": "True",
            "MaxPos": 255,
            "MinPos": 0,
            "onPosChanged": scrollbar_value_changed
        },
        {
            "Class":"TrackBar",
            "id":101,
            "X": 10,
            "Y": 40,
            "Width": 200,
            "Height": 25,
            "Border": "False",
            "Visible": "True",
            "MaxPos": 255,
            "MinPos": 0,
            "onPosChanged": scrollbar_value_changed
        },
        {
            "Class":"TrackBar",
            "id":102,
            "X": 10,
            "Y": 70,
            "Width": 200,
            "Height": 25,
            "Border": "False",
            "Visible": "True",
            "MaxPos": 255,
            "MinPos": 0,
            "onPosChanged": scrollbar_value_changed
        },
        {
            "Class":"Rectangle",
            "id":300,
            "X" : 10,
            "Y" : 100,
            "Width": 60,
            "Height": 60,
            "FillColor":0xff000000
        },
        {
            "Class":"Static",
            "id": 301,
            "X" : 100,
            "Y" : 100,
            "Width": 60,
            "Height":60,
            "Text":"(0,0,0)"
        }
    ]
},
{
    "X" : 0,
    "Y" : 0,
    "Width" : 260,
    "Height" : 180,
    "Visible": "True",
    "HasCaption": "True",
    "Border": "True",
    "Text": "ColorButton From JS",
    "children": [
        {
            "Class":"ColorButton",
            "id":100,
            "X": 20,
            "Y": 30,
            "Width": 120,
            "Height": 30,
            "Border": "False",
            "Visible": "True",
            "onColorChanged": function(nc, cbtPtr, id) {
                cbt = mg.wrap(cbtPtr);
                rect = mg.findWndObject(cbtPtr, 101);
                rect.FillColor = cbt.CurColor;
            }
        },
        {
            "Class":"Rectangle",
            "id":101,
            "X" : 20,
            "Y" : 80,
            "Width": 60,
            "Height": 60
        }
    ]
},
{
    "X" : 0,
    "Y" : 0,
    "Width" : 260,
    "Height" : 300,
    "Visible": "True",
    "HasCaption": "True",
    "Border": "True",
    "Text": "MonthButton From JS",
    "children": [
        {
            "Class":"MonthCalendar",
            "id":100,
            "X": 10,
            "Y": 30,
            "Width": 240,
            "Height": 220,
            "Border": "True",
            "Visible": "True",
            "onYearChanged" : month_calendar_changed,
            "onMonthChanged" : month_calendar_changed,
            "onDayChanged" : month_calendar_changed
        },
        {
            "Class":"Static",
            "id":101,
            "X" : 10,
            "Y" : 0,
            "Width": 240,
            "Height": 25
        }
    ]
},
{
    "X" : 0,
    "Y" : 0,
    "Width" : 240,
    "Height" : 150,
    "Visible": "True",
    "HasCaption": "True",
    "Border": "True",
    "Text": "Combobox From JS",
    "children": [
        {
            "Class":"Combobox",
            "id":100,
            "X": 10,
            "Y": 40,
            "Width": 200,
            "Height": 30,
            "Border": "False",
            "Visible": "True",
            "Type": "DropDownList",
            "DropDownHeight":80,
            "ReadOnly":"True",
            "onCreate": init_combobox,
            "onSelChanged": function (nc, cbPtr, id) {
                cb = mg.wrap(cbPtr);
                var sel = cb.getCurSel();
                print("Combobox current sel:"+sel);
                label = mg.findWndObject(cb, 150);
                label.Text = "CurrentSelect("+sel+"):"+cb.getItem(sel);
            }
        },
        {
            "Class":"Static",
            "id":150,
            "X" : 10,
            "Y" : 0,
            "Width": 240,
            "Height": 25,
            "Align":"Left"
        }
    ]
},
{
    "X" : 0,
    "Y" : 0,
    "Width" : 240,
    "Height" : 150,
    "Visible": "True",
    "HasCaption": "True",
    "Border": "True",
    "Text": "Progress From JS",
    "children": [
        {
            "Class":"ProgressBar",
            "id":100,
            "X": 10,
            "Y": 10,
            "Width": 180,
            "Height": 30,
            "Border": "False",
            "Visible": "True",
            "MinPos": 0,
            "MaxPos": 100,
            "LineStep": 10,
            "ShowPercent":"True",
            "BlockChunk":"True"
        },
        {
            "Class":"Button",
            "X": 10,
            "Y": 50,
            "Width": 80,
            "Height": 25,
            "Text":"Step up",
            "onClicked": function(nc, btnPtr, id) {
                pb = mg.findWndObject(btnPtr, 100);
                pb.increase(5);
            }
        },
        {
            "Class":"Button",
            "X": 100,
            "Y": 50,
            "Width": 80,
            "Height": 25,
            "Text":"Step Back",
            "onClicked": function(nc, btnPtr, id) {
                pb = mg.findWndObject(btnPtr, 100);
                pb.increase(-5);
            }
        },
        {
            "Class":"Spinner",
            "X": 190,
            "Y": 10,
            "Width": 30,
            "Height":30,
            "Visible":"True",
            "MinPos": 0,
            "MaxPos": 100,
            "LineStep": 10,
            "onPosChanged": function(nc, spPtr, id) {
                sp = mg.wrap(spPtr);
                progbar = mg.findWndObject(spPtr, 100);
                print(sp+","+progbar+", pos="+sp.CurPos);
                progbar.CurPos = sp.CurPos;
            }
        }
    ]
},
{
    "X" : 0,
    "Y" : 0,
    "Width" : 260,
    "Height" : 400,
    "Visible": "True",
    "HasCaption": "True",
    "Border": "True",
    "Text": "ListView From JS",
    "children": [
        {
            "Class":"ListView",
            "id":100,
            "X": 10,
            "Y": 30,
            "Width": 230,
            "Height": 300,
            "Border": "True"
        },
        {
            "Class":"Button",
            "X": 10,
            "Y": 340,
            "Width": 80,
            "Height": 25,
            "Text":"Init List Data",
            "onClicked": function(nc, btnPtr, id) {
                print("Init list data:"+btnPtr+",id="+id);
                var listview = mg.findWndObject(btnPtr, 100);
                print("Init list data:"+listview);
                listview_remove_all(listview);
                //add coloumn
                for(var i in list_data1[0]) {
                    var txt = list_data1[0][i];
                    listview.addColumn(i, 100, txt);
                    print("Add Column(" + i + "):" + txt);
                }
                for (var i = 1; i < list_data1.length; i++) {
                    listview.addItem(i-1, 0, 0, 25, 0, list_data1[i])
                }
            }
        },
        {
            "Class":"Button",
            "X": 120,
            "Y": 340,
            "Width": 80,
            "Height": 25,
            "Text":"Init Tree Data",
            "onClicked": function(nc, btnPtr, id) {
                var listview = mg.findWndObject(btnPtr, 100);
                print("Init list data:"+listview);
                listview_remove_all(listview);
                //add coloumn
                listview.addColumn(i, 20, "");
                for(var i = 0; i <  list_data1[0].length; i ++) {
                    var txt = list_data1[0][i];
                    listview.addColumn(i+1, 100, txt);
                }
                classes = ["Grade1", "Grade2", "Grade3", "Grade4"];
                for (var i in classes) {
                    hitem = listview.addItem(i, 0, 0, 25, 0, ["", classes[i]]);
                    for (var j = 1; j < list_data1.length; j++) {
                        listview.addItem(j, 0, hitem, 25, 0, list_data1[j])
                    }
                }
            }
        }
    ]
}
];

var listbox;

function openWindow() {
    var sel = listbox.getCurSel();
    print("sel=" + sel);
    if (sel >= 0 && sel < sub_tmpls.length) {
        print(sub_tmpls[sel]);
        mg.DoModal(listbox, sub_tmpls[sel]);
    } else {
        mg.MessageBox(listbox, "please select the window first!", "Error", mg.MB_OK);
    }
}

main_tmpl = {
    "X" : 0,
    "Y" : 0,
    "Width" : 260,
    "Height" : 320,
    "Visible": "True",
    "HasCaption": "True",
    "Border": "True",
    "Text": "Main menu",

    "children": [
        {
            "Class": "ListBox",
            "id": 100,
            "X" : 10,
            "Y" : 10,
            "Width": 240,
            "Height": 240,
            "Border": "True",
            "onCreate": function(message, widgetPtr) {
                print("onListBox create:" + widgetPtr);
                listbox = mg.wrap(widgetPtr);
                for (var i in sub_tmpls) {
                    var tmpl = sub_tmpls[i];
                    print(tmpl.Text);
                    listbox.addString(tmpl.Text);
                }
            },
            "onDBLClick" : function(nc, widgetPtr, id) {
                openWindow();
            }
        },
        {
            "Class": "Button",
            "id": 101,
            "X" : 10,
            "Y" : 260,
            "Width": 80,
            "Height": 25,
            "Text": "Open",
            "onClicked": function(nc, widgetPtr, id) {
                openWindow();
            }
        }
    ]
};



print("I in v8");

return mg.CreateMainWindow(main_tmpl);

