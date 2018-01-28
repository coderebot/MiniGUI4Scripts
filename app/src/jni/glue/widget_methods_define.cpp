
#define DEF_CONST(V) \
	V("NCSF_ITEM_NORMAL", NCSF_ITEM_NORMAL) \
	V("NCSF_ITEM_SELECTED", NCSF_ITEM_SELECTED) \
	V("NCSF_ITEM_DISABLED", NCSF_ITEM_DISABLED) \
	V("NCSF_LSTITM_FOLD", NCSF_LSTITM_FOLD) \



BEGIN_IMPLEMENT_CLASS_METHOD(Combobox, addItem)

    DECLARE_PARAMTER(string, 0, item, "")

    DECLARE_RETURN(bool)
    ret = (bool)_c(self)->addItem(self, item.c_str(), 0);
    SET_RETURN(bool)
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(Combobox, removeItem)

    DECLARE_PARAMTER(int, 0, index, 0)

    DECLARE_RETURN(bool)
    ret = (bool)_c(self)->removeItem(self, index);
    SET_RETURN(bool)
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(Combobox, setItem)

    DECLARE_PARAMTER(int, 0, index, 0)

    DECLARE_PARAMTER(string, 1, item, "")

    DECLARE_RETURN(bool)
    ret = (bool)_c(self)->setItem(self, index, item.c_str());
    SET_RETURN(bool)
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(Combobox, getItem)

    DECLARE_PARAMTER(int, 0, index, 0)

    DECLARE_RETURN(string)
    ret = string(_c(self)->getItem(self, index));
    SET_RETURN(string)
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(Combobox, getCurSel)

    DECLARE_RETURN(int)
    ret = (int)(_c(self)->getProperty(self, NCSP_COMB_SELECT));
    SET_RETURN(int)
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(ProgressBar, increase)

    DECLARE_PARAMTER(int, 0, delta, 1)

    DECLARE_RETURN(int)
    ret = _c(self)->increase(self, delta);
    SET_RETURN(int)
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(ProgressBar, stepIncrease)

    DECLARE_RETURN(int)
    ret = _c(self)->stepIncrease(self);
    SET_RETURN(int)
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(ListView, addColumn)

    DECLARE_PARAMTER(int, 0, index, 0)

    DECLARE_PARAMTER(int, 1, width, 0)

    DECLARE_PARAMTER(string, 2, text, "")

    DECLARE_RETURN(bool)
    {
        NCS_LISTV_CLMINFO info;
        memset(&info, 0, sizeof(info));
        info.index=index;
        info.width=width;
        info.text=(char*)text.c_str();
        ret = (bool)_c(self)->addColumn(self, &info);
    }
    SET_RETURN(bool)
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(ListView, delColumn)

    DECLARE_PARAMTER(int, 0, index, 0)

    DECLARE_RETURN(bool)
    ret = (bool)_c(self)->delColumn(self, index);
    SET_RETURN(bool)
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(ListView, getColumnCount)

    DECLARE_RETURN(int)
    ret = _c(self)->getColumnCount(self);
    SET_RETURN(int)
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(ListView, addItem)

    DECLARE_PARAMTER(int, 0, index, 0)

    DECLARE_PARAMTER(int, 1, retPos, 0)

    DECLARE_PARAMTER(int, 2, parent, 0)

    DECLARE_PARAMTER(int, 3, height, 0)

    DECLARE_PARAMTER(int, 4, flags, 0)

    DECLARE_PARAMTER(ObjectWrap, 5, texts, (ObjectWrap*)NULL)

    DECLARE_RETURN(int)
    {
        NCS_LISTV_ITEMINFO info;
        NCS_LISTV_ITEMDATA *psubdata = NULL;
        memset(&info, 0, sizeof(info));
        info.index=index;
        info.parent=(HITEM)parent;
        info.retPos = retPos;
        vector<string> subitem_texts = texts.toArray<string>();
        if (subitem_texts.size() > 0) {
            psubdata = new NCS_LISTV_ITEMDATA[subitem_texts.size()];
            memset(psubdata, 0, sizeof(*psubdata)*subitem_texts.size());
            for (int i = 0; i < subitem_texts.size(); i++) {
                psubdata[i].row = index;
                psubdata[i].col = i;
                psubdata[i].text = (char*)subitem_texts[i].c_str();
            }
            info.dataSize = subitem_texts.size();
            info.data = psubdata;
        }
        info.flags=flags;
        ret = _c(self)->addItem(self, &info);
        if (psubdata) {
           delete[] psubdata;
        }
    }
    SET_RETURN(int)
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(ListView, setHeadText)

    DECLARE_PARAMTER(int, 0, colindex, 0)

    DECLARE_PARAMTER(string, 1, text, "")

    DECLARE_RETURN(bool)
    ret = (bool)(_c(self)->setHeadText(self, colindex, text.c_str()));
    SET_RETURN(bool)
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(ListView, getChildItem)

    DECLARE_PARAMTER(int, 0, parent, 0)

    DECLARE_PARAMTER(int, 1, index, 0)

    DECLARE_RETURN(int)
    ret = (int)(_c(self)->getChildItem(self, (HITEM)parent, index));
    SET_RETURN(int)
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(ListView, getChildCount)

    DECLARE_PARAMTER(int, 0, item, 0)

    DECLARE_RETURN(int)
    ret = _c(self)->getChildCount(self, (HITEM)item);
    SET_RETURN(int)
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(ListView, getItemText)

    DECLARE_PARAMTER(int, 0, row, 0)

    DECLARE_PARAMTER(int, 1, col, 0)

    DECLARE_RETURN(string)
    {
        NCS_LISTV_ITEMDATA ii;
        memset(&ii, 0, sizeof(ii));
        ii.row=row;
        ii.col=col;
        if(_c(self)->getItemInfo(self,&ii)){
            ret=ii.text;
        } else {
            ret="";
        }
    }
    SET_RETURN(string)
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(Edit, setContent)

    DECLARE_PARAMTER(string, 0, text, "")

    DECLARE_PARAMTER(int, 1, start, 0)

    DECLARE_PARAMTER(int, 2, len, -1)

    DECLARE_RETURN(int)
    ret = _c(self)->setContent(self, text.c_str(), start, len);
    SET_RETURN(int)
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(Edit, replace)

    DECLARE_PARAMTER(string, 0, text, "")

    DECLARE_PARAMTER(int, 1, start, 0)

    DECLARE_PARAMTER(int, 2, len, -1)

    DECLARE_PARAMTER(int, 3, replaceStart, 0)

    DECLARE_PARAMTER(int, 4, replaceEnd, -1)

    
    _c(self)->replaceText(self, text.c_str(), start, len, replaceStart, replaceEnd);
    
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(Edit, insert)

    DECLARE_PARAMTER(string, 0, text, "")

    DECLARE_PARAMTER(int, 1, start, 0)

    DECLARE_PARAMTER(int, 2, len, -1)

    DECLARE_PARAMTER(int, 3, insertStart, 0)

    
    _c(self)->insert(self, text.c_str(), start, len, insertStart);
    
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(Edit, append)

    DECLARE_PARAMTER(string, 0, text, "")

    DECLARE_PARAMTER(int, 1, start, 0)

    DECLARE_PARAMTER(int, 2, len, -1)

    
    _c(self)->append(self, text.c_str(), start, len);
    
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(Edit, setSel)

    DECLARE_PARAMTER(int, 0, start, 0)

    DECLARE_PARAMTER(int, 1, end, 0)

    DECLARE_RETURN(int)
    ret = _c(self)->setSel(self, start, end);
    SET_RETURN(int)
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(Edit, getSelStart)

    DECLARE_RETURN(int)
    int end; _c(self)->getSel(self, &ret, &end);
    SET_RETURN(int)
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(Edit, getSelEnd)

    DECLARE_RETURN(int)
    int start; _c(self)->getSel(self, &start, &ret);
    SET_RETURN(int)
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(Edit, copy)

    
    _c(self)->copy(self);
    
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(Edit, cut)

    
    _c(self)->cut(self);
    
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(Edit, paste)

    
    _c(self)->paste(self);
    
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(Edit, redo)

    
    _c(self)->redo(self);
    
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(Edit, undeo)

    
    _c(self)->undo(self);
    
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(ItemView, getCurSel)

    DECLARE_RETURN(int)
    ret = _c(self)->getCurSel(self);
    SET_RETURN(int)
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(ItemView, setCurSel)

    DECLARE_PARAMTER(int, 0, sel, 0)

    
    _c(self)->setCurSel(self, sel);
    
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(ItemView, removeAll)

    DECLARE_RETURN(bool)
    _c(self)->removeAll(self);
    SET_RETURN(bool)
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(ListBox, setSel)

    DECLARE_PARAMTER(int, 0, newSel, 0)

    DECLARE_PARAMTER(int, 1, flags, 0)

    DECLARE_RETURN(int)
    ret = _c(self)->setSel(self, newSel, flags);
    SET_RETURN(int)
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(ListBox, addString)

    DECLARE_PARAMTER(string, 0, text, "")

    DECLARE_RETURN(int)
    ret = _c(self)->addString(self, text.c_str(), 0);
    SET_RETURN(int)
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(ListBox, insertString)

    DECLARE_PARAMTER(string, 0, text, "")

    DECLARE_PARAMTER(int, 1, index, 0)

    DECLARE_RETURN(int)
    ret = _c(self)->insertString(self, text.c_str(), 0, index);
    SET_RETURN(int)
END_IMPLEMENT_CLASS_METHOD

BEGIN_IMPLEMENT_CLASS_METHOD(ListBox, findString)

    DECLARE_PARAMTER(int, 0, start, 0)

    DECLARE_PARAMTER(string, 1, text, "")

    DECLARE_PARAMTER(bool, 2, bExact, false)

    DECLARE_RETURN(int)
    ret = _c(self)->findString(self, start, (char*)text.c_str(), bExact);
    SET_RETURN(int)
END_IMPLEMENT_CLASS_METHOD


BEGIN_REGISTER_CLASSES

BEGIN_REGISTER_CLASS(Combobox)
	REGISTER_METHOD(addItem)
	REGISTER_METHOD(removeItem)
	REGISTER_METHOD(setItem)
	REGISTER_METHOD(getItem)
	REGISTER_METHOD(getCurSel)

END_REGISTER_CLASS(Combobox)

BEGIN_REGISTER_CLASS(ProgressBar)
	REGISTER_METHOD(increase)
	REGISTER_METHOD(stepIncrease)

END_REGISTER_CLASS(ProgressBar)

BEGIN_REGISTER_CLASS(ListView)
	REGISTER_METHOD(addColumn)
	REGISTER_METHOD(delColumn)
	REGISTER_METHOD(getColumnCount)
	REGISTER_METHOD(addItem)
	REGISTER_METHOD(setHeadText)
	REGISTER_METHOD(getChildItem)
	REGISTER_METHOD(getChildCount)
	REGISTER_METHOD(getItemText)

END_REGISTER_CLASS(ListView)

BEGIN_REGISTER_CLASS(Edit)
	REGISTER_METHOD(setContent)
	REGISTER_METHOD(replace)
	REGISTER_METHOD(insert)
	REGISTER_METHOD(append)
	REGISTER_METHOD(setSel)
	REGISTER_METHOD(getSelStart)
	REGISTER_METHOD(getSelEnd)
	REGISTER_METHOD(copy)
	REGISTER_METHOD(cut)
	REGISTER_METHOD(paste)
	REGISTER_METHOD(redo)
	REGISTER_METHOD(undeo)

END_REGISTER_CLASS(Edit)

BEGIN_REGISTER_CLASS(ItemView)
	REGISTER_METHOD(getCurSel)
	REGISTER_METHOD(setCurSel)
	REGISTER_METHOD(removeAll)

END_REGISTER_CLASS(ItemView)

BEGIN_REGISTER_CLASS(ListBox)
	REGISTER_METHOD(setSel)
	REGISTER_METHOD(addString)
	REGISTER_METHOD(insertString)
	REGISTER_METHOD(findString)

END_REGISTER_CLASS(ListBox)

END_REGISTER_CLASSES


