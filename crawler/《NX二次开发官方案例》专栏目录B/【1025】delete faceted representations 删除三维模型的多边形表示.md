### 【1025】delete faceted representations 删除三维模型的多边形表示

#### 代码

```cpp
    /*HEAD DELETE_FACETED_REPRESENTATIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是一个在V18版本中新增的函数，用于打印系统日志。 */  
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
    static tag_t cycle_by_type_and_subtype( tag_t part, int type,  
                                           int subtype, tag_t object )  
    {  
        int  
            this_type,  
            this_subtype;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type,   
            &object)) && (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &this_type, &this_subtype));  
            if (this_subtype == subtype) return object;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            count = 0,  
            inx = 0;  
        tag_t  
            my_rep = NULL_TAG,  
            part_tag = NULL_TAG;      
        uf_list_p_t  
            my_list;  
        part_tag = UF_PART_ask_display_part();  
        UF_CALL( UF_MODL_create_list( &my_list ));  
        while ( (my_rep = cycle_by_type_and_subtype( part_tag,   
                                                   UF_faceted_model_type,  
                                                   UF_faceted_model_normal_subtype,  
                                                   my_rep )) != NULL_TAG )  
        {  
            UF_CALL( UF_MODL_put_list_item( my_list, my_rep ));  
        }                                                 
        UF_CALL( UF_MODL_ask_list_count( my_list, &count));  
        for ( inx = 0; inx < count; inx++ )  
        {  
            UF_CALL( UF_MODL_ask_list_item( my_list, inx, &my_rep ));  
            UF_CALL( UF_OBJ_delete_object( my_rep ));  
        }  
        UF_CALL( UF_MODL_delete_list( &my_list ));     
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

> 这段NX二次开发代码的主要功能是删除当前显示部件中的所有普通类型的Faceted Model（多面体模型）。
>
> 代码的主要逻辑包括以下几个步骤：
>
> 1. 初始化NX环境。
> 2. 获取当前显示的部件，并创建一个对象列表。
> 3. 遍历部件中的所有对象，找出类型为Faceted Model且子类型为普通的多面体模型，并将它们添加到列表中。
> 4. 遍历列表中的对象，并调用删除函数，逐一删除这些多面体模型。
> 5. 清理列表资源，终止NX环境。
>
> 在实现过程中，使用了NX提供的UF模块，通过UF_OBJ_cycle_objs_in_part函数遍历部件中的对象，通过UF_OBJ_ask_type_and_subtype函数获取对象的类型和子类型，以及UF_OBJ_delete_object函数删除对象。同时，通过UF_MODL_create_list等函数管理对象列表。在出现错误时，使用UF_get_fail_message函数获取错误信息，并通过UF_print_syslog和UF_UI_write_listing_window函数输出错误日志。
>
> 总体来说，这段代码实现了在NX部件中删除特定类型的多面体模型的功能，采用了面向对象的设计思想，结构清晰，代码复用性好。
>
