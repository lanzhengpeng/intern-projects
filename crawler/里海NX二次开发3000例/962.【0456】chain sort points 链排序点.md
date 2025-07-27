### 【0456】chain sort points 链排序点

#### 代码

```cpp
    static void chain_sort_points(tag_t start, tag_t end, tag_t *pts, int n_pts)  
    {  
        int  
            ii;  
        tag_t  
            best_point;  
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
        for (ii = 0; ii < n_pts; ii++)  
            if ((pts[ii] != start) && (pts[ii] != end))  
                UF_CALL(UF_MODL_put_list_item(point_list, pts[ii]));  
        while ((n_pts > 2) && (point_list != NULL))  
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
        for (ii = 0, temp = chain_list; ii < n_pts; temp = temp->next, ii++)  
            pts[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(&chain_list));  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是对一组曲线点进行链式排序，以便按照距离顺序连接相邻点。
>
> 主要步骤包括：
>
> 1. 创建两个链表，一个用于存储曲线点，另一个用于存储排序后的链式点集。
> 2. 将起点和终点加入链式点集。
> 3. 计算剩余点与上一个点的距离，并选择距离最近的点加入链式点集。
> 4. 重复上述步骤，直到所有点都被加入链式点集。
> 5. 最后将终点加入链式点集，并返回排序后的点集。
> 6. 通过循环遍历链式点集，将排序后的点集存储在数组中。
> 7. 清除链表，完成排序过程。
>
> 这个排序算法的核心思想是贪心选择，每次选择距离最近的点加入链式点集，最终得到按距离顺序连接的点集。这种排序方式特别适合于空间曲线的平滑连接。
>
