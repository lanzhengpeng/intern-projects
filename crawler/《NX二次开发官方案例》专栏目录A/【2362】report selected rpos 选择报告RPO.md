### 【2362】report selected rpos 选择报告RPO

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_RPOS CCC UFUN */  
    #include <uf_exit.h>  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，关于UF_print_syslog函数，需要注意以下信息：

- UF_print_syslog是V18版本中新增的函数。

所以，针对问题“UF_print_syslog是V18版本中新增的函数”，我的回答是：

UF_print_syslog是V18版本中新增的函数。 */  
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
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    static void do_it(void)  
    {  
        tag_t   feature_tag, *exp_tags;  
        int     count, resp, ii;  
        feature_tag = select_a_feature("Select feature");  
        UF_CALL( UF_UI_select_rpo_dimensions( "Select RPOs", feature_tag,  
                                               &count, &exp_tags, &resp));  
        for( ii = 0; ii < count; ii++)  
            printf("Tag of rpo dimension: %d\n", exp_tags[ii]);  
        UF_free( exp_tags);  
    }  
    extern void ufusr(char *param, int *retcode, int paramLen)  
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

> 这段代码是用于NX的二次开发，其功能是选择一个特征，然后选择该特征的RPO参数，并打印出RPO参数的标签。以下是代码的主要组成部分：
>
> 1. 包含必要的NX头文件，如uf.h、uf_modl.h等，用于调用NX的API。
> 2. 定义宏ECHO和UF_CALL，分别用于打印变量值和调用NX API，并在出错时打印错误信息。
> 3. 定义report_error函数，用于打印错误信息和调用信息。
> 4. 定义select_a_feature函数，用于提示用户选择一个特征，并返回第一个特征的标签。
> 5. 定义do_it函数，首先调用select_a_feature选择特征，然后调用UF_UI_select_rpo_dimensions选择该特征的RPO参数，并打印出RPO参数的标签。
> 6. 定义ufusr函数，这是NX二次开发的入口函数，初始化NX环境，调用do_it执行功能，然后终止NX环境。
> 7. 定义ufusr_ask_unload函数，返回立即卸载标志。
> 8. 整体上，这段代码实现了选择特征并打印其RPO参数的功能，采用了NX提供的UI选择机制，并进行了错误处理。
>
