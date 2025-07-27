### 【0853】create radius dimension 创建半径尺寸

#### 代码

```cpp
    /*HEAD CREATE_RADIUS_DIMENSION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，我理解的是：

UF_print_syslog 是一个新功能，在 V18 版本中首次出现。它主要用于打印系统日志，提供了一种新的方式来记录系统日志信息。 */  
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
    static int mask_for_drf_arcs(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = {{ UF_circle_type, 0, 0 },  
                       {UF_solid_type, 0, UF_UI_SEL_FEATURE_CIRCULAR_EDGE },  
                       { UF_section_edge_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static logical select_drf_arc(char *prompt, UF_DRF_object_p_t arc)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_drf_arcs, NULL, &resp,  
            &arc->object_tag, cp, &arc->object_view_tag));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(arc->object_tag, 0));  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static logical select_pos(char *prompt, double pos[3])  
    {  
        int  
            resp = uc1615(prompt, pos);  
        if (resp == 5) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        UF_DRF_object_t  
            curve;  
        UF_DRF_text_t  
            text = { "", 0, NULL };  
        double  
            org[3];  
        tag_t  
            dim;  
        while ( (select_drf_arc("Select arc", &curve)) &&  
                (select_pos("Indicate dimension origin", org)))  
        {  
            UF_CALL(UF_DRF_create_radius_dim(&curve, &text, org, &dim));  
        }  
    }  
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

> 根据这段NX二次开发代码，可以总结如下：
>
> 1. 该代码的主要功能是在NX中创建半径尺寸。
> 2. 首先包含必要的NX API头文件。
> 3. 定义了一个错误报告函数report_error，用于打印错误信息。
> 4. 定义了一个选择DRF弧的函数select_drf_arc，用于让用户选择一个DRF弧。
> 5. 定义了一个选择位置的函数select_pos，用于让用户指定一个位置。
> 6. 定义了一个主函数do_it，在该函数中，用户可以循环选择DRF弧和位置，然后创建一个半径尺寸。
> 7. 定义了UFusr函数，作为NX的启动入口函数。在该函数中，初始化NX环境，调用do_it函数，然后终止NX环境。
> 8. 定义了ufusr_ask_unload函数，用于卸载该代码。
> 9. 代码整体结构清晰，注释详细，是一个很好的NX二次开发示例。
>
