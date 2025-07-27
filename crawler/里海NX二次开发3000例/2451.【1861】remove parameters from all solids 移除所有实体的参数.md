### 【1861】remove parameters from all solids 移除所有实体的参数

#### 代码

```cpp
    /*HEAD REMOVE_PARAMETERS_FROM_ALL_SOLIDS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，我理解到UF_print_syslog是在V18版本中新增的函数。这个函数用于在系统日志中打印信息。 */  
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
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        do  
        {  
            UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body));  
            if (body)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
                if (!subtype) return body;  
            }  
        } while (body);  
        return NULL_TAG;  
    }  
    static uf_list_p_t make_a_list( int count, tag_t *item_array)  
    {  
        uf_list_p_t list;  
        int         ii;  
        UF_CALL( UF_MODL_create_list( &list));  
        for( ii = 0; ii < count; ii++)  
            UF_CALL( UF_MODL_put_list_item( list, item_array[ii]));  
        return( list);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            body = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        uf_list_p_t  
            body_list;  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
        {  
            body_list = make_a_list(1, &body);  
            UF_CALL(UF_MODL_delete_body_parms(body_list));  
            UF_CALL(UF_MODL_delete_list(&body_list));  
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

> 这是段用于NX Open的二次开发代码，其主要功能是移除当前显示部件中所有实体的参数。以下是代码的主要内容和逻辑：
>
> 1. 包含了NX Open所需的头文件，定义了错误报告函数report_error，用于在发生错误时打印错误信息。
> 2. 定义了ask_next_body函数，用于遍历当前部件中的实体，并返回下一个实体，跳过子实体。
> 3. 定义了make_a_list函数，用于创建一个包含实体标签的列表。
> 4. 定义了do_it函数，用于遍历当前部件中的所有实体，创建实体列表，并调用UF_MODL_delete_body_parms删除实体的参数。
> 5. 定义了ufusr函数，这是NX Open的二次开发入口函数。在ufusr中，初始化NX Open，调用do_it执行功能，然后终止NX Open。
> 6. 定义了ufusr_ask_unload函数，用于告诉NX立即卸载该二次开发代码。
>
> 通过遍历当前部件中的所有实体，并删除它们的参数，该代码实现了移除实体参数的功能。这段代码遵循了NX Open的二次开发规范，并使用了NX Open提供的API函数来实现所需功能。
>
