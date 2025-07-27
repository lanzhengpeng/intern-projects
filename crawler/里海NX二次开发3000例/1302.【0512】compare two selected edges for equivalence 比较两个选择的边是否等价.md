### 【0512】compare two selected edges for equivalence 比较两个选择的边是否等价

#### 代码

```cpp
    /*HEAD COMPARE_TWO_SELECTED_EDGES_FOR_EQUIVALENCE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* 里海 */  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，关于 UF_print_syslog 的说明如下：

UF_print_syslog 是 V18 版本新增的功能，用于打印系统日志。 */  
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
    /* 里海 */  
    static void do_it(void)  
    {  
        int  
            relation = 1,       /*    1 = Geometric Equivalence     译:几何等价 */  
            equivalent = 0;  
        tag_t  
            edge_1 = NULL_TAG,  
            edge_2 = NULL_TAG;  
        char  
            msg[132] = { "" };  
        while( ( edge_1 = select_an_edge( "Select First Edge" )) != NULL_TAG &&  
               ( edge_2 = select_an_edge( "Select Second Edge" )) != NULL_TAG  )  
        {  
            UF_CALL( UF_MODL_compare( edge_1, edge_2, relation, &equivalent ));  
            if( equivalent )  
            {  
                strcpy( msg, "Selected Edges Are Equivalent" );  
            }  
            else  
            {  
                strcpy( msg, "Selected Edges Are NOT Equivalent" );  
            }  
            uc1601( msg, TRUE );  
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

> 这段代码是一个NX Open C++应用程序，其主要功能是允许用户选择两条边，并比较它们是否在几何上等价。
>
> 主要步骤如下：
>
> 1. 包含所需的NX Open C++头文件。
> 2. 定义了一个错误报告函数report_error，用于输出错误信息。
> 3. 定义了一个选择边界的函数mask_for_edges，用于设置选择模式。
> 4. 定义了一个选择边的函数select_an_edge，用于通过对话框让用户选择一条边。
> 5. 定义了主函数do_it，其中包含循环，让用户选择两条边，并使用UF_MODL_compare函数比较它们是否等价，最后显示比较结果。
> 6. 定义了UFusr函数，用于初始化和终止NX，并调用do_it函数。
> 7. 定义了ufusr_ask_unload函数，用于在卸载应用程序时返回立即卸载标志。
>
> 这个程序实现了简单的选择边界并比较的功能，采用NX Open C++ API实现。
>
