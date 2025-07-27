### 【2939】suppress selected features by expression 按表达式抑制选定特征

#### 代码

```cpp
    /*HEAD SUPPRESS_SELECTED_FEATURES_BY_EXPRESSION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的函数。 */  
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
    static int select_features(char *prompt, tag_t **feats)  
    {  
        int  
            cnt,  
            resp;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, feats, &resp));  
        if (resp == UF_UI_OK) return cnt;  
        else return 0;  
    }  
    static logical prompt_for_text_item(char *prompt, char *item, char *text)  
    {  
        int  
            ia[1],  
            ip[1] = { 301 },  
            resp;  
        double  
            da[1];  
        char  
            ca[1][31],  
            menu[1][16];  
        strncpy(&menu[0][0], item, 15);  
        menu[0][16] = '\0';  
        strncpy(&ca[0][0], text, 30);  
        ca[0][31] = '\0';  
        resp = uc1613(prompt, menu, 1, ia, da, ca, ip);  
        if (resp == 3 || resp == 4)  
        {  
            strcpy(text, &ca[0][0]);  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static logical pick_one_of_two_choices(char *prompt, char *option_one,  
        char *option_two, int *choice)  
    {  
        int  
            resp;  
        char  
            options[2][38];  
        strncpy(&options[0][0], option_one, 37);  
        strncpy(&options[1][0], option_two, 37);  
        options[0][37] = '\0';  
        options[1][37] = '\0';  
        resp = uc1603(prompt, *choice, options, 2);  
        if ((resp > 4) && (resp < 19))  
        {  
            *choice = resp - 4;  /* returns 1 or 2 里海译:根据上下文，翻译"returns 1 or 2"可以表示为“返回1或2”。

例如，在编程中，一个函数可能执行某些操作后返回1或2作为结果。

所以，"returns 1 or 2"的意思就是“返回1或2”。 */  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static tag_t ask_tag_of_expression_name(char *exp_name)  
    {  
        tag_t  
            exp_tag;  
        char  
            exp_str[UF_MAX_EXP_LENGTH+1] = { "" },  
            *lhs,  
            *rhs;  
        UF_MODL_ask_exp(exp_name, exp_str);  
        if (!UF_CALL(UF_MODL_dissect_exp_string(exp_str, &lhs, &rhs, &exp_tag)))  
        {  
            UF_free(lhs);  
            UF_free(rhs);  
            return (exp_tag);  
        }  
        else  
            return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        logical  
            unsuppress;  
        int  
            ii,  
            is_suppressed,  
            n_feats,  
            which = 1,  
            wrong_count;  
        tag_t  
            exp_tag,  
            *feats,  
            old_exp;  
        double  
            value;  
        char  
            exp_name[UF_MAX_EXP_LENGTH] = { "" },  
            exp_str[UF_MAX_EXP_LENGTH];  
        uf_list_p_t  
            list;  
        while (((n_feats = select_features("Suppress by Expression", &feats)) > 0)  
            && prompt_for_text_item("Suppress by Expression", "Exp Name", exp_name))  
        {  
            exp_tag = ask_tag_of_expression_name(exp_name);  
            if (exp_tag == NULL_TAG)  
            {  
                sprintf(exp_str, "choose value for new expression %s", exp_name);  
                if (!pick_one_of_two_choices(exp_str, "Suppress", "Unsuppress",  
                    &which)) continue;  
                unsuppress = (which == 2);  
                sprintf(exp_str, "%s=%d", exp_name, unsuppress);  
                UF_CALL(UF_MODL_create_exp_tag(exp_str, &exp_tag));  
            }  
            else  
            {  
                UF_CALL(UF_MODL_ask_exp_tag_value(exp_tag, &value));  
                unsuppress = (value == 1.0);  
            }  
        /*  The suppress state of each feature must already correspond with the  
            value of the expression when calling UF_MODL_set_suppress_exp_tag */  
            UF_CALL(UF_MODL_create_list(&list));  
            for (ii = 0, wrong_count = 0; ii < n_feats; ii++)  
            {  
                UF_CALL(UF_MODL_ask_suppress_feature(feats[ii], &is_suppressed));  
                if ((is_suppressed && unsuppress) ||  
                    (!is_suppressed && !unsuppress))  
                {  
                    wrong_count++;  
                    UF_CALL(UF_MODL_put_list_item(list, feats[ii]));  
                }  
                UF_CALL(UF_MODL_ask_suppress_exp_tag(feats[ii], &old_exp));  
                if (old_exp != NULL_TAG)  
                    UF_CALL(UF_MODL_unset_suppress_exp_tag(feats[ii]));  
            }  
            if (wrong_count > 0)  
            {  
                if (unsuppress)  
                    UF_CALL(UF_MODL_unsuppress_feature(list));  
                else  
                    UF_CALL(UF_MODL_suppress_feature(list));  
                UF_CALL(UF_MODL_delete_list(&list));  
            }  
            for (ii = 0; ii < n_feats; ii++)  
                UF_CALL(UF_MODL_set_suppress_exp_tag(feats[ii], exp_tag));  
            UF_free(feats);  
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

> 这段代码是NX的二次开发示例，主要实现了按表达式抑制特征的功能。以下是代码的主要逻辑和功能：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于打印UF函数调用错误信息。
> 2. 选择特征：使用UF_UI_select_feature选择特征，并将选择的特征存储在feats数组中。
> 3. 输入表达式名：提示用户输入表达式名，并获取表达式的tag。
> 4. 处理表达式值：如果表达式不存在，则提示用户选择抑制或取消抑制，并创建表达式。如果表达式存在，则获取其值。
> 5. 创建列表：创建一个特征列表，遍历选择的特征，检查其抑制状态是否与表达式值一致。
> 6. 处理不一致特征：将不一致的特征添加到列表中，最后统一调整这些特征的抑制状态。
> 7. 设置表达式tag：为每个特征设置表达式的tag。
> 8. ufusr函数：初始化NX环境，调用do_it函数执行主要逻辑，最后终止NX环境。
> 9. 卸载提示：定义了一个卸载提示函数，返回立即卸载。
>
> 总体而言，这段代码实现了按表达式名抑制或取消抑制特征的功能，通过人机交互获取输入，遍历特征，批量调整特征的抑制状态。
>
