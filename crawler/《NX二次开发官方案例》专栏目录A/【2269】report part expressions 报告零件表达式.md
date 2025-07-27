### 【2269】report part expressions 报告零件表达式

#### 代码

```cpp
    /*HEAD REPORT_PART_EXPRESSIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #define ECHO(X)    (printf("\t%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 新增的功能。 */  
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
    static void report_part_expressions(tag_t part)  
    {  
        int  
            ii,  
            jj,  
            n_exps,  
            n_feats;  
        tag_t  
            *exps,  
            *feats;  
        char  
            fspec[MAX_FSPEC_SIZE+1],  
            *string;  
        UF_PART_ask_part_name(part, fspec);  
        if (!UF_CALL(UF_MODL_ask_exps_of_part(part, &n_exps, &exps)))  
        {  
            printf("%s has %d expressions:\n", fspec, n_exps);  
            for (ii = 0; ii < n_exps; ii++)  
            {  
                if (UF_CALL(UF_MODL_ask_exp_tag_string(exps[ii], &string)))  
                    continue;  
                printf("  %d.  %s", ii+1, string);  
                UF_free(string);  
                if (UF_CALL(UF_MODL_ask_features_of_exp(exps[ii],&n_feats,&feats)))  
                {  
                    printf("\n");  
                    continue;  
                }  
                printf(" - Used in %d features\n", n_feats);  
                for (jj = 0; jj < n_feats; jj++)  
                {  
                    UF_CALL(UF_MODL_ask_feat_name(feats[jj], &string));  
                    printf("    %d.  %s\n", jj+1, string);  
                    UF_free(string);  
                }  
                if (n_feats > 0) UF_free(feats);  
            }  
            if (n_exps > 0) UF_free(exps);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        if (part != NULL_TAG) report_part_expressions(part);  
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

> 这段代码是一个NX Open C++应用程序，主要实现了以下功能：
>
> 1. 错误报告函数：定义了一个report_error函数，用于在发生UF函数调用错误时，报告错误信息，包括文件名、行号、错误码和错误描述。
> 2. 表达式报告函数：定义了一个report_part_expressions函数，用于查询当前显示部件的表达式信息，并输出每个表达式的名称、所使用的特征数量以及每个特征的名称。
> 3. 主体函数：定义了一个do_it函数，用于获取当前显示的部件，然后调用report_part_expressions函数输出表达式信息。
> 4. UF初始化和终止：在ufusr函数中，首先调用UF_initialize进行UF初始化，然后调用do_it函数，最后调用UF_terminate终止UF。
> 5. 卸载函数：定义了一个ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY表示立即卸载应用程序。
>
> 该应用程序主要用于在NX中查询当前显示部件的表达式信息，并以可读的方式输出，以便用户了解当前部件的表达式使用情况。
>
