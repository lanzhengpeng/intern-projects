### 【0311】ask object color name 询问对象的颜色名称

#### 代码

```cpp
    /*  Prior to NX 4.0, the returned full color name looked like this:  
        full_color_name = "2 (Green)"  
        So the method shown here cleans it up and returns just the name.  
    */  
    static void ask_color_name(int color_num, char *color_name)  
    {  
        double  
            clr_values[3];  
        char  
            *full_color_name;  
        UF_CALL(UF_DISP_ask_color(color_num, UF_DISP_rgb_model, &full_color_name,  
            clr_values));  
        if (strchr(full_color_name, '('))  
        {  
            strcpy(color_name, strchr(full_color_name, '(')+1);  
            strcpy(strchr(color_name, ')'), "");  
        }  
        else  
            strcpy(color_name, full_color_name);  
        UF_free(full_color_name);  
    }  
    static void ask_object_color_name(tag_t obj, char *color)  
    {  
        logical  
            is_disp;  
        UF_OBJ_disp_props_t  
            disp_props;  
        UF_CALL(UF_OBJ_is_displayable(obj, &is_disp));  
        if (is_disp)  
        {  
            UF_CALL(UF_OBJ_ask_display_properties(obj, &disp_props));  
            ask_color_name(disp_props.color, color);  
        }  
        else strcpy(color, "NOT DISPLAYABLE");  
    }

```

#### 代码解析

> 这段NX二次开发代码实现了以下功能：
>
> 1. ask_color_name函数：通过传入的颜色编号，查询该颜色的全名，并去除全名中的编号和括号，只返回颜色名称。
> 2. ask_object_color_name函数：通过传入的NX对象标签，判断该对象是否可显示。如果可显示，则查询该对象的显示属性，获取颜色编号，然后调用ask_color_name函数获取颜色名称；如果不可显示，则返回"NOT DISPLAYABLE"。
> 3. 代码注释：提供了详细的代码注释，解释了函数的作用和实现原理。
> 4. NX版本兼容性：考虑到了NX不同版本的兼容性，在NX 4.0之前，颜色全名包含编号和括号，因此进行了相应的处理。
> 5. 错误处理：通过调用UF_CALL宏，实现了NX API的调用错误处理。
> 6. 内存管理：在ask_color_name函数中，使用UF_free释放了颜色全名字符串占用的内存。
> 7. 函数命名：采用了有意义的函数命名，便于理解代码逻辑。
> 8. 代码结构：代码结构清晰，通过静态函数封装了查询颜色名称的逻辑，便于重用。
>
> 总体来说，这段代码实现了在NX中查询对象颜色名称的功能，考虑了兼容性、错误处理和内存管理，代码质量较高。
>
