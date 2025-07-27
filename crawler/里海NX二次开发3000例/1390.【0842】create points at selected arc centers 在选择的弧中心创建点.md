### 【0842】create points at selected arc centers 在选择的弧中心创建点

#### 代码

```cpp
    /*HEAD CREATE_POINTS_AT_SELECTED_ARC_CENTERS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_curve.h>  
    #include <uf_csys.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，翻译如下：

注意：UF_print_syslog 是 V18 版本新增的。 */  
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
    static void do_it(void)  
    {  
        tag_t  
            arc,  
            display_part = UF_PART_ask_display_part(),  
            point,  
            work_part = UF_ASSEM_ask_work_part();  
        UF_EVAL_p_t  
            evaluator;  
        UF_EVAL_arc_t  
            e_arc;  
        while ((arc = select_an_arc("Create point at arc center")) != NULL_TAG)  
        {  
            UF_CALL(UF_EVAL_initialize(arc, &evaluator));  
            UF_CALL(UF_EVAL_ask_arc(evaluator, &e_arc));  
            UF_CALL(UF_EVAL_free(evaluator));  
            if (work_part != display_part)  
                UF_CALL(UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, e_arc.center,  
                    UF_CSYS_WORK_COORDS, e_arc.center));  
            UF_CALL(UF_CURVE_create_point(e_arc.center, &point));  
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

> 这段NX二次开发代码的主要功能是选择一个弧线，并在弧线的中心创建一个点。
>
> 主要步骤如下：
>
> 1. 定义错误报告函数report_error，用于输出错误信息。
> 2. 定义选择弧线的函数select_an_arc，其中使用UF_UI_set_sel_mask来设置选择器，只允许选择圆弧类型的实体，并使用UF_UI_select_with_single_dialog来选择一个弧线，并返回弧线的tag。
> 3. 定义核心处理函数do_it，其中使用UF_EVAL_ask_arc来获取弧线的中心点坐标，然后使用UF_CSYS_map_point将中心点坐标从根坐标系转换到工作坐标系，并使用UF_CURVE_create_point在弧线的中心创建一个点。
> 4. 在ufusr函数中，首先调用UF_initialize来初始化NX，然后调用do_it函数来处理，最后调用UF_terminate来结束NX。
> 5. 定义ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY表示模块可以立即卸载。
>
> 总的来说，这段代码实现了在选择的弧线中心创建点的功能。
>
