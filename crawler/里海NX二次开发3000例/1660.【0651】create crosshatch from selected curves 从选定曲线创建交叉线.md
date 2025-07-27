### 【0651】create crosshatch from selected curves 从选定曲线创建交叉线

#### 代码

```cpp
    /*Create crosshatch from selected curves*/  
    /*  
    This example lets the user select a set of drawing curves.   
    Then a crosshatch will be created in the area enclosed by the curves.   
    The curves should be continous and picked in order (or selected all at once with a rectangle).   
    Picking the curves not in order or forming a non-enclosed area gives different results.  
    */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_object_types.h>  
    #include <uf_view.h>  
    #include <uf_drf.h>  
    #include <uf_ui.h>  
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
            /* NOTE:  UF_print_syslog is new in V18 译:翻译：请注意，UF_print_syslog是在V18版本中新增的。 */  
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
    static int mask_for_drf_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[9] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE },  
                        { UF_section_line_type, 0, 0 },  
                        { UF_section_segment_type, 0, 0 },  
                        { UF_solid_silhouette_type, 0, 0 },  
                        { UF_section_edge_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 9, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static void do_it(void)  
    {  
        tag_t   
            view_tag,  
            xhatch,  
            *curves;  
        int  
            i,  
            resp,  
            cnt,  
            num_obj_bnd[1];  
        char   
            view_name[33];  
        /*Let the user select a set of curves.*/  
        UF_CALL(UF_UI_select_with_class_dialog("Select drawing curves", "", UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_drf_curves, NULL, &resp, &cnt, &curves));  
        if (resp == UF_UI_OK && cnt > 0)  
        {  
            for (i=0; i<cnt; i++)  
                UF_DISP_set_highlight(curves[i], 0);  
            /*Get the view name*/  
            UF_CALL(uc1653(view_name));  
            /*Get the view tag*/  
            UF_CALL(UF_VIEW_ask_tag_of_view_name(view_name, &view_tag));  
            /*Set number of curves for the single boundary*/  
            num_obj_bnd[0] = cnt;  
            /*Create a crosshatch within the set of curves*/  
            UF_CALL(UF_DRF_create_crosshatch(1, num_obj_bnd, curves, view_tag, &xhatch));  
            UF_free(curves);  
        }  
    }  
    /* 里海 */  
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

> 这段代码主要用于在NX中创建曲线交叉线。具体来说，它实现了以下功能：
>
> 1. 提供了一个错误报告函数report_error，用于在函数调用失败时记录错误信息。
> 2. 定义了一个选择掩码mask_for_drf_curves，用于在用户选择曲线时限制只选择绘图曲线。
> 3. do_it函数是主要逻辑。首先提示用户选择一系列绘图曲线，然后获取当前视图名称和视图标签，接着创建交叉线，并将曲线数组释放。
> 4. ufusr函数初始化NX并调用do_it函数，在结束时关闭NX。
> 5. ufusr_ask_unload函数用于卸载动态库。
>
> 总的来说，这段代码实现了根据用户选择的连续闭合曲线创建交叉线的功能。其主要逻辑是选择曲线、获取视图信息、创建交叉线，并通过错误报告函数处理异常。
>
