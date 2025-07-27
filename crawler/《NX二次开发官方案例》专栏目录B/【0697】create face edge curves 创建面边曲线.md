### 【0697】create face edge curves 创建面边曲线

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
    static int ask_face_edges(tag_t face, tag_t **face_edges)  
    {  
        uf_list_p_t  
            edge_list;  
        UF_CALL(UF_MODL_ask_face_edges(face, &edge_list));  
        return make_an_array(&edge_list, face_edges);  
    }  
    static void display_colored_conehead(double origin[3], double vec[3], int color)  
    {  
        UF_DISP_conehead_attrb_s  
            attrb;  
        UF_DISP_get_conehead_attrb(&attrb);  
        attrb.color = color;  
        UF_DISP_set_conehead_attrb(&attrb);  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
        UF_DISP_reset_conehead_attrb();  
    }  
    static void show_curve_direction(tag_t curve)  
    {  
        double  
            junk[3],  
            start[3],  
            tangent[3];  
        UF_CALL(UF_MODL_ask_curve_props(curve, 0.0, start, tangent, junk, junk,  
                junk, junk));  
        display_colored_conehead(start, tangent, UF_OBJ_RED);  
    }  
    static int create_face_edge_curves(tag_t face, tag_t **curves)  
    {  
        int  
            ii,  
            n_edges = 0;  
        tag_t  
            *edges;  
        n_edges = ask_face_edges(face, &edges);  
        UF_CALL(allocate_memory(n_edges * sizeof(int), curves));  
        for (ii = 0; ii < n_edges; ii++)  
        {  
            UF_CALL(UF_MODL_create_curve_from_edge(edges[ii], *curves + ii));  
            show_curve_direction(*(*curves + ii));  
        }  
        UF_free(edges);  
        return n_edges;  
    }

```

#### 代码解析

> 这段NX Open C++代码用于从NX模型中的一个面创建曲线。
>
> 主要功能包括：
>
> 1. allocate_memory：分配指定大小的内存。
> 2. make_an_array：从UF_MODL_ask_list_count获得的列表计数，分配内存并创建一个包含列表中所有元素的数组。
> 3. ask_face_edges：获取一个面的所有边，并使用make_an_array创建一个包含所有边标签的数组。
> 4. display_colored_conehead：显示一个锥形头部，可以设置颜色。
> 5. show_curve_direction：显示曲线的方向，使用display_colored_conehead函数显示。
> 6. create_face_edge_curves：从一个面的所有边创建曲线，并显示每条曲线的方向。返回创建的曲线数量。
>
> 主要流程是：
>
> 1. 获取面的所有边。
> 2. 为每条边创建曲线。
> 3. 显示每条曲线的方向。
> 4. 返回创建的曲线数量。
>
> 代码中使用了NX Open提供的UF_MODL和UF_DISP函数来操作NX模型和显示。通过这些函数，可以实现从NX面创建曲线并显示曲线方向的功能。
>
