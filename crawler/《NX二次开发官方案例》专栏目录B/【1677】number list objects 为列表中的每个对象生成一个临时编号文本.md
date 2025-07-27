### 【1677】number list objects 为列表中的每个对象生成一个临时编号文本

#### 代码

```cpp
    static void display_temporary_colored_text(char *text, double loc[3], int color)  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        props.color = color;  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_ACTIVE_MINUS,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void number_list_objects(uf_list_p_t objects)  
    {  
        int  
            ii = 0;  
        double  
            loc[3];  
        char  
            msg[133];  
        uf_list_p_t  
            tmp;  
        for (tmp = objects; tmp != NULL; tmp = tmp->next)  
        {  
            if (UF_OBJ_ask_name_origin(tmp->eid, loc))  
            {  
                UF_CALL(UF_OBJ_set_name(tmp->eid, "X"));  
                UF_CALL(UF_OBJ_ask_name_origin(tmp->eid, loc));  
                UF_CALL(UF_OBJ_delete_name(tmp->eid));  
            }  
            sprintf(msg, "%d", ++ii);  
            display_temporary_colored_text(msg, loc, UF_OBJ_BLUE);  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API的示例，用于在NX中显示临时文字。代码中包含两个函数：
>
> 1. display_temporary_colored_text- 用于在指定位置显示指定颜色的临时文字。它使用UF_DISP_display_temporary_text函数，其中参数包括文字内容、位置、对齐方式、文字属性等。文字属性包括是否加亮、颜色、是否填充、线宽、字体类型等。
> 2. number_list_objects- 用于对列表中的每个对象编号，并在对象位置显示编号。它遍历对象列表，使用UF_OBJ_ask_name_origin获取对象名称原点位置，然后调用display_temporary_colored_text在该位置显示编号。编号从1开始递增，显示为蓝色文字。
>
> 通过调用这两个函数，可以实现对NX中对象的编号显示，用于快速识别和选择对象。这段代码展示了NX二次开发中如何操作对象、获取位置信息以及显示文字等常见操作。
>
