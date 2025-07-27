### 【2154】report if selected sheet is offset midsurface 如果选中的板是偏移中面

#### 代码

```cpp
    /*HEAD REPORT_IF_SELECTED_SHEET_IS_OFFSET_MIDSURFACE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_sf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数。文档要求只翻译内容，不需要添加任何其他无关的评论。 */  
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
    static int filter_body_type(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            body_type;  
        UF_CALL(UF_MODL_ask_body_type(object, &body_type));  
        if (body_type == *(int *)data)  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    static int mask_body_type( UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_body_type, NULL, type)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_sheet(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            sheet = UF_MODL_SHEET_BODY,  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( "Select a sheet", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &sheet, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        logical  
            is_midsurface;  
        tag_t  
            sheet;  
        while ((sheet = select_a_sheet("Midsurface?")) != NULL_TAG)  
        {  
        /*  In V18.0.5.2 the return is non-zero even when the function works  
            so don't UF_CALL this function - this does not happen in UG NX */  
            UF_SF_is_offset_midsrf(sheet, &is_midsurface);  
            WRITE_L(is_midsurface);  
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

> 根据代码内容，这是一段NX Open C++ API编写的NX二次开发代码，主要功能如下：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用失败时报告错误信息。
> 2. 定义了一个筛选函数filter_body_type，用于在交互式选择时只允许选择实体类型为SHEET_BODY的物体。
> 3. 定义了一个设置选择器掩码的函数mask_body_type，用于在交互式选择时启用实体类型筛选。
> 4. 定义了一个选择一个薄板的函数select_a_sheet，用于弹出一个对话框，让用户选择一个薄板。
> 5. 定义了一个写逻辑值到列表窗口的函数write_logical_to_listing_window。
> 6. 定义了一个主函数do_it，用于循环调用select_a_sheet选择薄板，并判断每个薄板是否是中面薄板，如果是则将标志写入列表窗口。
> 7. 定义了ufusr函数，作为程序的入口，初始化NX Open环境，调用do_it执行主逻辑，然后终止NX Open环境。
> 8. 定义了ufusr_ask_unload函数，用于设置程序卸载模式为立即卸载。
>
> 综上，这段代码的主要功能是让用户选择薄板，然后判断每个薄板是否是中面薄板，如果是则输出提示。使用NX Open C++ API实现交互式选择和模型查询。
>
