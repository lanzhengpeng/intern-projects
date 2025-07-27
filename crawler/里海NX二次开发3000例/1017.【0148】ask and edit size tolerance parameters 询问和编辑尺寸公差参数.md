### 【0148】ask and edit size tolerance parameters 询问和编辑尺寸公差参数

#### 代码

```cpp
    /*HEAD ASK_AND_EDIT_SIZE_TOLERANCE_PARAMETERS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_gdt.h>  
    #include <uf_modl.h>  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据您的要求，我仅提供翻译：

注意：UF_print_syslog在V18中是新的。 */  
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
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* 里海 */  
    static int mask_gdt(UF_UI_selection_p_t select, void *user_data)  
    {  
        UF_UI_mask_t  
            triples = { UF_tol_feature_instance_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &triples)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_gdt(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_gdt, NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void strip_trailing_zeros(char *s)  
    {  
        int ii;  
        if (strchr(s, '.'))  
        {  
            for (ii = strlen(s) - 1; s[ii] == '0'; ii--) s[ii] = '\0';  
            if (s[ii] == '.') s[ii] = '\0';  
        }  
    }  
    static void set_expression_tag_value(tag_t exp, double new_value)  
    {  
        char  
            *exp_str,  
            *lhs_str,  
            new_exp[UF_MAX_EXP_LENGTH+1],  
            *rhs_str;  
        UF_CALL(UF_MODL_ask_exp_tag_string(exp, &exp_str));  
        UF_CALL(UF_MODL_dissect_exp_string(exp_str, &lhs_str, &rhs_str, &exp));  
        sprintf(new_exp, "%s=%f", lhs_str, new_value);  
        strip_trailing_zeros(new_exp);  
        UF_CALL(UF_MODL_edit_exp(new_exp));  
        UF_free(exp_str);  
        UF_free(rhs_str);  
        UF_free(lhs_str);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat,  
            obj;  
        UF_GDT_size_tolerance_t  
            *size_data;  
        while ((obj = select_a_gdt("Select size tolerance")) != NULL_TAG)  
        {  
            UF_CALL(UF_GDT_ask_tol_feat_of_instance(obj, &feat));  
            UF_CALL(UF_GDT_ask_size_tolerance_parms(feat, &size_data));  
            WRITE_F(size_data->nominal_value);  
            WRITE_F(size_data->upper_tol.value);  
            WRITE_F(size_data->lower_tol.value);  
        /*  To edit the tolerance values edit their expressions 译:修改容差值，请编辑它们的表达式。 */  
            set_expression_tag_value(size_data->upper_tol.expression,  
                (size_data->upper_tol.value + 0.01));  
            set_expression_tag_value(size_data->lower_tol.expression,  
                (size_data->lower_tol.value - 0.01));  
            UF_CALL(UF_GDT_free(UF_GDT_SIZE_TOL_TYPE, size_data));  
            UF_CALL(UF_MODL_update());  
            UF_CALL(UF_GDT_update_features(1, &feat));  
            UF_CALL(UF_GDT_ask_size_tolerance_parms(feat, &size_data));  
            WRITE_F(size_data->nominal_value);  
            WRITE_F(size_data->upper_tol.value);  
            WRITE_F(size_data->lower_tol.value);  
            UF_CALL(UF_GDT_free(UF_GDT_SIZE_TOL_TYPE, size_data));  
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

> 这段代码是NX的二次开发代码，主要用于修改尺寸公差参数。下面是对代码的详细说明：
>
> 1. 定义了两个宏WRITE_F和UF_CALL，用于在列表窗口中输出数值和调用UF函数并报告错误。
> 2. 定义了report_error函数，用于在调用UF函数出错时输出错误信息。
> 3. 定义了write_double_to_listing_window函数，用于在列表窗口中输出数值。
> 4. 定义了mask_gdt函数，用于设置选择过滤器，只选择特征实例。
> 5. 定义了select_a_gdt函数，用于通过对话框选择一个特征实例。
> 6. 定义了strip_trailing_zeros函数，用于去除字符串尾部的零。
> 7. 定义了set_expression_tag_value函数，用于修改表达式的值。
> 8. 定义了do_it函数，是主逻辑函数，用于选择特征实例，修改其尺寸公差参数，并更新显示。
> 9. 定义了ufusr函数，是NX调用的主入口函数。
> 10. 定义了ufusr_ask_unload函数，用于立即卸载该代码。
>
> 总体来说，这段代码通过选择特征实例，修改其尺寸公差的上下限，并更新显示，实现了对尺寸公差参数的修改功能。
>
