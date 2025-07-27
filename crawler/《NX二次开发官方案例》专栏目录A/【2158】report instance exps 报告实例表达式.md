### 【2158】report instance exps 报告实例表达式

#### 代码

```cpp
    /*HEAD REPORT_INSTANCE_EXPS CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据您的要求，我翻译了以下内容：

UF_print_syslog 是 V18 中新增的函数，用于打印系统日志。 */  
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
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            printf("\n");  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    static void report_feature_name(tag_t feat)  
    {  
        char  
            *string;  
        UF_CALL(UF_MODL_ask_feat_name(feat, &string));  
        printf("%d = %s\n", feat, string);  
        UF_free(string);  
    }  
    static void report_feature_relatives(tag_t feat)  
    {  
        int  
            ii,  
            num_exps = 0,  
            n_children,  
            n_parents;  
        char    *feat_type, *exp_str;  
        tag_t  
            master,  
            *exps,  
            *children,  
            *parents;  
        report_feature_name(feat);  
        UF_CALL( UF_MODL_ask_feat_type( feat, &feat_type));  
        printf("Feature type: %s\n", feat_type);  
        if(strcmp( feat_type, "INSTANCE") == 0)  
        {  
            printf("...asking for master feature...\n");  
            UF_CALL( UF_MODL_ask_master( feat, &master));  
        }  
        else  
            master = feat;  
        UF_free( feat_type);  
        if( !UF_CALL( UF_MODL_ask_exps_of_feature( master, &num_exps, &exps)) &&  
                         (num_exps > 0 ))  
        for ( ii = 0 ; ii < num_exps; ii++)  
        {  
            UF_CALL( UF_MODL_ask_exp_tag_string( exps[ii], &exp_str));  
            printf("    %s\n", exp_str);  
            UF_free(exp_str);  
        }  
        UF_CALL(UF_MODL_ask_feat_relatives(feat, &n_parents, &parents, &n_children,  
            &children));  
        printf("    %d parent(s): \n", n_parents);  
        for (ii = 0; ii < n_parents; ii++)  
        {  
            printf("        ");  
            report_feature_name(parents[ii]);  
        }  
        UF_free(parents);  
        printf("    %d children: \n", n_children);  
        for (ii = 0; ii < n_children; ii++)  
        {  
            printf("        ");  
            report_feature_name(children[ii]);  
        }  
        UF_free(children);  
        UF_free( exps);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat;  
        printf("\n\n");  
        while ((feat = select_a_feature("Select feature")) != NULL_TAG)  
            report_feature_relatives(feat);  
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

> 根据代码内容，这是一个NX Open C++ API编写的NX二次开发示例程序，主要实现了以下功能：
>
> 1. 定义了错误报告函数report_error，用于打印出调用NX API函数时的错误信息。
> 2. 定义了选择特征的函数select_a_feature，使用NX的UI API让用户选择一个特征，并返回特征tag。
> 3. 定义了报告特征名称的函数report_feature_name，使用NX的建模API获取特征的名称并打印。
> 4. 定义了报告特征相关信息的函数report_feature_relatives，它会打印特征类型、表达式、父特征和子特征等信息。
> 5. 定义了主函数do_it，它会循环调用select_a_feature选择特征，然后调用report_feature_relatives打印特征信息。
> 6. 定义了ufusr函数，这是NX二次开发程序的入口函数，在这里初始化NX系统，调用do_it执行功能，然后终止NX系统。
> 7. 定义了ufusr_ask_unload函数，在程序卸载时被调用。
>
> 总体来说，这个程序利用NX的UI和建模API，实现了选择特征并打印特征详细信息的功能。通过这个示例程序，可以学习到NX二次开发的基本流程和常用API的使用方法。
>
