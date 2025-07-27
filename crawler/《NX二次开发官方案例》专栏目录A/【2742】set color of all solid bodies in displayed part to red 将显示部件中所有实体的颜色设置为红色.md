### 【2742】set color of all solid bodies in displayed part to red 将显示部件中所有实体的颜色设置为红色

#### 代码

```cpp
    /*HEAD SET_COLOR_OF_ALL_SOLID_BODIES_IN_DISPLAYED_PART_TO_RED CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，V18中新增了UF_print_syslog函数，用于打印系统日志。 */  
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
    /* qq3123197280 */  
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype) return body;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            body = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        while((body = ask_next_body( part, body)) && NULL_TAG != body )  
        {  
            if( UF_ASSEM_is_occurrence( body ) == FALSE )  
            {  
                UF_CALL( UF_OBJ_set_color( body, UF_OBJ_RED ));  
            }  
        }  
        UF_MODL_update();  
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

> 这段代码是一个NX Open C++ API编写的二次开发程序，其主要功能是遍历显示部件中的所有实体，并将它们的颜色设置为红色。
>
> 主要内容包括：
>
> 1. 包含了必要的NX Open C++ API头文件。
> 2. 定义了一个错误报告函数report_error，用于打印出调用NX API时的错误信息。
> 3. 定义了一个辅助函数ask_next_body，用于遍历部件中的实体，并返回下一个实体的标签。
> 4. 定义了一个do_it函数，遍历显示部件中的所有实体，并设置它们的颜色为红色。注意判断实体是否为直接实体，避免修改实例的颜色。
> 5. 定义了ufusr函数，作为程序的入口函数。在这里初始化NX，调用do_it函数，然后终止NX。
> 6. 定义了ufusr_ask_unload函数，表示该程序不需要卸载，可以直接终止。
> 7. 使用宏UF_CALL包装了所有NX API调用，便于错误处理。
> 8. 使用了NX Open C++ API中的UF_OBJ, UF_PART, UF_ASSEM等模块进行操作。
>
> 总体来说，这段代码结构清晰，功能明确，是一个典型的NX二次开发程序示例。
>
