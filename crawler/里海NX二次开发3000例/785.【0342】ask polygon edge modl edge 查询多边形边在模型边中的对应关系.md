### 【0342】ask polygon edge modl edge 查询多边形边在模型边中的对应关系

#### 代码

```cpp
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
    static tag_t cycle_by_name_type_and_subtype(tag_t part, char *name, int type,  
        int subtype, tag_t object)  
    {  
        int  
            this_type,  
            this_subtype;  
        while (!UF_CALL(UF_OBJ_cycle_by_name_and_type(part, name, type, TRUE,  
            &object)) && (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &this_type, &this_subtype));  
            if (this_subtype == subtype) return object;  
        }  
        return NULL_TAG;  
    }  
    static int ask_body_edges_of_name(tag_t body, char *name, tag_p_t *body_edges)  
    {  
        tag_t  
            edge = NULL_TAG,  
            part;  
        uf_list_p_t  
            edge_list;  
        UF_CALL(UF_OBJ_ask_owning_part(body, &part));  
        UF_CALL(UF_MODL_create_list(&edge_list));  
        while ((edge = cycle_by_name_type_and_subtype(part, name,  
            UF_solid_type, UF_solid_edge_subtype, edge)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(edge_list, edge));  
        return make_an_array(&edge_list, body_edges);  
    }  
    static int ask_polygon_split_edges(tag_t edge, tag_p_t *split_edges)  
    {  
        logical  
            match;  
        int  
            ii, jj,  
            n_adj_edges,  
            n_faces,  
            n_t_faces;  
        tag_t  
            *adj_edges,  
            *faces,  
            *t_faces;  
        uf_list_p_t  
            edge_list;  
        UF_CALL(UF_MODL_create_list(&edge_list));  
        UF_CALL(UF_SF_edge_ask_faces(edge, &n_faces, &faces));  
        qsort(faces, n_faces, sizeof(tag_t), strcmp);  
    /*  Find any adjacent edges which share the same faces 译:找到任何共享相同面的相邻边。 */  
        UF_CALL(UF_SF_edge_ask_adjacent_edges(edge, &n_adj_edges, &adj_edges));  
        for (ii = 0; ii < n_adj_edges; ii++)  
        {  
            match = FALSE;  
            UF_CALL(UF_SF_edge_ask_faces(adj_edges[ii], &n_t_faces, &t_faces));  
            if (n_t_faces == n_faces)  
            {  
                match = TRUE;  
                qsort(t_faces, n_faces, sizeof(tag_t), strcmp);  
                for (jj = 0; jj < n_faces; jj++)  
                {  
                    if (faces[jj] != t_faces[jj])  
                    {  
                        match = FALSE;  
                        break;  
                    }  
                }  
            }  
            if (n_t_faces > 0) UF_free(t_faces);  
            if (match) UF_CALL(UF_MODL_put_list_item(edge_list, adj_edges[ii]));  
        }  
        if (n_adj_edges > 0) UF_free(adj_edges);  
        return make_an_array(&edge_list, split_edges);  
    }  
    static tag_t ask_polygon_edge_modl_edge(tag_t p_edge, tag_t *m_part)  
    {  
        int  
            ii, jj,  
            n_edges,  
            n_split_edges;  
        tag_t  
            i_body,  
            *i_edges,  
            m_edge = NULL_TAG,  
            p_body,  
            *split_edges,  
            t_edge = NULL_TAG;  
        char  
            p_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_SF_edge_ask_body(p_edge, &p_body));  
        UF_CALL(UF_SF_body_ask_modl_body(p_body, &i_body));  
        if (UF_OBJ_ask_name(p_edge, p_name))  
            n_edges = ask_body_edges(i_body, &i_edges);  
        else  
            n_edges = ask_body_edges_of_name(i_body, p_name, &i_edges);  
        if (n_edges == 1)  
            m_edge = UF_ASSEM_ask_prototype_of_occ(i_edges[0]);  
        else  
        {  
            for (ii = 0; ii < n_edges; ii++)  
            {  
                t_edge = UF_SF_map_object_to_current_part(i_edges[ii]);  
                if (t_edge == p_edge)  
                {  
                    m_edge = UF_ASSEM_ask_prototype_of_occ(i_edges[ii]);  
                    break;  
                }  
                n_split_edges = ask_polygon_split_edges(t_edge, &split_edges);  
                for (jj = 0; jj < n_split_edges; jj++)  
                {  
                    if (split_edges[jj] == p_edge)  
                    {  
                        m_edge = UF_ASSEM_ask_prototype_of_occ(i_edges[ii]);  
                        break;  
                    }  
                }  
                if (n_split_edges > 0) UF_free(split_edges);  
                if  (jj < n_split_edges) break;  
            }  
        }  
        if (n_edges > 0) UF_free(i_edges);  
        *m_part = UF_ASSEM_ask_prototype_of_occ(  
            UF_ASSEM_ask_part_occurrence(i_body));  
        return m_edge;  
    }

```

#### 代码解析

> 这段代码是NX二次开发代码，主要实现了以下功能：
>
> 1. 内存分配：allocate_memory函数用于在NX中分配内存，并返回分配的内存地址。
> 2. 创建列表：make_an_array函数根据输入的列表创建一个数组，并返回数组长度。
> 3. 查询边：ask_body_edges函数用于查询NX部件的所有边，并返回一个包含所有边标识的数组。
> 4. 按名称查询：cycle_by_name_type_and_subtype函数用于按名称和类型查询NX部件中的对象。
> 5. 查询特定名称的边：ask_body_edges_of_name函数用于查询NX部件中特定名称的边，并返回一个包含所有匹配边标识的数组。
> 6. 查询分割边：ask_polygon_split_edges函数用于查询NX边对象的所有分割边，并返回一个包含所有分割边标识的数组。
> 7. 查询模型边：ask_polygon_edge_modl_edge函数用于查询NX多边形边对应的模型边，并返回模型边的标识和所属部件。
>
> 总的来说，这段代码提供了NX模型查询和内存管理等功能，为NX二次开发提供了基础支持。
>
