### 【1375】highlight all route stock 高亮所有布线库

#### 代码

```cpp
    /*HEAD HIGHLIGHT_ALL_ROUTE_STOCK CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_layer.h>  
    #include <uf_view.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是一个在V18版本中新增的函数。文档中提到："UF_print_syslog是V18版本新增的"，因此，UF_print_syslog是V18版本新增的函数。 */  
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
    static int ask_all_route_stock(tag_t **stock)  
    {  
        int  
            flag,  
            ii,  
            old_layer_settings[UF_LAYER_MAX_LAYER],  
            subtype,  
            type;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            list;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        for (ii = 1; ii <= UF_LAYER_MAX_LAYER; ii++)  
            UF_CALL(UF_LAYER_ask_status(ii, &old_layer_settings[ii-1]));  
        for (ii = 1; ii <= UF_LAYER_MAX_LAYER; ii++)  
            UF_CALL(UF_LAYER_set_status(ii, UF_LAYER_ACTIVE_LAYER));  
        UF_CALL(UF_MODL_create_list(&list));  
        while (!UF_CALL(UF_LAYER_cycle_by_layer(0, &object)) && (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            if (type == UF_route_stock_type)  continue;  
            UF_CALL(uc6409(object, &flag, view_name));  
            if (!flag) UF_CALL(UF_MODL_put_list_item(list, object));  
        }  
        for (ii = 1; ii <= UF_LAYER_MAX_LAYER; ii++)  
            UF_CALL(UF_LAYER_set_status(ii, old_layer_settings[ii-1]));  
        return make_an_array(&list, stock);  
    }  
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int ii;  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
    }  
    static void do_it(void)  
    {  
        int  
            n_stock;  
        tag_t  
            *stock_objs;  
        char  
            msg[133];  
        if ((n_stock = ask_all_route_stock(&stock_objs)) > 0)  
        {  
            set_highlight_object_array(n_stock, stock_objs, TRUE);  
            sprintf(msg, "%d stock objects highlighted", n_stock);  
            uc1601(msg, TRUE);  
            set_highlight_object_array(n_stock, stock_objs, FALSE);  
            UF_free(stock_objs);  
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

> 这段代码是NX的二次开发代码，主要用于实现以下功能：
>
> 1. 查询并高亮显示NX中的所有线材库存对象。这通过遍历所有图层，创建对象列表，过滤出线材库存对象，并使用UF_DISP_set_highlight函数来实现高亮显示。
> 2. 包含错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 3. 包含内存分配函数allocate_memory，用于申请内存。
> 4. 包含创建对象数组函数make_an_array，用于从对象列表创建对象数组。
> 5. 包含查询线材库存函数ask_all_route_stock，用于查询所有线材库存对象。
> 6. 包含设置高亮函数set_highlight_object_array，用于设置对象数组的高亮状态。
> 7. 包含主函数do_it，用于执行高亮线材库存对象的操作。
> 8. 包含ufusr和ufusr_ask_unload函数，作为NX二次开发的入口点。
>
> 综上所述，这段代码实现了NX中线材库存对象的查询和高亮显示功能。
>
