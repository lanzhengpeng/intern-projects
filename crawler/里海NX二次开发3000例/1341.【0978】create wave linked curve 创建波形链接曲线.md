### 【0978】create wave linked curve 创建波形链接曲线

#### 代码

```cpp
    /*HEAD CREATE_WAVE_LINKED_CURVE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_wave.h>  
    #include <uf_so.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中新增的函数，用于打印系统日志。 */  
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
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    extern tag_t select_a_curve(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
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
            curve,  
            feat,  
            proto,  
            xform;  
        while ((curve = select_a_curve("Select curve to link")) != NULL_TAG)  
        {  
            if (UF_ASSEM_is_occurrence(curve))  
            {  
                proto = UF_ASSEM_ask_prototype_of_occ(curve);  
                UF_CALL(UF_SO_create_xform_assy_ctxt(curve,  
                    UF_ASSEM_ask_part_occurrence(curve), NULL_TAG, &xform));  
                UF_CALL(UF_WAVE_create_linked_curve(proto,xform,curve,TRUE,&feat));  
            }  
            else  
                UF_CALL(UF_WAVE_create_linked_curve(curve, NULL_TAG, curve, TRUE,  
                    &feat));  
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

> 这段代码是一个NX二次开发的示例，主要实现了以下功能：
>
> 1. 使用UF_CALL宏封装UF函数调用，便于错误检查和报告。
> 2. 定义了mask_for_curves函数，用于设置选择过滤，只允许选择线、圆、圆锥曲线和样条线。
> 3. select_a_curve函数实现了选择曲线的功能，并返回选择的曲线的tag。
> 4. do_it函数是主函数，用于创建波浪线特征。它会循环选择曲线，并对每个曲线调用UF_WAVE_create_linked_curve函数创建波浪线特征。
> 5. ufusr函数是NX调用的入口函数，用于初始化、执行do_it函数和终止。
> 6. ufusr_ask_unload函数定义了卸载方式，表示立即卸载。
>
> 综合来看，这段代码通过用户选择曲线，为每条曲线创建波浪线特征，实现了波浪线的创建功能。
>
