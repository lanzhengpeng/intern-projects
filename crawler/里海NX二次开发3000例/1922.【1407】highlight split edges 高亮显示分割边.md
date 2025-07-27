### 【1407】highlight split edges 高亮显示分割边

#### 代码

```cpp
    /*HEAD HIGHLIGHT_SPLIT_EDGES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 版本中新增的函数。 */  
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
    static int mask_for_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_an_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_edges, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int ii;  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
    }  
    static void do_it(void)  
    {  
        int  
            n_edges;  
        tag_t  
            edge,  
            *edges;  
        char  
             msg[133];  
        while ((edge = select_an_edge("Highlight splits")) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_split_edges(edge, &n_edges, &edges));  
            if (n_edges > 0)  
            {  
                set_highlight_object_array(n_edges, edges, TRUE);  
                sprintf(msg, "%d split edges highlighted", n_edges);  
                uc1601(msg, TRUE);  
                set_highlight_object_array(n_edges, edges, FALSE);  
                UF_free(edges);  
            }  
            else  
                uc1601("That edge has no split edges", TRUE);  
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

> 根据代码内容，这是一段用于在NX中进行二次开发的代码，主要功能是高亮显示边界的分割线。
>
> 主要步骤包括：
>
> 1. 引入必要的NX二次开发库。
> 2. 定义了错误报告函数report_error，用于输出错误信息。
> 3. 定义了选择边界的函数select_an_edge，用于提示用户选择一个边界。
> 4. 定义了设置高亮显示的函数set_highlight_object_array，用于设置对象的高亮显示。
> 5. 定义了执行主逻辑的函数do_it，用于根据用户选择的边界，获取其分割线并高亮显示。
> 6. 定义了ufusr函数，用于初始化NX环境并执行主逻辑。
> 7. 定义了卸载函数ufusr_ask_unload。
>
> 综上，该代码实现了在NX中根据用户选择的边界，高亮显示其分割线的功能。
>
