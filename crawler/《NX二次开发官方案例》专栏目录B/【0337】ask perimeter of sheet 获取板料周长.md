### 【0337】ask perimeter of sheet 获取板料周长

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
    static double ask_perimeter_of_sheet(tag_t sheet)  
    {  
        int  
            ii,  
            n_edges;  
        tag_t  
            *edges;  
        double  
            edge_length,  
            total_length = 0;  
        n_edges = ask_body_edges(sheet, &edges);  
        for (ii = 0; ii < n_edges; ii++)  
        {  
            if (!UF_CALL(UF_CURVE_ask_arc_length(edges[ii], 0, 1,  
                UF_MODL_UNITS_PART, &edge_length)))  
                total_length = total_length + edge_length;  
        }  
        if (n_edges > 0) UF_free(edges);  
        return total_length;  
    }

```

#### 代码解析

> 这段代码实现了以下功能：
>
> 1. allocate_memory函数：用于分配指定大小的内存，并返回指向内存的指针，以及错误响应代码。
> 2. make_an_array函数：接收一个实体列表，创建一个数组，并将列表中的实体ID填充到数组中，然后释放列表内存。
> 3. ask_body_edges函数：用于获取指定实体的所有边，并返回一个数组，其中包含边的ID。
> 4. ask_perimeter_of_sheet函数：用于计算一个片体的周长，首先获取片体的所有边，然后计算每条边的长度并累加，最后返回总长度。
>
> 综合来看，这段代码主要实现了NX实体遍历、内存管理以及几何属性查询等功能，为后续的NX二次开发提供了基础支持。
>
