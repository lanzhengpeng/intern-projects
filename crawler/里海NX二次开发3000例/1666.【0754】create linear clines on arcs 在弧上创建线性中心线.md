### 【0754】create linear clines on arcs 在弧上创建线性中心线

#### 代码

```cpp
    /*HEAD CREATE_LINEAR_CLINES_ON_ARCS CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：UF_print_syslog在V18中是新增的。

只回答译文，不要废话。 */  
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
    static void do_it(void)  
    {  
        tag_t  
            cline;  
        UF_DRF_object_t  
            arc;  
        UF_CALL(UF_DRF_init_object_structure(&arc));  
        arc.object_assoc_type = UF_DRF_arc_center;  
        while (select_drf_arc("Select arc for centerline", &arc))  
            UF_CALL(UF_DRF_create_linear_cline(1, &arc, &cline));  
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

> 这段代码是一个NX二次开发示例，其主要功能是创建一个圆弧的中心线。代码主要包含以下几个部分：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于在UF函数调用失败时输出错误信息。
> 2. 选择遮罩函数：定义了一个选择遮罩函数mask_for_drf_arcs，用于在创建中心线时仅选择圆弧。
> 3. 选择圆弧函数：定义了一个选择圆弧函数select_drf_arc，用于提示用户选择一个圆弧，并返回选择结果。
> 4. 创建中心线函数：定义了一个主函数do_it，用于循环调用选择圆弧函数，并在选择到圆弧后调用UF函数创建该圆弧的中心线。
> 5. UF初始化和终止函数：在ufusr函数中调用UF初始化和终止函数，以确保正确初始化和清理NX环境。
> 6. 立即卸载函数：定义了一个立即卸载函数ufusr_ask_unload，用于在二次开发模块卸载时立即清理资源。
>
> 总的来说，这段代码通过定义几个函数，实现了创建圆弧中心线的功能，并包含了错误处理和选择遮罩等细节处理。
>
