### 【2287】report previous feature 遍历选择特征并报告每个特征前一个特征的功能

#### 代码

```cpp
    /*HEAD REPORT_PREVIOUS_FEATURE CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的函数。 */  
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
            feature = NULL_TAG,  
            prev_feature = NULL_TAG;  
        char   
            *feature_name = NULL;  
        while (( feature = select_a_feature( "Select a Feature" )) != NULL_TAG )  
        {  
            UF_CALL( UF_MODL_ask_previous_feature( feature, &prev_feature ));  
                        /*****     workaround for PR 2041495     *****/  
            if( prev_feature == feature ) prev_feature = NULL_TAG;  
            /*--------------------------------------------------*/  
            WRITE_D( prev_feature );  
            if( NULL_TAG != prev_feature )  
            {  
                UF_CALL( UF_MODL_ask_feat_sysname( prev_feature, &feature_name ));  
                WRITE( "\nPrevious feature: " );  
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

> 这是一段用于NX的二次开发代码，其主要功能如下：
>
> 1. 错误报告函数：实现了report_error函数，用于在发生UF函数调用错误时打印错误信息。
> 2. 特征选择函数：实现了select_a_feature函数，用于让用户在图形窗口中选择一个特征，并返回其tag。
> 3. 整数打印函数：实现了write_integer_to_listing_window函数，用于在日志窗口打印整数值。
> 4. 功能实现：在do_it函数中，循环调用select_a_feature获取用户选择的一个特征，然后调用UF_MODL_ask_previous_feature查询该特征的前一个特征，并打印其tag和名称。
> 5. 主函数：在ufusr函数中，首先初始化UF环境，然后调用do_it执行功能，最后终止UF环境。
> 6. 卸载询问函数：实现了ufusr_ask_unload函数，用于在模块卸载时询问立即卸载还是等待。
> 7. 函数调用封装：使用了宏UF_CALL封装了UF函数调用，简化了错误处理。
> 8. 错误打印封装：使用了宏WRITE和WRITE_D封装了日志窗口的打印，简化了日志打印操作。
>
> 总体来说，该代码实现了在NX中查询并打印特征的前一个特征的功能，具有较好的封装性和可维护性。
>
