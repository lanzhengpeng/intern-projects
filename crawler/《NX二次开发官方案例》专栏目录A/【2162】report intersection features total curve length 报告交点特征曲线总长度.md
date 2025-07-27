### 【2162】report intersection features total curve length 报告交点特征曲线总长度

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
    static double report_intersection_features_total_curve_length(tag_t feat)  
    {  
        int  
            n;  
        tag_t  
            *curves;  
        double  
            length = 0;  
        if (!UF_CALL(UF_CURVE_ask_feature_curves(feat, &n, &curves))  
            && (n > 0))  
        {  
            length = report_curve_array_total_length(n, curves);  
            UF_free(curves);  
        }  
        return length;  
    }

```

#### 代码解析

> 这段NX二次开发代码主要包含以下功能：
>
> 1. 定义了一个宏SHOW_DOUBLE，用于显示双精度浮点数的值。
> 2. 定义了一个函数report_a_double，用于报告双精度浮点数变量的值。
> 3. 定义了一个函数display_temporary_text，用于在指定位置显示临时文本。
> 4. 定义了一个函数flag_object，用于在指定对象的位置显示文本。
> 5. 定义了一个函数set_highlight_object_array，用于设置对象的高亮显示。
> 6. 定义了一个函数report_curve_array_total_length，用于计算曲线数组的总长度并显示每条曲线的长度。
> 7. 定义了一个函数report_intersection_features_total_curve_length，用于计算交线特征中所有曲线的总长度。
>
> 综合来看，这段代码主要实现了对NX模型中的曲线长度进行统计和显示的功能，包括对单个对象、曲线数组以及交线特征中的曲线进行长度计算和显示。通过使用宏、函数等封装，提高了代码的可复用性和可维护性。
>
