### 【1078】display direction of selected arcs axis 显示选定弧的中心轴方向

#### 代码

```cpp
    /*HEAD DISPLAY_DIRECTION_OF_SELECTED_ARCS_AXIS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_csys.h>  
    #include <uf_disp.h>  
    #include <uf_eval.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译如下：

注意：UF_print_syslog 是在 V18 中新增的，只需回答翻译，不要添加其他内容。 */  
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
    /* qq3123197280 */  
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
    /* qq3123197280 */  
    static void display_conehead(double origin[3], double vec[3])  
    {  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            arc = NULL_TAG,  
            mtx_tag = NULL_TAG;  
        double  
            mx[9] = { 0., 0., 0.,  0., 0., 0.,  0., 0., 0. };  
        UF_EVAL_p_t   
            arc_eval;  
        UF_EVAL_arc_t   
            arc_data;  
        while( (arc = select_an_arc("Arc?")) != NULL_TAG )  
        {  
            UF_CALL( UF_CSYS_ask_matrix_of_object( arc, &mtx_tag ));   
            UF_CALL( UF_CSYS_ask_matrix_values (mtx_tag, mx ));  
            UF_CALL( UF_EVAL_initialize( arc, &arc_eval ));  
            UF_CALL( UF_EVAL_ask_arc( arc_eval, &arc_data ));  
            display_conehead( arc_data.center, &mx[6] );  
            UF_CALL(UF_EVAL_free(arc_eval));  
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

> 根据代码内容，这是用于在NX中实现选择弧并显示其方向矢头的二次开发代码。以下是代码的主要功能：
>
> 1. 包含必要的NX头文件，如uf.h、uf_csys.h、uf_disp.h等，以调用NX的API。
> 2. 定义了错误报告函数report_error，用于在调用NX API失败时打印错误信息。
> 3. 定义了选择过滤函数mask_for_arcs，用于在选择对话框中过滤出圆弧和圆形边缘。
> 4. 定义了选择函数select_an_arc，用于弹出一个对话框，让用户选择一个圆弧对象。
> 5. 定义了显示圆锥头函数display_conehead，用于在选定的圆弧中心显示一个圆锥头表示其方向。
> 6. 定义了主函数do_it，用于循环让用户选择圆弧，获取圆弧的中心点和法向量，并显示圆锥头。
> 7. 定义了ufusr函数，用于初始化NX环境，调用主函数，然后终止NX环境。
> 8. 定义了卸载函数ufusr_ask_unload，用于在卸载时立即卸载。
>
> 总的来说，这段代码实现了选择圆弧并显示其方向矢头的基本功能，为NX的二次开发提供了参考。
>
