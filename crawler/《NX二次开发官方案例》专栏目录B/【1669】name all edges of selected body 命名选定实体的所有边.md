### 【1669】name all edges of selected body 命名选定实体的所有边

#### 代码

```cpp
    /*HEAD NAME_ALL_EDGES_OF_SELECTED_BODY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 中新增的函数。

翻译：UF_print_syslog 是 V18 中新增的函数，请只回答译文，不要添加其他内容。 */  
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
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a body", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
    static int ask_body_edges(tag_t body, tag_p_t *body_edges)  
    {  
        uf_list_p_t  
            edge_list;  
        UF_CALL(UF_MODL_ask_body_edges(body, &edge_list));  
        return make_an_array(&edge_list, body_edges);  
    }  
    static void do_it(void)  
    {  
        int  
            inx,  
            count;  
        tag_t  
            body,  
            *edges;  
        char  
            name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL( UF_DISP_set_name_display_status ( UF_DISP_NAME_DISPLAY_ON ));  
        while ((body = select_a_body("Name Edges")) != NULL_TAG)  
        {  
            count = ask_body_edges(body, &edges);  
            for (inx = 0; inx < count; inx++ )  
            {  
                sprintf( name, "EDGE_%d", inx+1);  
                UF_CALL( UF_OBJ_set_name( edges[inx], name ));  
            }  
            if (count > 0) UF_free(edges);  
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

> 根据代码，这是一段用于NX的二次开发代码，主要功能是选择一个实体，并将其所有边线重新命名。
>
> 主要步骤包括：
>
> 1. 定义错误报告函数report_error，用于在出错时打印错误信息。
> 2. 定义选择实体的函数select_a_body，使用UF_UI接口让用户选择一个实体。
> 3. 定义申请内存的函数allocate_memory，用于申请指定大小的内存。
> 4. 定义从链表提取数据的函数make_an_array，用于提取UF_MODL_ask_body_edges得到的边线链表数据到数组。
> 5. 定义查询实体所有边线的函数ask_body_edges，使用UF_MODL接口查询实体所有边线。
> 6. 定义主函数do_it，循环让用户选择实体，然后提取该实体的所有边线并重命名。
> 7. 定义ufusr函数，初始化NX后调用do_it函数，最后终止NX。
> 8. 定义卸载函数ufusr_ask_unload，返回立即卸载标志。
>
> 总体来说，这段代码实现了选择实体、提取边线并重命名的功能，为NX的二次开发提供了一个简单的示例。
>
