### 【2326】report selected edge type 报告选定边类型

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_EDGE_TYPE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据原文，翻译如下：

UF_print_syslog 是在 V18 中新增的，请只回答翻译内容，不要添加其他无关内容。

UF_print_syslog 是在 V18 中新增的。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int  
            type;  
        tag_t  
            edge;  
        WRITE_D(UF_MODL_LINEAR_EDGE);  
        WRITE_D(UF_MODL_CIRCULAR_EDGE);  
        WRITE_D(UF_MODL_ELLIPTICAL_EDGE);  
        WRITE_D(UF_MODL_INTERSECTION_EDGE);  
        WRITE_D(UF_MODL_SPLINE_EDGE);  
        WRITE_D(UF_MODL_SP_CURVE_EDGE);  
        WRITE_D(UF_MODL_FOREIGN_EDGE);  
        WRITE_D(UF_MODL_CONST_PARAMETER_EDGE);  
        WRITE_D(UF_MODL_TRIMMED_CURVE_EDGE);  
        while ((edge = select_an_edge("Report type")) != NULL_TAG)  
        {  
            WRITE_D(edge);  
            UF_CALL(UF_MODL_ask_edge_type(edge, &type));  
            WRITE_D(type);  
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

> 这段代码是一个NX的二次开发代码示例，其主要功能是选择边并报告边的类型。代码主要包括以下几个部分：
>
> 1. 错误报告函数：通过调用UF_get_fail_message获取错误信息，并在控制台和日志窗口中输出错误信息。
> 2. 边缘选择函数：使用UF_UI_set_sel_mask设置选择掩码，只允许选择边，并使用UF_UI_select_with_single_dialog让用户选择一个边，然后返回边的tag。
> 3. 写信息到列表窗口的函数：用于将整数或字符串写入列表窗口。
> 4. 主要逻辑函数：打印出各种边类型的常量，然后进入循环，让用户选择边，获取边的tag，并调用UF_MODL_ask_edge_type获取边的类型，最后将边的tag和类型写入列表窗口。
> 5. ufusr函数：初始化UF模块，调用主要逻辑函数，然后终止UF模块。
> 6. 卸载函数：返回立即卸载。
>
> 总的来说，该代码通过对话框让用户选择边，并获取边的类型，并在列表窗口中打印出边的tag和类型。代码使用了NX的UF模块进行操作，包括UF_UI、UF_MODL等。
>
