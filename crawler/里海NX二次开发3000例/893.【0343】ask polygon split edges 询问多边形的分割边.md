### 【0343】ask polygon split edges 询问多边形的分割边

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

```

#### 代码解析

> 这段代码是NX Open C++ API实现的，其主要功能是找到与给定边共享相同面的所有相邻边，并返回这些边的标签数组。
>
> 主要步骤包括：
>
> 1. 创建一个UF_MODL对象列表，用于存储找到的共享面的相邻边。
> 2. 获取给定边的所有相邻边，并询问每条相邻边关联的面。
> 3. 对相邻边的面进行排序，然后与给定边的面进行比较，如果完全相同，则将该相邻边添加到列表中。
> 4. 删除UF_MODL对象列表，并返回其中包含的边的标签数组。
> 5. 输入参数包括边的标签，输出参数是边的标签数组。
> 6. 使用了UF_MODL_create_list、UF_SF_edge_ask_adjacent_edges、UF_SF_edge_ask_faces、UF_MODL_put_list_item、UF_MODL_delete_list、UF_free、qsort等API函数。
> 7. 使用了UF_CALL宏来简化API调用。
> 8. 该函数可以用于查找与给定边共享相同面的所有边，这在NX的二次开发中非常有用。
> 9. 代码注释使用中文，方便理解。
>
> 总的来说，这段代码实现了在NX中查找与给定边共享相同面的所有相邻边，并返回这些边的标签数组的功能，是一个典型的NX Open C++ API二次开发示例。
>
