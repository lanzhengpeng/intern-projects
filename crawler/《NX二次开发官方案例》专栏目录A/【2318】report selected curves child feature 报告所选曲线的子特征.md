### 【2318】report selected curves child feature 报告所选曲线的子特征

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_CURVES_CHILD_FEATURE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_undo.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据上下文，UF_print_syslog 在 V18 中是新增的。 */  
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
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_feature_name(tag_t feat)  
    {  
        char  
            *feature;  
        if (!UF_CALL(UF_MODL_ask_feat_name(feat, &feature)))  
        {  
            WRITE_S(feature);  
            UF_free(feature);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            curve,  
            feat;  
        while ((curve = select_a_curve("Report feature of curve")) != NULL_TAG)  
        {  
            if (!UF_CALL(UF_MODL_ask_object_feat(curve, &feat)) &&  
                (feat != NULL_TAG))  
            {  
                WRITE("Curve's child is ");  
                report_feature_name(feat);  
            }  
            else  
            {  
                uc1601("Curve has no associated feature", TRUE);  
            }  
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

> 这段代码是一个NX的二次开发示例，主要功能是查询曲线所对应的特征名称。
>
> 主要函数和流程如下：
>
> 1. report_error：自定义错误报告函数，用于打印出调用失败的UF函数的名称、错误码等信息。
> 2. mask_for_curves：设置选择过滤器，只允许选择线、圆、圆锥曲线和样条线。
> 3. select_a_curve：弹出一个对话框，让用户选择一条曲线。返回选择的曲线的tag。
> 4. write_string_to_listing_window：在列表窗口中打印信息。
> 5. report_feature_name：查询特征名称，并在列表窗口中打印。
> 6. do_it：主函数。循环调用select_a_curve获取曲线，然后调用UF_MODL_ask_object_feat查询曲线对应的特征，再调用report_feature_name打印特征名称。
> 7. ufusr：NX提供的入口函数，初始化后调用do_it，最后终止。
> 8. ufusr_ask_unload：NX提供的询问卸载函数，返回立即卸载。
>
> 该代码通过二次开发实现了查询曲线对应特征名称的功能，使用了NX提供的UF函数进行交互和查询。
>
