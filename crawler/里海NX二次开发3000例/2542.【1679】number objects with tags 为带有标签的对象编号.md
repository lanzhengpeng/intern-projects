### 【1679】number objects with tags 为带有标签的对象编号

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
    static void number_objects_with_tags(tag_t *objects, int n_objects)  
    {  
        logical  
            is_displayable;  
        int  
            ii;  
        double  
            loc[3];  
        char  
            msg[133];  
        for (ii = 0; ii < n_objects; ii++)  
        {  
            UF_CALL(UF_OBJ_is_displayable(objects[ii], &is_displayable));  
            if (!is_displayable) continue;  
            sprintf(msg, "%d - %d", ii+1, objects[ii]);  
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

> 这段代码是用于NX二次开发，主要包含两个函数：
>
> 1. display_temporary_text函数：该函数用于在工作视图中显示临时的文本信息。它接受文本内容和显示位置两个参数，并使用UF_DISP_display_temporary_text函数在指定位置显示文本。文本属性通过props结构体设置，例如字体、颜色等。
> 2. number_objects_with_tags函数：该函数用于给指定的对象数组编号，并显示编号。它遍历传入的对象数组，检查每个对象是否可显示，如果可显示，则生成编号文本，调用display_temporary_text函数在对象名称原点处显示编号。在显示编号前，会先给对象设置临时名称"X"，以获取正确的名称原点。
>
> 这两个函数结合使用，可以实现在NX中给模型对象编号并显示编号的功能。这对于二次开发中的对象管理和识别非常有用。
>
