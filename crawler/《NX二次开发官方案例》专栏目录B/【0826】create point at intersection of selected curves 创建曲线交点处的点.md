### 【0826】create point at intersection of selected curves 创建曲线交点处的点

#### 代码

```cpp
    /*HEAD CREATE_POINT_AT_INTERSECTION_OF_SELECTED_CURVES CCC UFUN */  
    #include <stdio.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本新增的函数。 */  
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
    /* 里海 */  
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    extern tag_t select_a_curve(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            curve1,  
            curve2,  
            point;  
        while (((curve1 = select_a_curve(  
            "Intersection point - Select first object")) != NULL_TAG) &&  
               ((curve2 = select_a_curve(  
            "Intersection point - Select second object")) != NULL_TAG))  
        {  
            UF_CALL(UF_SO_create_point_two_curves(curve1,  
                UF_SO_update_within_modeling, curve1, curve2, NULL_TAG, NULL_TAG,  
                &point));  
            UF_CALL(UF_SO_set_visibility_option(point, UF_SO_visible));  
        }  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是创建两个选定曲线的交点。
>
> 具体步骤如下：
>
> 1. 包含必要的NX API头文件。
> 2. 定义错误报告函数report_error，用于输出错误信息。
> 3. 定义曲线选择遮罩mask_for_curves，用于限定选择类型为直线、圆、圆锥曲线和样条线。
> 4. 定义选择曲线函数select_a_curve，用于通过对话框选择一个曲线对象。
> 5. 在do_it函数中，通过循环调用select_a_curve函数，让用户选择两个曲线对象curve1和curve2。
> 6. 使用NX API函数UF_SO_create_point_two_curves，根据curve1和curve2创建交点point。
> 7. 设置point的可见性。
> 8. 在ufusr函数中，初始化NX系统，调用do_it函数执行创建交点操作，然后终止NX系统。
> 9. 定义卸载函数ufusr_ask_unload，返回立即卸载。
>
> 总体来说，该代码通过对话框让用户选择两个曲线对象，然后创建并显示这两个曲线的交点，实现了一个简单的二次开发示例。
>
