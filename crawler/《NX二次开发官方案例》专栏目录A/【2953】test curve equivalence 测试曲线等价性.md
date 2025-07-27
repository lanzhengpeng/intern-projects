### 【2953】test curve equivalence 测试曲线等价性

#### 代码

```cpp
    /*HEAD TEST_CURVE_EQUIVALENCE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_eval.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中新增的函数。 */  
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
    static int mask_for_curves_and_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[5] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 5, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    extern tag_t select_a_curve_or_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves_and_edges, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
            return object;  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        logical  
            is_equal;  
        tag_t  
            first,  
            second;  
        UF_EVAL_p_t  
            fp,  
            sp;  
        char  
            msg[133];  
        while (((first=select_a_curve_or_edge("Select first object"))!=NULL_TAG) &&  
            ((second = select_a_curve_or_edge("Select second object"))!=NULL_TAG))  
        {  
            UF_CALL(UF_EVAL_initialize(first, &fp));  
            UF_CALL(UF_EVAL_initialize(second, &sp));  
            UF_CALL(UF_EVAL_is_equal(fp, sp, &is_equal));  
            if (is_equal)  
                sprintf(msg, "%d = %d", first, second);  
            else  
                sprintf(msg, "%d != %d", first, second);  
            uc1601(msg, TRUE);  
            UF_CALL(UF_DISP_set_highlight(first, 0));  
            UF_CALL(UF_DISP_set_highlight(second, 0));  
            UF_CALL(UF_EVAL_free(fp));  
            UF_CALL(UF_EVAL_free(sp));  
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

> 这段代码是用于NX CAD系统进行曲线等价性比较的二次开发示例。其主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于输出错误信息。
> 2. 定义了一个用于设置选择掩码的函数mask_for_curves_and_edges，以允许选择曲线和边线。
> 3. 定义了一个选择曲线或边的函数select_a_curve_or_edge，使用户可以交互式地选择曲线或边。
> 4. 主函数do_it中，首先使用select_a_curve_or_edge选择两个对象，然后使用UF_EVAL模块进行初始化，调用UF_EVAL_is_equal函数比较两个对象是否等价，并输出结果。
> 5. ufusr是主函数，用于初始化和调用do_it函数。ufusr_ask_unload函数用于在卸载时立即卸载。
> 6. 使用了UF_DISP模块设置高亮显示选中的对象。
> 7. 使用了UF_UI模块打开列表窗口并输出信息。
>
> 总体来说，这段代码通过交互式选择和比较两个对象，实现了曲线等价性的比较功能，并提供了友好的用户交互界面。
>
