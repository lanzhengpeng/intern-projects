### 【1350】get value of expression associated with dimension 获取与尺寸关联的表达式的值

#### 代码

```cpp
    /*HEAD GET_VALUE_OF_EXPRESSION_ASSOCIATED_WITH_DIMENSION CCC UFUN */  
    /* inherited sketch dimensions have an associated expression.   
        They are generated in drafting by:  
        insert --> model parameters --> features */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_drf.h>  
    #include <uf_ui.h>  
    #include <uf_obj_types.h>  
    #include <uf_object_types.h>  
    #include <uf_drf_types.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中新增的函数，用于打印系统日志。 */  
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
    static int mask_for_dimensions(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_dimension_type, UF_all_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_dimension(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a dimension", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_dimensions, NULL, &resp,  
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
            exp_tag = NULL_TAG,  
            draft_aid = NULL_TAG;  
        double  
            value;  
        char   
             msg[133];  
        while (draft_aid = select_a_dimension("Select Dimension object"))  
        {  
            ECHO(draft_aid);  
            UF_CALL(UF_DRF_ask_assoc_exp(draft_aid, &exp_tag));  
            if (exp_tag == NULL_TAG)  
            {  
                uc1601("There is no associated expression", TRUE);  
            }  
            else  
            {  
                UF_CALL(UF_MODL_ask_exp_tag_value(exp_tag, &value));  
                sprintf(msg, "Dimension expression evaluates to %f", value);  
                uc1601(msg, TRUE);  
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

> 这段代码是一个NX二次开发代码，其主要功能是获取与尺寸相关的表达式的值。
>
> 代码的主要步骤包括：
>
> 1. 包含所需的NX API头文件。
> 2. 定义了一个报告错误函数report_error，用于在函数调用出错时打印错误信息。
> 3. 定义了一个选择尺寸的函数select_a_dimension，该函数通过对话框让用户选择一个尺寸对象。
> 4. 定义了一个获取尺寸关联表达式值的函数do_it。该函数首先调用select_a_dimension获取尺寸对象，然后调用UF_DRF_ask_assoc_exp获取关联的表达式，再调用UF_MODL_ask_exp_tag_value获取表达式的值，最后输出。
> 5. ufusr是主函数，初始化NX环境后调用do_it函数，最后终止NX环境。
> 6. ufusr_ask_unload函数用于卸载用户库，此处返回UF_UNLOAD_IMMEDIATELY表示立即卸载。
>
> 整体而言，这段代码实现了通过选择尺寸对象，获取其关联的表达式值的功能。
>
