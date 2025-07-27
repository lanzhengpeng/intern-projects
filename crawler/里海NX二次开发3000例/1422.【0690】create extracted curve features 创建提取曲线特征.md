### 【0690】create extracted curve features 创建提取曲线特征

#### 代码

```cpp
    /*HEAD CREATE_EXTRACTED_CURVE_FEATURES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_wave.h>  
    #include <uf_assem.h>  
    #include <uf_so.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据原文内容，UF_print_syslog是V18版本中的新增功能。 */  
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
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve or edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves_and_edges, NULL,  
            &resp, &object, cp, &view));  
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
        while ((curve = select_a_curve_or_edge("Extract Curve")) != NULL_TAG)  
        {  
            if (UF_ASSEM_is_occurrence(curve))  
            {  
                proto = UF_ASSEM_ask_prototype_of_occ(curve);  
                UF_CALL(UF_SO_create_xform_assy_ctxt(curve,  
                    UF_ASSEM_ask_part_occurrence(curve), NULL_TAG, &xform));  
                UF_CALL(UF_WAVE_create_linked_curve(proto, xform, curve, TRUE,  
                    &feat));  
            }  
            else  
                UF_CALL(UF_WAVE_create_linked_curve(curve, NULL_TAG, curve, TRUE,  
                    &feat));  
        }  
        UF_CALL(UF_MODL_update());  
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

> 根据代码，这是段用于NX的二次开发代码，主要功能是从NX的图形对象中提取曲线特征。以下是代码的主要功能和步骤：
>
> 1. 引入所需的NX API头文件。
> 2. 定义了一个错误报告函数report_error，用于输出错误信息。
> 3. 定义了一个用于选择曲线或边的函数select_a_curve_or_edge，它使用NX的选择对话框，让用户选择一个曲线或边，并返回所选对象的tag。
> 4. 定义了一个用于设置选择掩码的函数mask_for_curves_and_edges，它用于设置选择对话框只可以选择曲线和边。
> 5. 定义了主要函数do_it，该函数循环调用select_a_curve_or_edge获取用户选择的曲线或边，然后使用UF_WAVE_create_linked_curve API创建一个链接曲线特征，如果对象是装配中的实例，还需要获取原型并设置装配变换。
> 6. 在ufusr函数中初始化NX环境，调用do_it函数，然后终止NX环境。
> 7. 定义了ufusr_ask_unload函数，用于在用户卸载该程序时立即卸载。
>
> 综上所述，这段代码实现了从NX图形对象提取曲线特征的功能，包括选择对象、创建链接曲线特征，并对装配中的实例进行了处理。
>
