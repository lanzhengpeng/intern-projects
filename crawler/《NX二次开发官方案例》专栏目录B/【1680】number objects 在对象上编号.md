### 【1680】number objects 在对象上编号

#### 代码

```cpp
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void number_objects(tag_t *objects, int n_objects)  
    {  
        logical  
            is_displayable;  
        int  
            ii;  
        double  
            loc[3];  
        char  
            msg[133];  
        UF_DISP_refresh();  
        for (ii = 0; ii < n_objects; ii++)  
        {  
            UF_CALL(UF_OBJ_is_displayable(objects[ii], &is_displayable));  
            if (!is_displayable) continue;  
            sprintf(msg, "%d", ii+1);  
            if (UF_OBJ_ask_name_origin(objects[ii], loc))  
            {  
                UF_CALL(UF_OBJ_set_name(objects[ii], "X"));  
                UF_CALL(UF_OBJ_ask_name_origin(objects[ii], loc));  
                UF_CALL(UF_OBJ_delete_name(objects[ii]));  
            }  
            display_temporary_text(msg, loc);  
        }  
    }

```

#### 代码解析

> 这段代码实现了在NX中显示临时文本的功能。具体来说，代码包含了以下两个关键部分：
>
> 1. display_temporary_text函数：该函数用于在指定位置显示临时文本。它接受文本内容和位置坐标作为参数，并设置文本的显示属性，然后调用UF_DISP_display_temporary_text API在NX中显示文本。
> 2. number_objects函数：该函数用于给NX中的对象编号。它遍历传入的对象数组，检查每个对象是否可显示，并在可显示的对象的名称原点处调用display_temporary_text函数显示编号。
>
> 总体来说，这段代码通过二次开发的方式，实现了在NX中高效地给对象编号的功能，提高了NX软件的可交互性。通过显示临时文本的方式，避免了修改对象名称，保证了编号的即时性和易用性。
>
