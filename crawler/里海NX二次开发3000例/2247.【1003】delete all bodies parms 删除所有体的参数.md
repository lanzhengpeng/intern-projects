### 【1003】delete all bodies parms 删除所有体的参数

#### 代码

```cpp
    /*HEAD DELETE_ALL_BODIES_PARMS CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog是在V18版本中新增的函数。文档只要求回答翻译，因此不需要添加任何其他内容，只需翻译文档中的关键信息即可。 */  
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

> 这段代码是一个NX二次开发程序，其主要功能是删除当前显示部件中的所有实体参数。具体步骤如下：
>
> 1. 包含NX二次开发所需的相关头文件。
> 2. 定义了一个错误报告函数report_error，用于输出错误信息。
> 3. 定义了一个循环查询部件中下一个实体的函数ask_next_body。
> 4. 定义了一个创建实体列表的函数make_a_list。
> 5. 定义了一个删除实体参数的主函数do_it。
> 6. 主函数中首先获取当前显示的部件，然后循环查询部件中的每个实体，创建实体列表，调用UF_MODL_delete_body_parms删除实体的参数，然后删除实体列表。
> 7. ufusr函数是程序的入口函数，在初始化NX环境后调用do_it函数，然后终止NX环境。
> 8. ufusr_ask_unload函数用于卸载程序，这里设置为立即卸载。
>
> 综上，这段代码的主要功能是删除当前显示部件中的所有实体参数，以清理实体数据。
>
