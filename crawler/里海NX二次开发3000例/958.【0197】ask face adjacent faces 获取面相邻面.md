### 【0197】ask face adjacent faces 获取面相邻面

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
    static int ask_face_adjacent_faces(tag_t face, tag_t **adjacent_faces)  
    {  
        uf_list_p_t  
            adj_face_list,  
            ii, jj,  
            edge_list,  
            face_list;  
        UF_CALL(UF_MODL_ask_face_edges(face, &edge_list));  
        UF_CALL(UF_MODL_create_list(&adj_face_list));  
        for (ii = edge_list; ii != NULL; ii = ii->next)  
        {  
            UF_CALL(UF_MODL_ask_edge_faces(ii->eid, &face_list));  
            for (jj = face_list; jj != NULL; jj = jj->next)  
                UF_CALL(UF_MODL_put_list_item(adj_face_list, jj->eid));  
            UF_CALL(UF_MODL_delete_list(&face_list));  
        }  
        UF_CALL(UF_MODL_delete_list(&edge_list));  
        UF_CALL(UF_MODL_delete_list_item(&adj_face_list, face));  
        return make_an_array(&adj_face_list, adjacent_faces);  
    }

```

#### 代码解析

> 这段NX Open C++代码主要用于查询一个NX面相邻的面的列表。主要步骤如下：
>
> 1. 函数allocate_memory()：用于申请指定大小的内存，并将内存指针赋值给参数where。
> 2. 函数make_an_array()：创建一个面列表，然后循环遍历该列表，将每个面的标签保存到数组objects中，然后删除面列表。
> 3. 函数ask_face_adjacent_faces()：调用UF_MODL_ask_face_edges()获取面的边列表。创建一个空的面列表adj_face_list。循环遍历边列表，对每条边调用UF_MODL_ask_edge_faces()获取相邻面列表，并将这些面添加到adj_face_list中，同时删除临时面列表。删除边的列表。从adj_face_list中删除当前面本身。调用make_an_array()将adj_face_list转换为数组adjacent_faces，并返回数组大小。
> 4. 调用UF_MODL_ask_face_edges()获取面的边列表。
> 5. 创建一个空的面列表adj_face_list。
> 6. 循环遍历边列表，对每条边调用UF_MODL_ask_edge_faces()获取相邻面列表，并将这些面添加到adj_face_list中，同时删除临时面列表。
> 7. 删除边的列表。
> 8. 从adj_face_list中删除当前面本身。
> 9. 调用make_an_array()将adj_face_list转换为数组adjacent_faces，并返回数组大小。
>
> 综上，该代码实现了查询一个NX面相邻面的功能。
>
