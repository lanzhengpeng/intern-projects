### 【1684】number string list objects 对字符串列表对象进行编号

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
    static void number_string_list_objects(UF_STRING_t *string)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        double  
            loc[3];  
        char  
            msg[133];  
        for (ii = 0; ii < string->num; ii++)  
        {  
            for (jj = 0; jj < string->string[ii]; jj++)  
            {  
                sprintf(msg, "%d", kk+1);  
                if (UF_OBJ_ask_name_origin(string->id[kk], loc))  
                {  
                    UF_CALL(UF_OBJ_set_name(string->id[kk], "X"));  
                    UF_CALL(UF_OBJ_ask_name_origin(string->id[kk], loc));  
                    UF_CALL(UF_OBJ_delete_name(string->id[kk]));  
                }  
                display_temporary_colored_text(msg, loc, UF_OBJ_WHITE);  
                kk++;  
            }  
        }  
    }

```

#### 代码解析

> 这段NX二次开发代码主要实现了以下功能：
>
> 1. display_temporary_colored_text函数：用于在NX中显示一个临时彩色文本。它接受文本内容、位置和颜色作为参数，并调用NX API来显示文本。
> 2. number_string_list_objects函数：遍历一个字符串数组，为数组中的每个字符串对应的NX对象编号。它会根据对象在数组中的顺序为其生成编号，并将编号以文本形式显示在对象的位置上。编号从1开始递增。
> 3. 在number_string_list_objects函数中，首先遍历字符串数组，然后遍历每个字符串中的每个字符。对于每个字符，使用sprintf生成编号字符串，然后调用NX API获取字符对应的NX对象的位置，并在该位置上显示编号文本。
> 4. 为了避免编号与对象已有名称冲突，在显示编号之前，会先删除对象的原有名称，显示完编号后再恢复原名称。
> 5. 该代码通过遍历字符串数组并对数组中的每个字符串对应的NX对象进行编号和显示编号，实现了对NX对象列表的编号显示功能。
>
