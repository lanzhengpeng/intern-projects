### 【3150】report node count on CAE meshes and bodies 报告CAE网格和实体上的节点数量

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_sf.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
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
    /*  
        UF_SF_locate_all_meshes does not seem to work - see PR 6407606.  This  
        example function can be used to work around.  
    */  
    static int ask_all_meshes(tag_t **meshes)  
    {  
        tag_t  
            part = UF_ASSEM_ask_work_part();  
        return (ask_all_of_type_and_subtype(part,  UF_sfem_mesh_type, 0, meshes));  
    }  
    static void do_it(void)  
    {  
        int  
            ii, jj,  
            n_bodies,  
            n_meshes,  
            n_nodes,  
            node_id,  
            max_id = 0,  
            min_id = 0;  
        tag_t  
            *bodies,  
            *meshes,  
            *nodes;  
        UF_SF_node_btype_t  
            b_type;  
        UF_SF_mid_node_type_t  
            e_type;  
        double  
            abspos[3];  
        n_meshes = ask_all_meshes(&meshes);  
        ECHO("number of meshes = %d\n", n_meshes);  
        for (ii = 0; ii < n_meshes; ii++)  
        {  
            ECHO("mesh %d:", ii+1);  
            UF_CALL(UF_SF_locate_nodes_on_mesh(meshes[ii], &n_nodes, &nodes));  
            ECHO("  number of nodes = %d\n", n_nodes);  
            if (n_nodes > 0)  
            {  
                UF_CALL(UF_SF_ask_node(nodes[0], &min_id, &b_type, &e_type, abspos));  
                max_id = min_id;  
                for (jj = 1; jj < n_nodes; jj++)  
                {  
                    UF_CALL(UF_SF_ask_node(nodes[jj], &node_id, &b_type, &e_type,  
                        abspos));  
                    if (node_id < min_id) min_id = node_id;  
                    if (node_id > max_id) max_id = node_id;  
                }  
                ECHO("\tMinimum node label = %d, Maximum node label = %d\n",  
                    min_id, max_id);  
                UF_free(nodes);  
            }  
        }  
        if (n_meshes > 0) UF_free(meshes);  
        UF_CALL(UF_SF_ask_all_polygon_bodies(&n_bodies, &bodies));  
        ECHO("\nnumber of bodies = %d\n", n_bodies);  
        for (ii = 0; ii < n_bodies; ii++)  
        {  
            ECHO("bodies %d:", ii+1);  
            UF_CALL(UF_SF_locate_nodes_on_geometry(bodies[ii], UF_SF_SWITCH_ALL,  
                &n_nodes, &nodes));  
            ECHO("  number of nodes = %d\n", n_nodes);  
            if (n_nodes > 0)  
            {  
                UF_CALL(UF_SF_ask_node(nodes[0], &min_id, &b_type, &e_type, abspos));  
                max_id = min_id;  
                for (jj = 1; jj < n_nodes; jj++)  
                {  
                    UF_CALL(UF_SF_ask_node(nodes[jj], &node_id, &b_type, &e_type,  
                        abspos));  
                    if (node_id < min_id) min_id = node_id;  
                    if (node_id > max_id) max_id = node_id;  
                }  
                ECHO("\tMinimum node label = %d, Maximum node label = %d\n",  
                    min_id, max_id);  
                UF_free(nodes);  
            }  
        }  
        if (n_bodies > 0) UF_free(bodies);  
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

> 这是NX的二次开发代码，主要实现了以下功能：
>
> 1. 定义了ECHO函数，用于打印信息到NX的日志窗口和系统日志。
> 2. 定义了UF_CALL宏，用于调用NX的API函数，并打印错误信息。
> 3. 实现了allocate_memory函数，用于分配内存。
> 4. 实现了make_an_array函数，用于将链表转换为数组。
> 5. 实现了ask_all_of_type_and_subtype函数，用于查询指定类型和子类型的对象。
> 6. 实现了ask_all_meshes函数，用于查询所有网格对象。
> 7. 实现了do_it函数，用于遍历所有网格和几何体，统计节点数量和节点编号范围。
> 8. 实现了ufusr函数，作为NX的启动函数，调用do_it函数。
> 9. 实现了ufusr_ask_unload函数，用于卸载NX用户库。
>
> 该代码主要用于查询NX模型中的网格和几何体的节点信息，并统计节点数量和节点编号范围。
>
