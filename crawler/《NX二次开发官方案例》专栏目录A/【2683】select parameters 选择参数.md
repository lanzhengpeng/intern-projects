### 【2683】select parameters 选择参数

#### 代码

```cpp
    /*HEAD SELECT_PARAMETERS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
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
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
        int   
            inx = 0,  
            count = 0,  
            response = 0;     
        tag_t   
            *exp_tags = NULL,  
            feat = NULL_TAG;  
        while((feat=select_a_feature("Pick Feature:"))!=NULL_TAG)  
        {  
            UF_CALL(UF_UI_select_parameters("Feature:", feat, &count,   
                                                 &exp_tags,&response ));  
            if( response != UF_UI_CANCEL )  
            {  
                WRITE_D( count );  
                for(inx = 0; inx < count; inx++ )  
                {  
                    WRITE_D( exp_tags[inx] );  
                }  
                if( count > 0 ) UF_free( exp_tags );  
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

> 这段代码是一个NX二次开发的示例，主要功能是让用户选择特征，然后显示该特征的所有参数。代码主要包含以下几个部分：
>
> 1. 错误报告函数：用于在调用NX API出错时报告错误信息。
> 2. 写入列表窗口函数：用于在NX的列表窗口中写入信息。
> 3. 选择特征函数：用于提示用户选择一个特征。
> 4. 执行函数：循环调用选择特征函数，获取用户选择的特征，然后调用NX API获取特征的参数并显示。
> 5. ufusr函数：NX二次开发程序的入口函数，在这里初始化NX，调用执行函数，然后终止NX。
> 6. 卸载函数：当二次开发程序被卸载时调用，这里直接返回立即卸载。
>
> 通过这个程序，用户可以方便地选择特征并查看其参数，有助于理解和操作NX中的特征。
>
