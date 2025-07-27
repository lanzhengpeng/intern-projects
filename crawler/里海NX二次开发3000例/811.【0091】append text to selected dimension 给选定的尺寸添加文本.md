### 【0091】append text to selected dimension 给选定的尺寸添加文本

#### 代码

```cpp
    /*HEAD APPEND_TEXT_TO_SELECTED_DIMENSION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，V18版本中新增了一个名为UF_print_syslog的函数，用于打印系统日志。 */  
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
    static int mask_for_dimensions(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_dimension_type, UF_all_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_dimension(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a dimension", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_dimensions, NULL, &resp,  
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
            dim;  
        char  
            *the_text[1] = { "appended after" };  
        UF_DRF_appended_text_t  
            appended_text[1] = { UF_DRF_APPENDED_TEXT_AFTER, 1, NULL };  
        appended_text[0].text = the_text;  
        while ((dim = select_a_dimension("Append text")) != NULL_TAG)  
            UF_CALL(UF_DRF_set_appended_text(dim, 1, appended_text));  
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

> 这是段用于NX二次开发的UFUNC代码，其主要功能是在选中的尺寸上追加文本。
>
> 主要功能包括：
>
> 1. 包含了所需的头文件。
> 2. 定义了错误报告函数report_error，用于输出错误信息。
> 3. 定义了mask_for_dimensions函数，用于设置选择过滤，只选择尺寸类型。
> 4. 定义了select_a_dimension函数，用于通过对话框选择一个尺寸。
> 5. 定义了do_it函数，用于在选中的尺寸后追加文本。
> 6. 定义了ufusr函数，作为主函数，初始化NX系统，执行do_it函数，然后终止NX系统。
> 7. 定义了ufusr_ask_unload函数，用于卸载UFUNC。
>
> 代码通过定义选择过滤，实现了选择尺寸并追加文本的功能。
>
