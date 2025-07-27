### 【0907】create smart point on selected arc angle 在选定的弧上创建智能点，用于标注角度

#### 代码

```cpp
    /*HEAD CREATE_SMART_POINT_ON_SELECTED_ARC_ANGLE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_so.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog是V18版本新增的。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    /* qq3123197280 */  
    static int mask_for_arcs(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_circle_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_CIRCULAR_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_an_arc(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_arcs, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            arc,  
            scalar,  
            smart_point,  
            xform = NULL_TAG;  
        double  
            angle = 0,  
            x_dir[3] = { 1, 0, 0 },  
            y_dir[3] = { 0, 1, 0 },  
            zero[3] = { 0, 0, 0 };  
        while (((arc = select_an_arc("Select arc")) != NULL_TAG) &&  
                (prompt_for_a_number("Enter angle", "degrees", &angle)))  
        {  
            UF_CALL(UF_SO_create_scalar_double(arc, UF_SO_update_after_modeling,  
                (angle * DEGRA), &scalar));  
        /*  Work around for PR 1081354 - xform is NOT optional 里海译:对 PR 1081354 的解决方案 - xform 不是可选的 */  
            UF_CALL(UF_SO_create_xform_doubles(arc, UF_SO_update_after_modeling,  
                zero, x_dir, y_dir, 1.0, &xform));  
            UF_CALL(UF_SO_create_point_on_arc_angle(arc,  
                UF_SO_update_after_modeling, arc, scalar, xform, &smart_point));  
            UF_CALL(UF_SO_set_visibility_option(smart_point, UF_SO_visible));  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码的主要功能是在NX中创建一个智能点，该智能点位于选择弧的指定角度位置。
>
> 主要步骤如下：
>
> 1. 包含所需的NX API头文件。
> 2. 定义一个错误报告函数，用于在函数调用失败时打印错误信息。
> 3. 定义一个函数，用于设置弧的选择掩码。
> 4. 定义一个函数，用于提示用户选择一个弧。
> 5. 定义一个函数，用于提示用户输入一个角度值。
> 6. 定义一个主函数，用于实现创建智能点的功能。循环选择弧，提示用户输入角度。使用UF_SO_create_scalar_double()函数创建一个标量，表示角度值。使用UF_SO_create_xform_doubles()函数创建一个变换，用于指定弧的坐标系。使用UF_SO_create_point_on_arc_angle()函数在弧的指定角度创建一个智能点。设置智能点的可见性。
> 7. 循环选择弧，提示用户输入角度。
> 8. 使用UF_SO_create_scalar_double()函数创建一个标量，表示角度值。
> 9. 使用UF_SO_create_xform_doubles()函数创建一个变换，用于指定弧的坐标系。
> 10. 使用UF_SO_create_point_on_arc_angle()函数在弧的指定角度创建一个智能点。
> 11. 设置智能点的可见性。
> 12. 定义ufusr()函数，用于初始化NX API并调用主函数。
> 13. 定义ufusr_ask_unload()函数，用于卸载用户自定义函数。
>
> 整体来说，这段代码实现了选择弧、输入角度，然后在弧的指定角度位置创建智能点的功能。
>
