### 【2124】report feature sysname using uf modl ask feat sysname 报告使用UF_MODL询问特征系统名称的功能系统名称

#### 代码

```cpp
    /*HEAD REPORT_FEATURE_SYSNAME_USING_UF_MODL_ASK_FEAT_SYSNAME CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_modl_utilities.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 的新增功能。

只回答翻译，不要添加其他内容。 */  
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
    /* qq3123197280 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    /* qq3123197280 */  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* qq3123197280 */  
    static void do_it(void)  
    {  
        tag_t  
            feature = NULL_TAG;  
        char   
            *feature_name = NULL;  
        while (( feature = select_a_feature( "Select a Feature" )) != NULL_TAG )  
        {  
            if( NULL_TAG != feature )  
            {  
                WRITE( "\n" );  
                WRITE_D( feature );  
                UF_CALL( UF_MODL_ask_feat_sysname( feature, &feature_name ));  
                WRITE( "Feature sysname: " );  
                WRITE( feature_name );  
                WRITE( "\n" );  
                UF_free( feature_name );  
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

> 这段NX二次开发代码实现了以下功能：
>
> 1. 定义了错误报告函数report_error，用于在发生UF函数调用错误时打印错误信息。
> 2. 定义了选择特征函数select_a_feature，用于弹出一个对话框，让用户选择一个特征。
> 3. 定义了整数写入函数write_integer_to_listing_window，用于将整数写入NX日志窗口。
> 4. 定义了主函数do_it，循环调用select_a_feature函数，获取用户选择的一个特征，然后调用UF函数UF_MODL_ask_feat_sysname获取该特征的系统名称，并输出到日志窗口。
> 5. 定义了UF初始化函数ufusr，在NX启动时调用do_it函数。
> 6. 定义了卸载函数ufusr_ask_unload，在卸载时立即返回。
>
> 通过以上功能，该代码实现了在NX环境中循环获取用户选择的特征，并显示其系统名称的功能。
>
