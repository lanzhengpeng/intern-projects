### 【3127】name all sheets 遍历当前显示零件中的所有板体，并为每个板体设置一个名称

#### 代码

```cpp
    /*HEAD NAME_ALL_SHEETS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog 是 V18 版本新增的功能，只需提供翻译，无需添加其他内容。 */  
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
    static tag_t ask_next_sheet_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype)  
            {  
                UF_CALL(UF_MODL_ask_body_type(body, &type));  
                if (type == UF_MODL_SHEET_BODY) return body;  
            }  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            ii = 0;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            sheet = NULL_TAG;  
        char  
            name[MAX_ENTITY_NAME_SIZE+1];  
        while ((sheet = ask_next_sheet_body(part, sheet)) != NULL_TAG)  
        {  
            sprintf(name, "sheet_%d", ++ii);  
            UF_CALL(UF_OBJ_set_name(sheet, name));  
        }  
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

> 这段代码是一个NX Open C++ API的二次开发示例，主要功能是遍历当前显示部件中的所有薄片体，并为它们重新命名。以下是代码的主要功能：
>
> 1. 引入NX Open C++ API的头文件，包括UF、UF_UI、UF_OBJ、UF_OBJECT_TYPES、UF_MODL等模块。
> 2. 定义了一个错误报告函数report_error，用于在发生API调用错误时打印错误信息。
> 3. 定义了一个获取下一个薄片体的函数ask_next_sheet_body，它遍历部件中的所有实体，并检查是否为薄片体，如果是则返回该薄片体的tag。
> 4. 定义了一个do_it函数，用于遍历所有薄片体，并为它们按顺序命名，例如"sheet_1"、"sheet_2"等。
> 5. 定义了ufusr函数，这是二次开发的入口函数。在这个函数中，首先初始化NX Open API，然后调用do_it函数执行主体功能，最后终止API。
> 6. 定义了一个卸载函数ufusr_ask_unload，返回立即卸载标志。
>
> 总体来说，这段代码通过NX Open C++ API遍历当前显示部件中的所有薄片体，并为它们重新命名，实现了简单的NX二次开发功能。
>
