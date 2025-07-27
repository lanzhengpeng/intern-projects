### 【0242】ask is curve tiny 询问曲线是否微小

#### 代码

```cpp
    /*HEAD ASK_IS_CURVE_TINY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，翻译如下：

UF_print_syslog是V18版本新增的功能。 */  
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
    static tag_t select_a_curve(char *prompt)  
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
            curve;  
        logical  
            is_tiny = 0;  
        while ((curve = select_a_curve("Select curve")) != NULL_TAG)   
        {  
            UF_CALL(UF_MODL_ask_tiny_geometry(curve, 0.0, &is_tiny));  
            if (is_tiny == 1)  
                printf("Object %d is tiny\n", curve);  
            else  
                printf("Object %d is not tiny\n", curve);  
        }  
    }  
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

> 这段NX二次开发代码实现了选择曲线并判断其是否为“微小曲线”的功能。
>
> 主要功能如下：
>
> 1. 定义了一个错误报告函数report_error，用于输出错误信息。
> 2. 定义了一个设置曲线选择掩码的函数mask_for_curves，用于只允许选择线、圆、圆锥曲线和样条曲线。
> 3. 定义了一个选择曲线的函数select_a_curve，用于打开选择对话框，让用户选择一个曲线对象。
> 4. 定义了一个主函数do_it，用于循环调用select_a_curve选择曲线，然后调用UF_MODL_ask_tiny_geometry函数判断曲线是否为微小曲线，并打印结果。
> 5. 定义了UF初始化函数ufusr，用于在NX启动时调用do_it函数。
> 6. 定义了卸载函数ufusr_ask_unload，用于在NX卸载时返回立即卸载标志。
>
> 整体来看，这段代码通过UI和MODL两个模块，实现了选择曲线并判断其是否为微小曲线的功能。
>
