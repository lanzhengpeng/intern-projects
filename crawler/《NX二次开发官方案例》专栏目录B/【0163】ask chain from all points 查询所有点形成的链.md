### 【0163】ask chain from all points 查询所有点形成的链

#### 代码

```cpp
    static tag_t ask_next_point(tag_t part, tag_t object)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_point_type, &object));  
        return (object);  
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
    static int ask_chain_from_all_points(tag_t start, tag_t end, tag_t **chain)  
    {  
        int  
            n_pts;  
        tag_t  
            best_point,  
            part = UF_PART_ask_display_part(),  
            point = NULL_TAG;  
        double  
            best_dist,  
            last_point[3],  
            this_dist,  
            this_point[3];  
        uf_list_p_t  
            chain_list,  
            point_list,  
            temp;  
        UF_CALL(UF_MODL_create_list(&point_list));  
        UF_CALL(UF_MODL_create_list(&chain_list));  
        UF_CALL(UF_MODL_put_list_item(chain_list, start));  
        UF_CALL(UF_CURVE_ask_point_data(start, last_point));  
        while ((point = ask_next_point(part, point)) != NULL_TAG)  
            if ((point != start) && (point != end))  
                UF_CALL(UF_MODL_put_list_item(point_list, point));  
        UF_CALL(UF_MODL_ask_list_count(point_list, &n_pts));  
        while ((n_pts > 0) && (point_list != NULL))  
        {  
            best_dist = -1;  
            for (temp = point_list; temp != NULL; temp = temp->next)  
            {  
                UF_CALL(UF_CURVE_ask_point_data(temp->eid, this_point));  
                UF_VEC3_distance(last_point, this_point, &this_dist);  
                if ((best_dist == -1) || (this_dist < best_dist))  
                {  
                    best_point = temp->eid;  
                    best_dist = this_dist;  
                }  
            }  
            UF_CALL(UF_MODL_put_list_item(chain_list, best_point));  
            UF_CALL(UF_CURVE_ask_point_data(best_point, last_point));  
            UF_CALL(UF_MODL_delete_list_item(&point_list, best_point));  
        }  
        UF_CALL(UF_MODL_put_list_item(chain_list, end));  
        return (make_an_array(&chain_list, chain));  
    }

```

#### 代码解析

> 这段NX Open C++代码主要用于实现从一个点到另一个点的最短路径查找功能，并主要包含以下关键函数：
>
> 1. ask_next_point：遍历零件中的所有点，并返回下一个点对象。
> 2. allocate_memory：分配指定大小的内存。
> 3. make_an_array：将列表转换为数组。
> 4. ask_chain_from_all_points：从所有点到结束点的最短路径查找。
>
> 具体实现步骤如下：
>
> 1. 创建两个列表：point_list和chain_list，分别用于存放所有点和路径点。
> 2. 将起始点放入chain_list。
> 3. 遍历零件中的所有点，将它们放入point_list，但不包括起始点和结束点。
> 4. 在point_list中找到距离chain_list最后一个点最近的点，并放入chain_list。
> 5. 重复步骤4，直到point_list为空。
> 6. 将结束点放入chain_list。
> 7. 调用make_an_array，将chain_list转换为数组，返回路径点数组。
>
> 通过以上步骤，实现了从起始点到结束点的最短路径查找功能。该代码充分利用了NX Open提供的对象遍历、列表操作、内存分配等功能，实现了较复杂的几何算法。
>
