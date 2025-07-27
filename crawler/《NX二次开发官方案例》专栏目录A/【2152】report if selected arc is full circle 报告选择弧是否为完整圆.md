### 【2152】report if selected arc is full circle 报告选择弧是否为完整圆

#### 代码

```cpp
    /*HEAD REPORT_IF_SELECTED_ARC_IS_FULL_CIRCLE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    #include <uf_eval.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18中新增的，仅提供译文，不要添加任何废话。 */  
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
    static int mask_for_arcs(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_circle_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_CIRCULAR_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_an_arc(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an arc", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_arcs, NULL, &resp,  
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
        logical  
            is_closed;  
        tag_t  
            arc;  
        UF_EVAL_p_t  
            evaluator;  
        while ((arc = select_an_arc("Full circle or not?")) != NULL_TAG)  
        {  
            WRITE_D(arc);  
            UF_CALL(UF_EVAL_initialize(arc, &evaluator));  
            UF_CALL(UF_EVAL_is_periodic(evaluator, &is_closed));  
            WRITE_L(is_closed);  
            UF_CALL(UF_EVAL_free(evaluator));  
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

> 这段代码是一个NX Open C++应用程序，用于检测用户选择的弧是否为完整的圆弧。
>
> 主要功能和步骤如下：
>
> 1. 定义了错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 2. 定义了mask_for_arcs函数，设置选择过滤器，仅允许用户选择圆弧类型的实体。
> 3. 定义了select_an_arc函数，弹出一个选择对话框，让用户选择一个弧，并返回弧的tag。
> 4. 定义了write_logical_to_listing_window和write_integer_to_listing_window函数，用于在列表窗口中输出逻辑值和整数值。
> 5. do_it函数是主函数，循环调用select_an_arc获取用户选择的弧，然后创建评估器，检测该弧是否为周期性的（即完整圆弧），并将结果输出到列表窗口。
> 6. ufusr函数是NX调用的入口函数，初始化NX后调用do_it函数，完成后终止NX。
> 7. ufusr_ask_unload函数用于处理卸载，返回立即卸载。
>
> 整个程序通过选择过滤器让用户只选择圆弧，然后检测该圆弧是否为完整圆弧，并将结果输出，实现了检查圆弧是否为完整圆弧的功能。
>
