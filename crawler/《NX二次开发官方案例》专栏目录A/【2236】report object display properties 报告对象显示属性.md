### 【2236】report object display properties 报告对象显示属性

#### 代码

```cpp
    #define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \  
        else ECHO("%s = FALSE\n", #X);  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    static void report_object_display_properties(tag_t object)  
    {  
        UF_OBJ_disp_props_t  
            display_properties;  
        if (UF_CALL(UF_OBJ_ask_display_properties(object, &display_properties)))  
            return;  
        WRITE_D(display_properties.layer);  
        WRITE_D(display_properties.color);  
        WRITE_D(display_properties.blank_status);  
        WRITE_D(display_properties.line_width);  
        WRITE_D(display_properties.font);  
        WRITE_L(display_properties.highlight_status);  
    }

```

#### 代码解析

> 这是NX二次开发代码，用于报告对象显示属性。
>
> 主要功能包括：
>
> 1. 定义了两个宏WRITE_L和WRITE_D，分别用于输出布尔值和整数值的属性。
> 2. 定义了函数report_object_display_properties，用于报告传入对象的所有显示属性。
> 3. 使用UF_OBJ_ask_display_properties函数获取对象的显示属性。
> 4. 使用WRITE_D宏输出对象所在的层、颜色、是否空白、线宽、字体等属性。
> 5. 使用WRITE_L宏输出对象是否高亮显示的布尔值。
> 6. 通过调用UF_CALL宏检查UF_OBJ_ask_display_properties函数的返回值，以确定是否成功获取了显示属性。
>
> 总体来说，这段代码通过UF_OBJ_ask_display_properties函数获取对象的所有显示属性，并使用宏打印输出，实现了报告对象显示属性的功能。
>
