### 【0841】create points at selected arc centers 在选定的弧中心创建点

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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog在V18中是新增的，仅需要回答翻译，无需添加任何额外内容。 */  
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

> 这段代码是NX的二次开发代码，其主要功能是在选择的圆弧中心创建一个点。以下是代码的详细介绍：
>
> 1. 引入了NX API的头文件，包括错误处理、用户交互、对象类型、显示、装配、零件、曲线、坐标系和评估等。
> 2. 定义了错误处理函数report_error，用于打印函数执行错误信息。
> 3. 定义了mask_for_arcs函数，用于设置选择圆弧的过滤条件。
> 4. 定义了select_an_arc函数，用于提示用户选择一个圆弧，并返回选择的圆弧标签。
> 5. 定义了do_it函数，是代码的核心功能函数。首先获取当前显示的零件和当前工作零件。然后循环调用select_an_arc函数选择圆弧，创建评估器，获取圆弧的参数，释放评估器。如果当前工作零件不是显示零件，则转换圆弧中心坐标系。最后在圆弧中心创建一个点。
> 6. ufusr函数是NX二次开发的入口函数。初始化NX系统后，调用do_it函数执行功能，最后终止NX系统。
> 7. ufusr_ask_unload函数用于设置用户卸载请求，返回立即卸载。
>
> 总的来说，这段代码实现了在NX中选择圆弧并在其中心创建一个点的功能，通过用户交互、几何评估、坐标转换等操作实现。
>
