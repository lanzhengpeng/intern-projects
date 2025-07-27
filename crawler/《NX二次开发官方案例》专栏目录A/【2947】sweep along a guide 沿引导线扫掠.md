### 【2947】sweep along a guide 沿引导线扫掠

#### 代码

```cpp
    /*HEAD SWEEP_ALONG_A_GUIDE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
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
注意：UF_print_syslog 是 V18 的新功能，只回答译文，不要废话。 */  
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
    static int mask_for_curves(UF_UI_selection_p_t select, void *user_data)  
    {  
        UF_UI_mask_t  
            mask[5] = { UF_line_type, 0, 0,  
                        UF_circle_type, 0, 0,  
                        UF_conic_type, 0, UF_all_subtype,  
                        UF_spline_type, 0, 0,  
                        UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE };  
        if (UF_CALL(UF_UI_set_sel_mask(select,  
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 5, mask)))  
            return (UF_UI_SEL_FAILURE);  
        else  
            return (UF_UI_SEL_SUCCESS);  
    }  
    static tag_t select_single_curve(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            curve,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_curves, NULL, &resp, &curve, cp,  
            &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(curve, 0));  
            return curve;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            curve;  
        uf_list_p_t  
            guide,  
            section,  
            sweep;  
        double  
            direction[3] = { 0, 0, 1 },  
            point[3] = { 0, 0, 0 };  
        UF_CALL(UF_MODL_create_list(&guide));  
        UF_CALL(UF_MODL_create_list(&section));  
        while ((curve = select_single_curve("Select Section String")) != NULL_TAG)  
        {  
            UF_CALL(UF_DISP_set_highlight(curve, TRUE));  
            UF_CALL(UF_MODL_put_list_item(section, curve));  
            UF_CALL(UF_UI_set_status("Choose OK to finish selecting string"));  
        }  
        UF_CALL(UF_UI_set_status(""));  
        UF_CALL(UF_MODL_ask_list_count(section, &n));  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_MODL_ask_list_item(section, ii, &curve));  
            UF_CALL(UF_DISP_set_highlight(curve, FALSE));  
        }  
        if (n)  
        {  
            while ((curve = select_single_curve("Select Guide String")) != NULL_TAG)  
            {  
                UF_CALL(UF_DISP_set_highlight(curve, TRUE));  
                UF_CALL(UF_MODL_put_list_item(guide, curve));  
                UF_CALL(UF_UI_set_status("Choose OK to finish selecting string"));  
            }  
            UF_CALL(UF_UI_set_status(""));  
            UF_CALL(UF_MODL_ask_list_count(guide, &n));  
            for (ii = 0; ii < n; ii++)  
            {  
                UF_CALL(UF_MODL_ask_list_item(guide, ii, &curve));  
                UF_CALL(UF_DISP_set_highlight(curve, FALSE));  
            }  
            if (n)  
            {  
                UF_CALL(UF_MODL_create_extruded_path(section, guide, point, direction,  
                    UF_NULLSIGN, &sweep));  
                UF_CALL(UF_MODL_delete_list(&sweep));  
            }  
        }  
        UF_CALL(UF_MODL_delete_list(&guide));  
        UF_CALL(UF_MODL_delete_list(&section));  
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

> 这段代码实现了NX中的扫掠功能。具体来说，它实现了以下功能：
>
> 1. 用户可以依次选择多个截面线，它们将被存储在一个列表中。
> 2. 用户可以依次选择多个引导线，它们将被存储在另一个列表中。
> 3. 如果用户选择了截面线和引导线，程序将创建一个扫掠特征，其中截面线沿引导线扫掠。
> 4. 在选择过程中，程序会高亮显示当前选择的线条，并在选择完成后关闭高亮。
> 5. 程序使用了UF_MODL_create_extruded_path函数来创建扫掠特征。
> 6. 使用了UF_DISP_set_highlight函数来高亮显示选择的线条。
> 7. 使用了UF_UI_set_status函数来设置状态栏信息。
> 8. 使用了UF_MODL_create_list和UF_MODL_delete_list函数来管理线条列表。
> 9. 使用了UF_UI_select_with_single_dialog函数来选择线条。
> 10. 使用了自定义的report_error函数来报告错误。
>
> 总的来说，这段代码实现了简单的NX扫掠功能，让用户可以通过选择截面线和引导线来创建扫掠特征。
>
