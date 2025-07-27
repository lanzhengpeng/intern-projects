### 【2136】report gdt annotation tags 报告GDT注释标签

#### 代码

```cpp
    /*HEAD REPORT_GDT_ANNOTATION_TAGS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_gdt.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据上下文，我理解您的意思：UF_print_syslog 是 V18 中新增的功能。 */  
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
    static int mask_gdt(UF_UI_selection_p_t select, void *user_data)  
    {  
        UF_UI_mask_t  
            triples = { UF_tol_feature_instance_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &triples)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* qq3123197280 */  
    static tag_t select_a_gdt(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_gdt, NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int  
            inx = 0;  
        tag_t  
            instance;  
        UF_GDT_annotation_tags_t  
            tags;  
        while ((instance = select_a_gdt("Select a GDT feature")) != NULL_TAG)  
        {  
            WRITE_D(instance);  
            if (UF_CALL( UF_GDT_ask_annotation_tags ( instance, &tags )) ) break;  
            WRITE_D(tags.num_dimension_tags);  
            for (inx = 0; inx < tags.num_dimension_tags; inx++ )  
            {  
                WRITE_D(inx);  
                WRITE_D(tags.dimension_tags[inx]);  
            }  
            WRITE_D(tags.num_feature_control_frame_tags);  
            for (inx = 0; inx < tags.num_feature_control_frame_tags; inx++ )  
            {  
                WRITE_D(inx);  
                WRITE_D(tags.feature_control_frame_tags[inx]);  
            }  
            WRITE_D(tags.num_appended_text_tags);  
            for (inx = 0; inx < tags.num_appended_text_tags; inx++ )  
            {  
                WRITE_D(inx);  
                WRITE_D(tags.appended_text_tags[inx]);  
            }  
            WRITE_D(tags.num_non_dimensional_leaders);  
            UF_GDT_free( UF_GDT_ANNOTATION_TAGS_TYPE, &tags );  
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

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在发生错误时输出错误信息。
> 2. 定义了一个选择过滤器mask_gdt，用于在选择对话框中仅显示特征实例。
> 3. 定义了一个选择特征实例的函数select_a_gdt，用于提示用户选择一个特征实例，并返回其tag。
> 4. 定义了一个写入整数的函数write_integer_to_listing_window，用于将整数写入列表窗口。
> 5. 定义了一个主要执行函数do_it，用于循环提示用户选择特征实例，获取其注释标签，并输出到列表窗口。
> 6. 定义了ufusr函数，用于初始化、执行do_it函数，并在结束时进行清理。
> 7. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 整体来看，该代码通过获取用户选择的特征实例，查询其注释标签，并将结果输出到列表窗口，实现了对NX特征实例注释标签的查询和报告功能。
>
