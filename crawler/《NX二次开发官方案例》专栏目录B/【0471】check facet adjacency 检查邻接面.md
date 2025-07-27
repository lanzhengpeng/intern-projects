### 【0471】check facet adjacency 检查邻接面

#### 代码

```cpp
    /*HEAD CHECK_FACET_ADJACENCY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_facet.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：

注意：UF_print_syslog是在V18版本中新增的，只回答译文，不要闲聊。 */  
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
    static int display_facet(tag_t model, int facet)  
    {  
        int  
            ii,  
            n;  
        double  
            verts[5][3];  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_FACET_ask_vertices_of_facet(model, facet, &n, verts));  
        for (ii = 0; ii < (n - 1); ii++)  
            UF_CALL(UF_DISP_display_temporary_line(NULL_TAG,  
                UF_DISP_USE_WORK_VIEW, &verts[ii][0], &verts[ii+1][0], &props));  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG,  
            UF_DISP_USE_WORK_VIEW, &verts[n-1][0], &verts[0][0], &props));  
        return n;  
    }  
    static void do_it(void)  
    {  
        int  
            facet = UF_FACET_NULL_FACET_ID,  
            adj_edge,  
            adj_facet,  
            ii,  
            jj,  
            n,  
            n_edges;  
        tag_t  
            body = NULL_TAG,  
            *models,  
            part = UF_PART_ask_display_part();  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
        {  
            UF_CALL(UF_FACET_ask_models_of_solid(body, &n, &models));  
            for (ii = 0; ii < n; ii++)  
            {  
                while (!UF_CALL(UF_FACET_cycle_facets(models[ii], &facet)) &&  
                    (facet != UF_FACET_NULL_FACET_ID))  
                {  
                    n_edges = display_facet(models[ii], facet);  
                    for (jj = 0; jj < n_edges; jj++)  
                    {  
                        UF_CALL(UF_FACET_ask_adjacent_facet(models[ii], facet,  
                            jj, &adj_facet, &adj_edge));  
                        adj_edge = display_facet(models[ii], adj_facet);  
                    }  
    /*  
                    uc1601("OK?", TRUE);  
                    UF_DISP_refresh();  
    */  
                }  
            }  
            UF_free(models);  
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

> 这段代码是一个NX的二次开发示例，用于检查面与面的邻接关系。其主要功能和步骤包括：
>
> 1. 定义错误报告函数report_error，用于在调用UF函数出错时打印错误信息。
> 2. 定义函数ask_next_body，用于遍历部件中的实体，返回下一个实体的tag。
> 3. 定义函数display_facet，用于显示一个面的顶点连线，以便可视化。
> 4. 定义函数do_it，遍历当前显示部件中的所有实体，对每个实体，遍历所有面，显示面的顶点连线，并查询每个面的邻接面，也显示其顶点连线。
> 5. 定义ufusr函数，作为用户自定义函数的入口，初始化UF模块，调用do_it函数，然后终止UF模块。
> 6. 定义ufusr_ask_unload函数，用于卸载用户自定义函数时立即释放内存。
>
> 总体来说，这段代码利用NX提供的UF函数，实现了检查面与面的邻接关系，并通过显示面的顶点连线来可视化每个面及其邻接面，以便于检查面的邻接情况。
>
