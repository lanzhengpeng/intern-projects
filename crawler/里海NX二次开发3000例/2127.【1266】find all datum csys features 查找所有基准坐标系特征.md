### 【1266】find all datum csys features 查找所有基准坐标系特征

#### 代码

```cpp
    /*HEAD FIND_ALL_DATUM_CSYS_FEATURES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据您提供的提示，UF_print_syslog是V18版本中新增的函数。

翻译如下：

注意：UF_print_syslog是V18版本新增的函数。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    /* qq3123197280 */  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* qq3123197280 */  
    static tag_t ask_next_feature(tag_t part, tag_t feat)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
            if (UF_OBJ_ask_status(feat) == 3) return feat;  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            msg[133],  
            *type = { "Type unknown" };  
        UF_UI_open_listing_window();  
        while ((feat = ask_next_feature(part, feat)) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_feat_type(feat, &type));  
            if( strstr( type, "DATUM_CSYS" ) != NULL )  
            {  
                sprintf( msg, "%d is a DATUM_CSYS feature\n", feat);  
                UF_UI_write_listing_window( msg );  
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

> 这段NX Open C++代码实现了查找当前显示部件中所有DATUM_CSYS特征的功能。具体代码功能如下：
>
> 1. 定义了错误报告函数report_error，用于输出错误信息。
> 2. 定义了字符串输出函数write_string_to_listing_window，用于将字符串输出到列表窗口。
> 3. 定义了ask_next_feature函数，用于遍历部件中的特征对象，并返回下一个激活的特征对象。
> 4. 定义了do_it函数，用于遍历当前显示部件中的所有特征，检查特征类型是否为DATUM_CSYS，如果是，则输出特征id。
> 5. 定义了ufusr函数，作为程序的入口函数。在ufusr中，先调用UF_initialize初始化NX Open，然后调用do_it执行查找DATUM_CSYS特征的功能，最后调用UF_terminate结束NX Open。
> 6. 定义了ufusr_ask_unload函数，用于处理用户请求卸载事件，返回立即卸载。
>
> 总体来说，这段代码利用NX Open C++ API，实现了在当前显示的NX部件中查找所有DATUM_CSYS特征的功能，并将找到的特征id输出到列表窗口。
>
