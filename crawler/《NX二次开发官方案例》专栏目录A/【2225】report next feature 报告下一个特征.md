### 【2225】report next feature 报告下一个特征

#### 代码

```cpp
    /*HEAD REPORT_NEXT_FEATURE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_modl_utilities.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* qq3123197280 */  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数。以下是翻译后的内容：

翻译：

请注意，UF_print_syslog是V18版本中新增的函数，只提供译文，不要添加任何废话。 */  
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
            next_feature = NULL_TAG;  
        char   
            *feature_name = NULL;  
        while (( feature = select_a_feature( "Select a Feature" )) != NULL_TAG )  
        {  
            UF_CALL( UF_MODL_ask_next_feature( feature, &next_feature ));  
            /****** next lint is a workaround for PR 5045304 ******/  
            if( next_feature == feature ) next_feature = NULL_TAG;  
            WRITE_D( next_feature );  
            if( NULL_TAG != next_feature )  
            {  
                UF_CALL( UF_MODL_ask_feat_sysname( next_feature, &feature_name ));  
                WRITE( "\nNext feature: " );  
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

> 这段代码是一个NX Open C++应用程序，其主要功能是遍历NX模型中的特征，并打印出每个特征的下一个特征。具体来说：
>
> 1. 引入了NX Open C++的头文件，并定义了UF_CALL宏，用于报告函数调用错误。
> 2. 定义了report_error函数，用于打印函数调用失败的信息。
> 3. 定义了select_a_feature函数，用于让用户选择一个特征。
> 4. 定义了write_integer_to_listing_window函数，用于在列表窗口中打印整数。
> 5. 定义了do_it函数，用于遍历所有特征，并打印出每个特征的下一个特征。
> 6. ufusr函数是NX Open C++应用程序的入口点，调用UF_initialize初始化NX系统，然后调用do_it函数遍历特征，最后调用UF_terminate终止NX系统。
> 7. ufusr_ask_unload函数用于告诉NX系统立即卸载应用程序。
> 8. 整个应用程序通过遍历模型中的每个特征，并调用UF_MODL_ask_next_feature获取下一个特征，然后打印出下一个特征的信息。
> 9. 应用程序使用了NX Open C++的UI模块和Modeling模块的API函数。
> 10. 应用程序以用户交互的方式运行，让用户选择要遍历的特征。
>
> 这段代码是一个典型的NX Open C++应用程序示例，实现了遍历模型特征并打印信息的功能，并使用了NX Open C++提供的API函数。
>
