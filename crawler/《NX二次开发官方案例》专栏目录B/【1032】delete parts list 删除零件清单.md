### 【1032】delete parts list 删除零件清单

#### 代码

```cpp
    /*HEAD DELETE_PARTS_LIST CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog是V18版本中的新功能，用于打印系统日志信息。

翻译如下：

UF_print_syslog是V18版本中新增的功能，用于打印系统日志信息。 */  
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
    static tag_t ask_parts_list(tag_t part)  
    {  
        int  
            subtype,  
            type;  
        tag_t  
            plist = NULL_TAG;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_tabular_note_type,  
            &plist)) && (plist != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(plist, &type, &subtype));  
            if (subtype == UF_parts_list_subtype) return plist;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            plist,  
            part = UF_PART_ask_display_part();  
        if ((plist = ask_parts_list(part)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_delete_object(plist));  
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

> 根据代码内容，这是一段用于删除NX零件列表的二次开发代码。
>
> 主要功能包括：
>
> 1. 初始化和终止：代码首先调用UF_initialize()进行NX初始化，并在结束时调用UF_terminate()终止NX。
> 2. 获取当前显示零件：通过UF_PART_ask_display_part()获取当前显示的零件。
> 3. 遍历零件中的表格注释：使用UF_OBJ_cycle_objs_in_part()遍历零件中的所有表格注释对象。
> 4. 判断表格注释类型：通过UF_OBJ_ask_type_and_subtype()判断表格注释的类型和子类型，如果子类型是UF_parts_list_subtype（零件列表），则将其删除。
> 5. 删除零件列表：通过UF_OBJ_delete_object()删除找到的零件列表对象。
> 6. 错误处理：在UF调用前后使用宏UF_CALL()进行错误处理，打印错误信息。
> 7. 卸载函数：提供卸载函数ufusr_ask_unload()，返回UF_UNLOAD_IMMEDIATELY表示立即卸载。
>
> 这段代码的主要功能是遍历当前显示零件中的表格注释，找到类型为零件列表的表格注释并删除，从而实现删除零件列表的功能。
>
