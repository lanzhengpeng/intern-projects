### 【2386】report specified expressions features 报告指定表达式的特征

#### 代码

```cpp
    /*HEAD REPORT_SPECIFIED_EXPRESSIONS_FEATURES CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数。它用于将错误日志信息打印到系统日志中。

文档中的说明翻译如下：

UF_print_syslog是V18版本新增的函数，用于将错误日志信息打印到系统日志中。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static tag_t ask_tag_of_expression_name(char *exp_name)  
    {  
        tag_t  
            exp_tag;  
        char  
            exp_str[UF_MAX_EXP_LENGTH+1] = { "" },  
            *lhs,  
            *rhs;  
        UF_CALL(UF_MODL_ask_exp(exp_name, exp_str));  
        if (!UF_CALL(UF_MODL_dissect_exp_string(exp_str, &lhs, &rhs, &exp_tag)))  
        {  
            UF_free(lhs);  
            UF_free(rhs);  
            return (exp_tag);  
        }  
        else  
            return NULL_TAG;  
    }  
    static void report_feature_name_and_type(char *title, tag_t feat)  
    {  
        char  
            msg[MAX_LINE_SIZE+1],  
            *name,  
            *type;  
        if (!UF_CALL(UF_MODL_ask_feat_name(feat, &name)) &&  
            !UF_CALL(UF_MODL_ask_feat_type(feat, &type)))  
        {  
            UF_CALL(UF_UI_open_listing_window());  
            sprintf(msg, "%s = %d %s (%s)\n", title, feat, name, type);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            UF_free(name);  
            UF_free(type);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_feats;  
        tag_t  
            exp_tag,  
            *feats;  
        char  
            feat_name[20],  
            exp_name[UF_MAX_EXP_LENGTH] = { "" };  
        while (prompt_for_text("Enter expression name (lhs)", exp_name))  
        {  
            WRITE_S(exp_name);  
            if ((exp_tag = ask_tag_of_expression_name(exp_name)) == NULL_TAG)  
            {  
                WRITE("Expression not found.\n");  
                continue;  
            }  
            UF_CALL(UF_MODL_ask_features_of_exp(exp_tag, &n_feats, &feats));  
            WRITE_D(n_feats);  
            for (ii = 0; ii < n_feats; ii++)  
            {  
                sprintf(feat_name, "feats[%d]", ii);  
                report_feature_name_and_type(feat_name, feats[ii]);  
            }  
            if (n_feats > 0) UF_free(feats);  
            WRITE("\n");  
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

> 这段代码是一个NX Open C++的二次开发示例，其主要功能是获取用户输入的表达式名称，然后查询该表达式对应的特征，并输出每个特征的信息。代码的关键步骤包括：
>
> 1. 包含必要的NX Open C++头文件。
> 2. 定义UF_CALL宏，用于调用NX Open C++函数，并捕获错误码。
> 3. 定义report_error函数，用于打印错误信息。
> 4. 定义WRITE系列宏，用于向列表窗口输出文本。
> 5. 定义write_integer_to_listing_window和write_string_to_listing_window函数，用于向列表窗口输出整型和字符串。
> 6. 定义prompt_for_text函数，用于提示用户输入文本。
> 7. 定义ask_tag_of_expression_name函数，用于查询表达式名称对应的表达式标签。
> 8. 定义report_feature_name_and_type函数，用于输出特征名称和类型。
> 9. 定义do_it函数，用于实现主体功能：获取用户输入的表达式名称，查询对应的特征并输出。
> 10. 定义ufusr函数，作为NX Open C++的入口函数。
> 11. 定义ufusr_ask_unload函数，用于卸载二次开发程序。
>
> 通过上述步骤，该代码实现了在NX中查询表达式对应的特征信息的功能。
>
