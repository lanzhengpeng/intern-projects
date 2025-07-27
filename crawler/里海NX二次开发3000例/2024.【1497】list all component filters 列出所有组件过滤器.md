### 【1497】list all component filters 列出所有组件过滤器

#### 代码

```cpp
    /*HEAD LIST_ALL_COMPONENT_FILTERS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_object_types.h>  
    #include <uf_fltr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据提供的信息，翻译如下：

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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static tag_t ask_next_of_type(tag_t part, int type, tag_t object)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object));  
        return (object);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            filter = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            condition[133],  
            name[MAX_ENTITY_NAME_SIZE+1];  
        while ((filter = ask_next_of_type(part, UF_filter_type, filter)) !=  
            NULL_TAG)  
        {  
            UF_CALL(UF_FLTR_ask_filter(filter, name, condition));  
            WRITE_S(name);  
            WRITE_S(condition);  
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

> 这段代码是一个NX的二次开发示例，其主要功能是遍历当前显示部件中的所有过滤器，并打印它们的名称和条件。代码的主要结构如下：
>
> 1. 头文件引入：引入了必要的NX API头文件，包括UF、UI、OBJ、PART、OBJECT_TYPES和FLTR。
> 2. 错误报告函数：定义了一个错误报告函数report_error，用于在API调用出错时打印错误信息。
> 3. 字符串写入函数：定义了一个字符串写入函数write_string_to_listing_window，用于将信息写入NX的列表窗口。
> 4. 遍历部件中的对象：定义了一个ask_next_of_type函数，用于遍历部件中的指定类型的对象。
> 5. 主函数：定义了一个do_it函数，用于遍历当前显示部件中的所有过滤器，并打印它们的名称和条件。
> 6. UFusr入口函数：定义了NX二次开发的入口函数ufusr，用于初始化NX，调用do_it函数，并终止NX。
> 7. 卸载函数：定义了一个卸载函数ufusr_ask_unload，用于在卸载时立即卸载。
>
> 通过上述代码结构，实现了遍历部件中的过滤器并打印名称和条件的功能。
>
