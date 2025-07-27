### 【1257】facet selected solids facet selected solids

#### 代码

```cpp
    /*HEAD FACET_SELECTED_SOLIDS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_facet.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的，仅提供翻译，无需其他废话。 */  
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
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_bodies, NULL, &resp,  
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
            body,  
            facet;  
        UF_FACET_parameters_t  
            parms;  
        UF_CALL(UF_FACET_ask_default_parameters(&parms));  
        while ((body = select_a_body("Select solid to facet")) != NULL_TAG)  
            UF_CALL(UF_FACET_facet_solid(body, &parms, &facet));  
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

> 这段代码是NX Open C++ API编写的二次开发代码，主要实现选择实体并对其进行Faceting处理的功能。具体来说，代码的主要逻辑如下：
>
> 1. 包含所需的头文件，并定义了错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了mask_for_bodies函数，用于设置选择实体的类型为实体体，并启用了特征体选择模式。
> 3. 定义了select_a_body函数，用于弹出一个选择对话框，让用户选择一个实体。如果选择成功，则返回该实体的tag。
> 4. 定义了do_it函数，用于循环调用select_a_body获取用户选择的实体，然后调用UF_FACET_facet_solid对该实体进行Faceting处理。
> 5. 定义了ufusr函数，作为二次开发的入口函数。在该函数中，首先初始化UF，然后调用do_it函数处理实体，最后终止UF。
> 6. 定义了ufusr_ask_unload函数，用于设置二次开发的卸载模式为立即卸载。
>
> 总的来说，这段代码利用NX的Faceting功能，实现了对用户选择的实体进行Faceting处理的功能，为用户提供了便捷的操作界面。
>
