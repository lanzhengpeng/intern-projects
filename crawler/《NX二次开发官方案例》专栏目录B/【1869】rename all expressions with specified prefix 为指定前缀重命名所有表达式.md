### 【1869】rename all expressions with specified prefix 为指定前缀重命名所有表达式

#### 代码

```cpp
    /*HEAD RENAME_ALL_EXPRESSIONS_WITH_SPECIFIED_PREFIX CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的说明，UF_print_syslog是在V18版本中新增的功能。因此，在翻译时只需简洁地表述为“UF_print_syslog是V18版本新增的功能”，而不需要添加任何其他内容或细节。 */  
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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static void do_it(void)  
    {     
        int  
            inx = 0,  
            n_exps = 0;  
        logical  
            flag = FALSE;  
        tag_t  
            *exps = NULL,  
            exp = NULL_TAG,  
            part = NULL_TAG;  
        char  
            *string = NULL,  
            *lhs_str = NULL,  
            *rhs_str = NULL,  
            tmp_name[UF_MAX_EXP_LENGTH+1] = { "" },  
            prefix[UF_MAX_EXP_LENGTH+1] = { "" };  
        if( (part = UF_PART_ask_display_part()) == NULL_TAG ) return;   
        if((flag = prompt_for_text("Enter Expression Prefix:", prefix)) == TRUE)  
        {  
            if( strlen( prefix) == 0 ) return;  
            UF_CALL(UF_MODL_ask_exps_of_part(part, &n_exps, &exps));  
            if( n_exps == 0 ) return;  
            for( inx = 0; inx < n_exps; inx++ )  
            {  
                UF_CALL(UF_MODL_ask_exp_tag_string(exps[inx], &string));  
                UF_CALL(UF_MODL_dissect_exp_string(string, &lhs_str, &rhs_str, &exp));  
                strcpy( tmp_name, prefix );  
                strcat( tmp_name, lhs_str );  
                UF_CALL(UF_MODL_rename_exp( lhs_str, tmp_name ));  
                strcpy( tmp_name, "" );  
                UF_free(string);  
                UF_free(rhs_str);  
                UF_free(lhs_str);  
            }  
            if (n_exps > 0) UF_free(exps);  
            UF_MODL_update();  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        UF_set_retiring_flag( 2 ); /* notify about retiring functions 里海译:关于退役功能的通告 */  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 根据代码，这是一个NX Open C++的二次开发示例程序，主要功能是为NX部件中的所有表达式添加指定的前缀。下面是该代码的详细介绍：
>
> 1. 包含了必要的NX Open C++头文件，用于访问NX的建模和UI等API。
> 2. 定义了宏UF_CALL，用于调用NX API函数，并在调用失败时报告错误。
> 3. 实现了report_error函数，用于打印错误信息和调用失败的函数名。
> 4. 实现了prompt_for_text函数，用于弹出提示框让用户输入文本。
> 5. 实现了do_it函数，是程序的核心部分。首先获取当前显示的部件，然后提示用户输入要添加的前缀。接着遍历该部件中的所有表达式，为每个表达式添加前缀并重命名。最后调用UF_MODL_update更新NX。
> 6. 实现了ufusr函数，是NX二次开发的入口函数。在这里初始化NX环境，调用do_it执行功能，最后终止NX环境。
> 7. 实现了ufusr_ask_unload函数，返回立即卸载标志，表示该二次开发程序不需要在NX中保留状态。
>
> 总体来说，该代码通过NX Open C++ API实现了为部件表达式添加前缀的功能，采用了面向对象的编程方式，具有良好的结构。
>
