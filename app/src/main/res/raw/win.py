import mg

idx = 0

def check_button_status_changed(nc, ckbtnPtr, btid, addData):
    mg.log("check button status nc=%d,checkbutton ptr: %d,id=%d"%(nc, ckbtnPtr, btid))
    ckbtn = mg.wrap(ckbtnPtr)
    label = mg.findWndObject(ckbtnPtr, 200)
    if label:
        label.Text = ckbtn.Text + "State Is:" + str(ckbtn.CheckState)
    else:
        mg.log("cannot get the sub window 200")

def month_calendar_changed(nc, mcPtr, cid, addData):
    mc = mg.wrap(mcPtr)
    label = mg.findWndObject(mcPtr, 101)
    label.Text = "Calendar:%d/%d/%d"%(mc.Year,mc.Month,mc.Day)

def on_button_clicked(nc, widgetPtr, cid, addData):
    global idx
    btn = mg.wrap(widgetPtr)
    mg.MessageBox(widgetPtr, "button1 click: Text=%s"%btn.Text, "Caption")
    btn.Text = "Button:%d"%idx
    idx = idx + 1

def init_combobox(message, cbPtr):
    cb = mg.wrap(cbPtr)
    for i in range(0,10):
        cb.addItem("combobox item :%d"% i)

def scrollbar_value_changed(nc, scbPtr, sid, addData):
    scbRed = mg.findWndObject(scbPtr, 100)
    scbGreen = mg.findWndObject(scbPtr, 101)
    scbBlue = mg.findWndObject(scbPtr, 102)
    rect = mg.findWndObject(scbPtr, 300)
    label = mg.findWndObject(scbPtr, 301)
    red = scbRed.CurPos
    green = scbGreen.CurPos
    blue = scbBlue.CurPos
    text = "(%d,%d,%d)" %(red, green, blue)
    mg.log(text)
    rect.FillColor = 0xff000000|red|(green<<8)|(blue<<16)
    label.Text = text

def on_radio_button_changed(nc,bgPtr, rid, sel_id):
    rect = mg.findWndObject(bgPtr, 300)
    colors = [0xff0000ff, 0xff00ff00, 0xffff0000]
    rect.FillColor = colors[sel_id - 100]

def on_color_changed(nc, cbtPtr, cid, addData):
    cbt = mg.wrap(cbtPtr)
    rect = mg.findWndObject(cbtPtr, 101)
    rect.FillColor = cbt.CurColor

def on_combobox_sel_changed(nc, cbPtr, cbid, addData):
    cb = mg.wrap(cbPtr)
    sel = cb.getCurSel()
    label = mg.findWndObject(cbPtr, 150)
    label.Text = "CurrentSelect(%d):%s"%(sel, cb.getItem(sel))

def on_button_pb_increase(nc, btnPtr, bid, addData):
    pb = mg.findWndObject(btnPtr, 100)
    pb.increase(5)

def on_button_pb_decrease(nc, btnPtr, bid, addData):
    pb = mg.findWndObject(btnPtr, 100)
    pb.increase(-5)

def on_spinner_changed(nc, spPtr, bid, addData):
    sp = mg.wrap(spPtr)
    progbar = mg.findWndObject(spPtr, 100)
    progbar.CurPos = sp.CurPos


def listview_remove_all(listview):
    colcount = listview.getColumnCount()-1
    while colcount >= 0:
        listview.delColumn(colcount)
        colcount = colcount - 1
    listview.removeAll()

def on_init_list_data(nc, btnPtr, bid, addData):
    listview = mg.findWndObject(btnPtr, 100)
    listview_remove_all(listview)
    i = 0
    for d in list_data1[0]:
        listview.addColumn(i, 100, d)
        i = i + 1
    i = 0
    for d in list_data1[1:]:
        listview.addItem(i, 0, 0, 25, 0, d)

def on_init_tree_data(nc, btnPtr, bid, addData):
    listview = mg.findWndObject(btnPtr, 100)
    listview_remove_all(listview)
    i = 1
    listview.addColumn(0, 20, "")
    for d in list_data1[0]:
        listview.addColumn(i, 100, d)
    classes = ["Grade1", "Grade2", "Grade3", "Grade4"]
    i = 0
    for d in classes:
        hitem = listview.addItem(i, 0, 0, 25, 0, ["", classes[i]])
        j = 0
        for s in list_data1[1:]:
            listview.addItem(j, 0, hitem, 25, 0, s)

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
]

sub_tmpls = [
{
    "X" : 0,
    "Y" : 0,
    "Width" : 260,
    "Height" : 200,
    "Visible": "True",
    "HasCaption": "True",
    "Border": "True",
    "Text": "Button From Python",
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
            "onClicked" : on_button_clicked
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
    "Text": "CheckButton From Python",
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
    "Text": "RadioButton From Python",
    "children": [
        {
            "Class":"ButtonGroup",
            "id":200,
            "X": 10,
            "Y": 10,
            "Width": 150,
            "Height":140,
            "Text":"Color",
            "onRadioButtonChanged": on_radio_button_changed
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
    "Text": "Scrollbar From Python",
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
    "Text": "TrackBar From Python",
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
    "Text": "ColorButton From Python",
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
            "onColorChanged": on_color_changed
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
    "Text": "MonthButton From Python",
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
    "Text": "Combobox From Python",
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
            "onSelChanged": on_combobox_sel_changed
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
    "Text": "Progress From Python",
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
            "onClicked": on_button_pb_increase
        },
        {
            "Class":"Button",
            "X": 100,
            "Y": 50,
            "Width": 80,
            "Height": 25,
            "Text":"Step Back",
            "onClicked": on_button_pb_decrease
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
            "onPosChanged": on_spinner_changed
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
    "Text": "ListView From Python",
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
            "onClicked": on_init_list_data
        },
        {
            "Class":"Button",
            "X": 120,
            "Y": 340,
            "Width": 80,
            "Height": 25,
            "Text":"Init Tree Data",
            "onClicked": on_init_tree_data
        }
    ]
}
]

listbox = None
def on_open_window(nc, widgetPtr, cid, addData):
    global listbox
    try:
        mg.log("on_open_window="+str(listbox))
        sel = listbox.getCurSel()
        mg.log("on_open_window="+str(sel))
        if sel >= 0 and sel < len(sub_tmpls):
            mg.log("open window:"+str(sub_tmpls[sel]))
            mg.DoModel(sub_tmpls[sel], widgetPtr)
        else:
            mg.MessageBox(widgetPtr, "please select the window first!", "Error", mg.MB_OK)
    except Exception, e:
        mg.log("on_open_window="+e.message)

def on_listbox_create(message, widgetPtr):
    global listbox
    listbox = mg.wrap(widgetPtr)
    mg.log("on_listbox_create:" + str(listbox))
    mg.log("sub_tmpls="+str(sub_tmpls))
    for t in sub_tmpls:
        try:
            listbox.addString(t["Text"])
        except Exception, e:
            mg.log("call listbox error="+e.message)


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
            "onCreate": on_listbox_create,
            "onDBLClick": on_open_window
        },
        {
            "Class": "Button",
            "id": 101,
            "X" : 10,
            "Y" : 260,
            "Width": 80,
            "Height": 25,
            "Text": "Open",
            "onClicked": on_open_window
        }
    ]
}

mg.log(str(main_tmpl))

mg.CreateMainWindow(main_tmpl)

