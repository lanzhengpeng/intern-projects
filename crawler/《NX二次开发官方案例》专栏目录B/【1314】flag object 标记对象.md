### 【1314】flag object 标记对象

#### 代码

```cpp
    #define SHOW_TAG(X) { char xm[20]; sprintf(xm,"%d",X); flag_object(X,xm); }  
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void flag_object(tag_t object, char *msg)  
    {  
        double  
            loc[3];  
        if (UF_OBJ_ask_name_origin(object, loc))  
        {  
            UF_CALL(UF_OBJ_set_name(object, "X"));  
            UF_CALL(UF_OBJ_ask_name_origin(object, loc));  
            UF_CALL(UF_OBJ_delete_name(object));  
        }  
        display_temporary_text(msg, loc);  
    }

```

#### 代码解析

> 这段代码是用于NX（Siemens NX）的二次开发，主要实现了两个功能：
>
> 1. SHOW_TAG宏：用于显示NX对象（如特征、边等）的tag值。它会调用flag_object函数，传递对象的tag值，并将tag值转换为字符串作为提示信息。
> 2. flag_object函数：用于在指定对象的位置显示一个临时的提示文本。首先，它会获取对象的原点位置，然后在该位置上显示传入的提示信息msg。在显示前，它会临时设置对象的名称为"X"，以获取正确的原点位置，并在显示后删除该名称。
> 3. display_temporary_text函数：用于在指定位置显示一个临时的文本。它调用UF_DISP_display_temporary_text函数，传入文本内容和位置信息，设置显示属性，从而实现文本的显示。
>
> 总的来说，这段代码的主要功能是在NX模型中，针对指定对象显示其tag值或自定义的提示信息，以便进行调试或提示。
>
