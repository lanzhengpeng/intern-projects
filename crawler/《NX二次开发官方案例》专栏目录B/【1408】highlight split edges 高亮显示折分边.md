### 【1408】highlight split edges 高亮显示折分边

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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 版本中的新功能，用于打印系统日志信息。

翻译为：
注意：UF_print_syslog 是 V18 的新功能，只回答译文，不要添加废话。 */  
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

> 这段代码是NX Open C++ API编写的二次开发代码，实现了以下功能：
>
> 1. 包含了UF基本模块和UI模块的头文件，用于调用NX的API函数。
> 2. 定义了report_error函数，用于打印函数调用失败的错误信息。
> 3. 定义了mask_for_edges函数，设置了选择过滤条件，只允许选择边线。
> 4. 定义了select_an_edge函数，用于弹出选择对话框，让用户选择一条边线。
> 5. 定义了set_highlight_object_array函数，用于设置对象的高亮显示。
> 6. 定义了do_it函数，循环让用户选择边线，然后调用UF_MODL_ask_split_edges函数获取该边线的所有分割边线，并高亮显示分割边线，然后取消高亮。
> 7. 定义了ufusr函数，在NX启动时初始化并调用do_it函数，在NX退出时终止。
> 8. 定义了ufusr_ask_unload函数，设置二次开发模块立即卸载。
>
> 总的来说，这段代码通过NX的API，实现了让用户选择边线并高亮显示其所有分割边线的效果。
>
