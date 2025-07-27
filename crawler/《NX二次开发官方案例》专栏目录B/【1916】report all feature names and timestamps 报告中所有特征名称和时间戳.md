### 【1916】report all feature names and timestamps 报告中所有特征名称和时间戳

#### 代码

```cpp
    /*HEAD REPORT_ALL_FEATURE_NAMES_AND_TIMESTAMPS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的内容，UF_print_syslog是V18版本新增的功能，用于打印系统日志。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static tag_t ask_next_feature(tag_t part, tag_t feat)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE) return feat;  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part(),  
            feature = NULL_TAG;  
        char  
            *feature_name,  
            *feature_sysname,  
            *feature_type;  
        while( ( feature = ask_next_feature( part, feature ) ) != NULL_TAG )  
        {  
            UF_CALL(UF_MODL_ask_feat_name( feature, &feature_name ));  
            WRITE_S(feature_name);  
            UF_free(feature_name);  
            UF_CALL(UF_MODL_ask_feat_sysname( feature, &feature_sysname ));  
            WRITE_S(feature_sysname);  
            UF_free(feature_sysname);  
            UF_CALL(UF_MODL_ask_feat_type( feature, &feature_type ));  
            WRITE_S(feature_type);  
            UF_free(feature_type);  
            WRITE( "\n" );  
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

> 这是段NX的二次开发代码，主要功能是遍历当前显示的部件中的所有特征，并打印每个特征的名字、系统名称和类型。以下是代码的主要内容和流程：
>
> 1. 包含了NX二次开发所需的头文件。
> 2. 定义了UF_CALL宏，用于执行NX的函数，并捕获错误。
> 3. 定义了report_error函数，用于打印执行NX函数时的错误信息。
> 4. 定义了WRITE和WRITE_S宏，用于向列表窗口打印信息。
> 5. 定义了write_string_to_listing_window函数，用于格式化字符串并打印到列表窗口。
> 6. 定义了ask_next_feature函数，用于遍历部件中的所有特征，并返回下一个存活的特征。
> 7. 定义了do_it函数，用于遍历当前显示的部件中的所有特征，并打印每个特征的名字、系统名称和类型。
> 8. 定义了ufusr函数，这是NX二次开发的入口函数。在此函数中初始化NX系统，调用do_it函数，然后终止NX系统。
> 9. 定义了ufusr_ask_unload函数，用于卸载二次开发程序。
>
> 总的来说，这段代码实现了遍历和打印当前显示部件中所有特征信息的功能。通过使用NX提供的API函数，实现了对NX模型的查询操作。
>
