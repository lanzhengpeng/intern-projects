### 【2226】report node count on all cae bodies 报告所有CAE体的节点数

#### 代码

```cpp
    /*HEAD REPORT_NODE_COUNT_ON_ALL_CAE_BODIES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_sf.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的，仅回答翻译，不要无关内容。 */  
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
    static int ask_all_of_type_and_subtype(tag_t part, int type, int subtype,  
        tag_t **objects)  
    {  
        int  
            this_type,  
            this_subtype;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object)) &&  
                (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &this_type, &this_subtype));  
            if (this_subtype == subtype)  
                (UF_MODL_put_list_item(object_list, object));  
        }  
        return (make_an_array(&object_list, objects));  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_bodies,  
            n_nodes;  
        tag_t  
            *cae_bodies,  
            *nodes,  
            part = UF_PART_ask_display_part();  
        char  
            msg[MAX_LINE_SIZE+1];  
        n_bodies = ask_all_of_type_and_subtype(part, UF_caegeom_type,  
            UF_caegeom_body_subtype, &cae_bodies);  
        WRITE_D(n_bodies);  
        for (ii= 0; ii < n_bodies; ii++)  
        {  
            UF_CALL(UF_SF_locate_nodes_on_geometry(cae_bodies[ii],  
                UF_SF_SWITCH_ALL, &n_nodes, &nodes));  
            if (n_nodes > 0)  
            {  
                UF_CALL(UF_DISP_set_highlight(cae_bodies[ii], TRUE));  
                sprintf(msg, "CAE Body has %d nodes", n_nodes);  
                uc1601(msg, TRUE);  
                UF_CALL(UF_DISP_set_highlight(cae_bodies[ii], FALSE));  
                UF_free(nodes);  
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

> 这段代码是一个NX Open的UF程序，主要用于在NX的CAE几何体上查找并高亮显示节点。
>
> 主要步骤包括：
>
> 1. 定义了错误报告函数report_error，用于在函数调用失败时报告错误。
> 2. 定义了内存分配函数allocate_memory，用于分配内存。
> 3. 定义了列表处理函数make_an_array，用于从列表中提取对象数组并释放列表。
> 4. 定义了根据类型和子类型查找对象的函数ask_all_of_type_and_subtype。
> 5. 定义了写整数到列表窗口的函数write_integer_to_listing_window。
> 6. 定义了主函数do_it，用于查找并高亮显示CAE几何体上的节点。
> 7. 定义了UF主函数ufusr，用于初始化、调用do_it和终止。
> 8. 定义了卸载函数ufusr_ask_unload，用于立即卸载。
>
> 该程序的主要功能是在NX的CAE几何体上查找节点，并高亮显示节点数量大于0的CAE几何体。
>
