-- lua minigui test
function dump_table(t, prefix)
    if not prefix then prefix = "" end
    for k,v in pairs(t) do
        if type(v) == 'table' then
            mg.log(prefix .. '{')
            dump_table(v, prefix .. '\t')
            mg.log(prefix .. '}')
        else
            mg.log(prefix .. tostring(k) .. '=' .. tostring(v))
        end
    end
end

dump_table(mg)

mg.log("lua test")

idx = 0;

function check_button_status_changed(nc, ckbtnPtr, id)
    mg.log(string.format("check button status nc=%d,checkbutton ptr:%d,id=%d", nc, ckbtnPtr, id))
    ckbtn = mg.wrap(ckbtnPtr)
    label = mg.findWndObject(ckbtnPtr, 200)
    if label then
        label.Text = ckbtn.Text .. " State Is:" .. tostring(ckbtn.CheckState)
    else
        mg.log("cannot get the sub window 200");
    end
end

function month_calendar_changed(nc, mcPtr, id)
    mg.log(string.format("MonthCalendar mcPtr=%d, nc=%d", mcPtr, nc))
    mc = mg.wrap(mcPtr)
    label = mg.findWndObject(mcPtr, 101)
    label.Text = string.format("Calendar:%d/%d/%d", mc.Year, mc.Month ,mc.Day)
end

function init_combobox(message, cbPtr)
    cb = mg.wrap(cbPtr)
    for i = 0,10,1 do
        mg.log(string.format("combobox item:%d",i));
        cb:addItem(string.format("combobox item:%d", i));
    end
end

function listview_remove_all(listview)
    colcount = listview:getColumnCount() - 1
    while colcount >= 0 do
        listview:delColumn(colcount)
        colcount = colcount - 1
    end

    listview:removeAll()
end

function scrollbar_value_changed(nc, scbPtr, id)
    mg.log(string.format("nc=%d,scb=%d,id=%d", nc, scbPtr, id))
    scbRed = mg.findWndObject(scbPtr, 100)
    scbGreen = mg.findWndObject(scbPtr, 101)
    scbBlue = mg.findWndObject(scbPtr, 102)
    rect = mg.findWndObject(scbPtr, 300)
    label = mg.findWndObject(scbPtr, 301)
    if not scbRed or not scbGreen or not scbBlue or not rect or not label then
        return
    end

    red = scbRed.CurPos
    green = scbGreen.CurPos
    blue = scbBlue.CurPos

    strRgb = string.format("(%d,%d,%d)", red, green, blue)
    mg.log("RGB:" .. strRgb)
    rect.FillColor = 0xff000000 + red + (green * 0xff) +  (blue * 0xffff)
    label.Text = strRgb
end

list_data1 = {
    {"Name", "Chinese", "Math", "English"},
    {"Tom", "70","65","56"},
    {"Jerry","98","97","100"},
    {"Jack", "80","89","85"},
    {"Mary", "98","100","100"},
    {"Tim", "99","98","100"},
    {"Jobs", "100","100","100"},
    {"Gates", "99","99","100"},
    {"Tom", "70","65","56"},
    {"Jerry","98","97","100"},
    {"Jack", "80","89","85"},
    {"Mary", "98","100","100"},
    {"Tim", "99","98","100"},
    {"Jobs", "100","100","100"},
    {"Gates", "99","99","100"},
    {"Tom", "70","65","56"},
    {"Jerry","98","97","100"},
    {"Jack", "80","89","85"},
    {"Mary", "98","100","100"},
    {"Tim", "99","98","100"},
    {"Jobs", "100","100","100"},
    {"Gates", "99","99","100"}
};

sub_tmpls = {
{
    X = 0,
    Y = 0,
    Width = 260,
    Height = 200,
    Visible = "True",
    HasCaption = "True",
    Border = "True",
    Text = "Button From Lua",
    onCreate = function(message, widgetPtr)
        mg.log("Main Window Created:" .. tostring(widgetPtr))
        return true;
    end,
    children = {
        {
            Class = "Button",
            id =100,
            X = 30,
            Y = 30,
            Width = 80,
            Height = 25,
            Border ="True",
            Text = "Button",
            onClicked = function(nc, widgetPtr, id)
                mg.log(string.format("button1 click:%d, nc=%d, id=%d", widgetPtr, nc, id))
                btn = mg.wrap(widgetPtr)
                mg.log("button wrap obj=" .. tostring(btn))
                mg.MessageBox(widgetPtr, "button1 click: Text=" .. btn.Text , "caption")
                btn.Text = "Button" .. tostring(idx)
                idx = idx + 1
            end
        },
        {
            Class = "Button",
            id =101,
            X = 150,
            Y = 30,
            Width = 80,
            Height = 25,
            Text = "Button2"
        }
    }
},
{
    X = 0,
    Y = 0,
    Width = 260,
    Height = 180,
    Visible = "True",
    HasCaption = "True",
    Border = "True",
    Text = "CheckButton From Lua",
    children = {
        {
            Class ="CheckButton",
            id =100,
            X = 20,
            Y = 30,
            Width = 120,
            Height = 25,
            Border = "False",
            Visible = "True",
            AutoCheck = "False",
            Text = "option1",
            onStateChanged = check_button_status_changed
        },
        {
            Class ="CheckButton",
            id =101,
            X = 20,
            Y = 60,
            Width = 120,
            Height = 25,
            AutoCheck = "True",
            Border = "False",
            Visible = "True",
            Text = "option2",
            onStateChanged = check_button_status_changed
        },
        {
            Class ="CheckButton",
            id =101,
            X = 20,
            Y = 100,
            Width = 120,
            Height = 25,
            AutoCheck = "True",
            ThreeDCheck = "True",
            Border = "True",
            Visible = "True",
            Text = "option3",
            onStateChanged = check_button_status_changed
        },
        {
            Class ="Static",
            id =200,
            X = 10,
            Y = 0,
            Width = 200,
            Height =25,
            Text ="No State"
        },
    }
},
{
    X = 0,
    Y = 0,
    Width = 260,
    Height = 200,
    Visible = "True",
    HasCaption = "True",
    Border = "True",
    Text = "RadioButton From Lua",
    children = {
        {
            Class ="ButtonGroup",
            id =200,
            X = 10,
            Y = 10,
            Width = 150,
            Height =140,
            Text ="Color",
            onRadioButtonChanged = function(nc, bgPtr, id, sel_id)
                rect = mg.findWndObject(bgPtr, 300)
                colors = {0xff0000ff, 0xff00ff00, 0xffff0000}
                mg.log(string.format("raido changed sel_id=%d, color=0x%x", sel_id, colors[sel_id-100+1]))
                rect.FillColor = colors[sel_id - 100 + 1]
            end
        },
        {
            Class ="RadioButton",
            id =100,
            X = 20,
            Y = 30,
            Width = 120,
            Height = 25,
            Border = "False",
            Visible = "True",
            Text = "Red",
            GroupId =200,
            CheckState ="Checked"
        },
        {
            Class ="RadioButton",
            id =101,
            X = 20,
            Y = 60,
            Width = 120,
            Height = 25,
            Border = "False",
            Visible = "True",
            Text = "Green",
            GroupId =200
        },
        {
            Class ="RadioButton",
            id =102,
            X = 20,
            Y = 100,
            Width = 120,
            Height = 25,
            Border = "True",
            Visible = "True",
            Text = "Blue",
            GroupId =200
        },
        {
            Class ="Rectangle",
            id =300,
            X = 180,
            Y = 10,
            Width = 60,
            Height = 60,
            FillColor = 0xff0000ff
        }
    }
},
{
    X = 0,
    Y = 0,
    Width = 240,
    Height = 200,
    Visible = "True",
    HasCaption = "True",
    Border = "True",
    Text = "Scrollbar From Lua",
    children = {
        {
            Class ="ScrollBar",
            id =100,
            X = 10,
            Y = 10,
            Width = 200,
            Height = 25,
            Border = "False",
            Visible = "True",
            MaxPos = 255,
            MinPos = 0,
            onPosChanged = scrollbar_value_changed
        },
        {
            Class ="ScrollBar",
            id =101,
            X = 10,
            Y = 40,
            Width = 200,
            Height = 25,
            Border = "False",
            Visible = "True",
            MaxPos = 255,
            MinPos = 0,
            onPosChanged = scrollbar_value_changed
        },
        {
            Class ="ScrollBar",
            id =102,
            X = 10,
            Y = 70,
            Width = 200,
            Height = 25,
            Border = "False",
            Visible = "True",
            MaxPos = 255,
            MinPos = 0,
            onPosChanged = scrollbar_value_changed
        },
        {
            Class ="Rectangle",
            id =300,
            X = 10,
            Y = 100,
            Width = 60,
            Height = 60,
            FillColor =0xff000000
        },
        {
            Class ="Static",
            id = 301,
            X = 100,
            Y = 100,
            Width = 60,
            Height =60,
            Text ="(0,0,0)"
        }
    }
},
{
    X = 0,
    Y = 0,
    Width = 240,
    Height = 200,
    Visible = "True",
    HasCaption = "True",
    Border = "True",
    Text = "TrackBar From Lua",
    children = {
        {
            Class ="TrackBar",
            id =100,
            X = 10,
            Y = 10,
            Width = 200,
            Height = 25,
            Border = "False",
            Visible = "True",
            MaxPos = 255,
            MinPos = 0,
            onPosChanged = scrollbar_value_changed
        },
        {
            Class ="TrackBar",
            id =101,
            X = 10,
            Y = 40,
            Width = 200,
            Height = 25,
            Border = "False",
            Visible = "True",
            MaxPos = 255,
            MinPos = 0,
            onPosChanged = scrollbar_value_changed
        },
        {
            Class ="TrackBar",
            id =102,
            X = 10,
            Y = 70,
            Width = 200,
            Height = 25,
            Border = "False",
            Visible = "True",
            MaxPos = 255,
            MinPos = 0,
            onPosChanged = scrollbar_value_changed
        },
        {
            Class ="Rectangle",
            id =300,
            X = 10,
            Y = 100,
            Width = 60,
            Height = 60,
            FillColor =0xff000000
        },
        {
            Class ="Static",
            id = 301,
            X = 100,
            Y = 100,
            Width = 60,
            Height =60,
            Text ="(0,0,0)"
        }
    }
},
{
    X = 0,
    Y = 0,
    Width = 260,
    Height = 180,
    Visible = "True",
    HasCaption = "True",
    Border = "True",
    Text = "ColorButton From Lua",
    children = {
        {
            Class ="ColorButton",
            id =100,
            X = 20,
            Y = 30,
            Width = 120,
            Height = 30,
            Border = "False",
            Visible = "True",
            onColorChanged = function(nc, cbtPtr, id)
                cbt = mg.wrap(cbtPtr)
                rect = mg.findWndObject(cbtPtr, 101)
                mg.log("new color:".. tostring(cbt.CurColor))
                rect.FillColor = cbt.CurColor
            end
        },
        {
            Class ="Rectangle",
            id =101,
            X = 20,
            Y = 80,
            Width = 60,
            Height = 60
        }
    }
},
{
    X = 0,
    Y = 0,
    Width = 260,
    Height = 300,
    Visible = "True",
    HasCaption = "True",
    Border = "True",
    Text = "MonthButton From Lua",
    children = {
        {
            Class ="MonthCalendar",
            id =100,
            X = 10,
            Y = 30,
            Width = 240,
            Height = 220,
            Border = "True",
            Visible = "True",
            onYearChanged = month_calendar_changed,
            onMonthChanged = month_calendar_changed,
            onDayChanged = month_calendar_changed
        },
        {
            Class ="Static",
            id =101,
            X = 10,
            Y = 0,
            Width = 240,
            Height = 25
        }
    }
},
{
    X = 0,
    Y = 0,
    Width = 240,
    Height = 150,
    Visible = "True",
    HasCaption = "True",
    Border = "True",
    Text = "Combobox From Lua",
    children = {
        {
            Class ="Combobox",
            id =100,
            X = 10,
            Y = 40,
            Width = 200,
            Height = 30,
            Border = "False",
            Visible = "True",
            Type = "DropDownList",
            DropDownHeight =80,
            ReadOnly ="True",
            onCreate = init_combobox,
            onSelChanged = function (nc, cbPtr, id)
                cb = mg.wrap(cbPtr)
                sel = cb:getCurSel()
                mg.log("Combobox current sel:" .. tostring(sel))
                label = mg.findWndObject(cb, 150)
                label.Text = string.format("CurrentSelect(%d):%s", sel, cb:getItem(sel))
            end
        },
        {
            Class ="Static",
            id =150,
            X = 10,
            Y = 0,
            Width = 240,
            Height = 25,
            Align ="Left"
        }
    }
},
{
    X = 0,
    Y = 0,
    Width = 240,
    Height = 150,
    Visible = "True",
    HasCaption = "True",
    Border = "True",
    Text = "Progress From Lua",
    children = {
        {
            Class ="ProgressBar",
            id =100,
            X = 10,
            Y = 10,
            Width = 180,
            Height = 30,
            Border = "False",
            Visible = "True",
            MinPos = 0,
            MaxPos = 100,
            LineStep = 10,
            ShowPercent ="True",
            BlockChunk ="True"
        },
        {
            Class ="Button",
            X = 10,
            Y = 50,
            Width = 80,
            Height = 25,
            Text ="Step up",
            onClicked = function(nc, btnPtr, id)
                pb = mg.findWndObject(btnPtr, 100)
                pb:increase(5)
            end
        },
        {
            Class ="Button",
            X = 100,
            Y = 50,
            Width = 80,
            Height = 25,
            Text ="Step Back",
            onClicked = function(nc, btnPtr, id)
                pb = mg.findWndObject(btnPtr, 100)
                pb:increase(-5)
            end
        },
        {
            Class ="Spinner",
            X = 190,
            Y = 10,
            Width = 30,
            Height =30,
            Visible ="True",
            MinPos = 0,
            MaxPos = 100,
            LineStep = 10,
            onPosChanged = function(nc, spPtr, id)
                sp = mg.wrap(spPtr)
                progbar = mg.findWndObject(spPtr, 100)
                progbar.CurPos = sp.CurPos
            end
        }
    }
},
{
    X = 0,
    Y = 0,
    Width = 260,
    Height = 400,
    Visible = "True",
    HasCaption = "True",
    Border = "True",
    Text = "ListView From Lua",
    children = {
        {
            Class ="ListView",
            id =100,
            X = 10,
            Y = 30,
            Width = 230,
            Height = 300,
            Border = "True"
        },
        {
            Class ="Button",
            X = 10,
            Y = 340,
            Width = 80,
            Height = 25,
            Text ="Init List Data",
            onClicked = function(nc, btnPtr, id)
                mg.log(string.format("Init list data: %d, id=%d", btnPtr, id))
                listview = mg.findWndObject(btnPtr, 100)
                mg.log("Init list data:" .. tostring(listview))
                listview_remove_all(listview)
                --add coloumn
                i = 0
                for k,v in pairs(list_data1[1]) do
                    txt = v
                    listview:addColumn(i, 100, txt)
                    mg.log(string.format("Add Column(%d):%s", i, txt))
                    i = i + 1
                end
                for i = 1,#list_data1-1,1 do
                    listview:addItem(i-1, 0, 0, 25, 0, list_data1[i+1])
                end
            end
        },
        {
            Class ="Button",
            X = 120,
            Y = 340,
            Width = 80,
            Height = 25,
            Text ="Init Tree Data",
            onClicked = function(nc, btnPtr, id)
                listview = mg.findWndObject(btnPtr, 100)
                mg.log("Init list data:" .. tostring(listview))
                listview_remove_all(listview)
                --add coloumn
                i = 0
                listview:addColumn(i, 20, "");
                for i = 1, #list_data1[1],1 do
                    txt = list_data1[1][i];
                    listview:addColumn(i, 100, txt);
                end
                classes = {"Grade1", "Grade2", "Grade3", "Grade4"}
                for i,v in pairs(classes) do
                    hitem = listview:addItem(i - 1, 0, 0, 25, 0, {"", classes[i]});
                    for j = 1, #list_data1 - 1, 1 do
                        listview:addItem(j, 0, hitem, 25, 0, list_data1[j+1])
                    end
                end
            end
        }
    }
}
}

listbox = nil

function openWindow(nc, widgetPtr, id)
    mg.log("openWindow")
    sel = listbox:getCurSel()
    mg.log("openWindow sel=" .. tostring(sel))
    if sel >= 0 and sel < #sub_tmpls then
        dump_table(sub_tmpls[sel+1], "")
        mg.DoModal(sub_tmpls[sel+1], listbox)
    else
        mg.MessageBox(listbox, "please select the window first!", "error", mg.MB_OK)
    end
end

main_tmpl = {
    X = 0,
    Y = 0,
    Width = 260,
    Height = 320,
    Visible = "True",
    HasCaption = "True",
    Border = "True",
    Text = "Main menu",

    children = {
        {
            Class = "ListBox",
            id = 100,
            X = 10,
            Y = 10,
            Width = 240,
            Height = 240,
            Border = "True",
            onCreate = function(message, widgetPtr)
                mg.log("onListBox create:" .. tostring(widgetPtr))
                listbox = mg.wrap(widgetPtr);
                mg.log("onListBox create wrap=" .. tostring(listbox))
                for _,tmpl  in pairs(sub_tmpls) do
                    mg.log("sub tmpl:" .. tmpl.Text)
                    listbox:addString(tmpl.Text);
                end
            end,
            onDBLClick = openWindow
        },
        {
            Class = "Button",
            id = 101,
            X = 10,
            Y = 260,
            Width = 80,
            Height = 25,
            Text = "Open",
            onClicked = openWindow
        }
    }
}

dump_table(main_tmpl)

return mg.CreateMainWindow(main_tmpl)

