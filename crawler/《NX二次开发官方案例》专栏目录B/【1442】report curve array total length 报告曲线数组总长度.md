### 【1442】report curve array total length 报告曲线数组总长度

#### 代码

```cpp
    #define SHOW_DOUBLE(X)    (report_a_double(#X, X))  
    static void report_a_double(char *variable, double value)  
    {  
        char  
            msg[133];  
        sprintf(msg, "%s = %f\n", variable, value);  
        uc1601(msg, TRUE);  
    }  
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
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int  
            ii,  
            subtype,  
            type;  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(objects[ii], &type, &subtype));  
            if (type == UF_occ_instance_type)  
                UF_CALL(UF_DISP_set_highlight(UF_ASSEM_ask_part_occ_of_inst(  
                    NULL_TAG, objects[ii]), flag));  
            else  
                UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
        }  
    }  
    static double report_curve_array_total_length(int n_curves, tag_t *curves)  
    {  
        int  
            ii;  
        double  
            length,  
            total_length = 0;  
        char  
            msg[133];  
        for (ii = 0; ii < n_curves; ii++)  
        {  
            UF_CALL(UF_CURVE_ask_arc_length(curves[ii], 0.0, 1.0,  
                UF_MODL_UNITS_PART, &length));  
            sprintf(msg, "%f", length);  
            flag_object(curves[ii], msg);  
            total_length = total_length + length;  
        }  
        set_highlight_object_array(n_curves, curves, TRUE);  
        SHOW_DOUBLE(total_length);  
        set_highlight_object_array(n_curves, curves, FALSE);  
        return total_length;  
    }

```

#### 代码解析

> 根据提供的NX二次开发代码，可以总结如下：
>
> 1. 定义了一个宏SHOW_DOUBLE，用于报告一个double类型变量的值。
> 2. 定义了report_a_double函数，用于报告double类型变量的值。
> 3. 定义了display_temporary_text函数，用于在模型中显示临时文字。
> 4. 定义了flag_object函数，用于在模型对象上标记文字。
> 5. 定义了set_highlight_object_array函数，用于设置模型对象的高亮显示。
> 6. 定义了report_curve_array_total_length函数，用于计算并报告曲线数组的总长度，并在每个曲线上标记长度，最后设置曲线高亮显示。
> 7. 整体代码实现了对模型对象进行标记、显示临时文字、计算曲线长度以及设置高亮显示等功能。
> 8. 代码使用了NX提供的UF对象和显示相关的API，是典型的NX二次开发代码。
> 9. 该代码片段应该是某个NX二次开发程序中的一部分，用于实现某些特定的功能。
>
> 综上所述，这段代码是NX二次开发代码，实现了对模型对象的处理和显示功能。
>
