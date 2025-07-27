### 【1008】delete all layer categories 删除所有图层类别

#### 代码

```cpp
    /*HEAD DELETE_ALL_LAYER_CATEGORIES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中新增的函数。 */  
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
    static tag_t ask_next_layer_category(tag_t part, tag_t cat)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_layer_category_type, &cat));  
        return cat;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static int ask_all_layer_categories(tag_t part, tag_t **categories)  
    {  
        tag_t  
            category = NULL_TAG;  
        uf_list_p_t  
            cat_list;  
        UF_CALL(UF_MODL_create_list(&cat_list));  
        while ((category = ask_next_layer_category(part, category)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(cat_list, category));  
        return (make_an_array(&cat_list, categories));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_cats;  
        tag_t  
            *cats,  
            part = UF_PART_ask_display_part();  
        char  
            cat_name[MAX_ENTITY_NAME_SIZE+1];  
        n_cats = ask_all_layer_categories(part, &cats);  
        for (ii = 0; ii < n_cats; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_name(cats[ii], cat_name));  
            if (strcmp(cat_name, "ALL"))  
                UF_CALL(UF_OBJ_delete_object(cats[ii]));  
        }  
        if (n_cats > 0) UF_free(cats);  
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

> 这段NX二次开发代码的主要功能是删除NX部件中的所有图层分类，除了名为"ALL"的分类。
>
> 主要步骤包括：
>
> 1. 初始化NX环境。
> 2. 获取当前显示的部件。
> 3. 遍历部件中的所有图层分类，并获取它们的标签。
> 4. 对于每个分类，判断其名称是否为"ALL"，如果不是，则删除该分类。
> 5. 释放内存。
> 6. 结束NX环境。
>
> 代码中使用了NX提供的UF函数来实现以上功能，如UF_OBJ_cycle_objs_in_part遍历分类，UF_OBJ_delete_object删除分类，UF_MODL_create_list和UF_MODL_put_list_item创建并填充分类列表等。
>
> 此外，代码还包含了一个错误报告函数report_error，用于在UF函数调用失败时输出错误信息。
>
> 整体来看，这段代码结构清晰，实现了删除所有非"ALL"分类的功能，适用于NX二次开发中的图层管理。
>
