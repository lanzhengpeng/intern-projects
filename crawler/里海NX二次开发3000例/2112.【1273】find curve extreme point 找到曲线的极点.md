### 【1273】find curve extreme point 找到曲线的极点

#### 代码

```cpp
    static uf_list_p_t make_a_list(int count, tag_t *item_array)  
    {  
        int  
            ii;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < count; ii++)  
            UF_CALL(UF_MODL_put_list_item(list, item_array[ii]));  
        return (list);  
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
    static int ask_feat_faces(tag_t feat, tag_p_t *feat_faces)  
    {  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_ask_feat_faces(feat, &face_list));  
        return make_an_array(&face_list, feat_faces);  
    }  
    static void find_curve_extreme_point(tag_t curve, double dir[3],  
        double point[3])  
    {  
        int  
            n_faces,  
            n_feats;  
        tag_t  
            *feats,  
            *faces;  
        double  
            mtx[9];  
        char  
            *limits[2] = { "0.0", "1.0" };  
        UF_UNDO_mark_id_t  
            mark_id;  
        uf_list_p_t  
            curve_list,  
            feat_list;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark_id));  
        UF_CALL(UF_MTX3_initialize_x(dir, mtx));  
        curve_list = make_a_list(1, &curve);  
        UF_CALL(UF_MODL_create_extruded(curve_list, "0", limits, point,  
            &mtx[6], UF_NULLSIGN, &feat_list));  
        UF_CALL(UF_MODL_delete_list(&curve_list));  
        if (((n_feats = make_an_array(&feat_list, &feats)) > 0) &&  
            ((n_faces = ask_feat_faces(feats[0], &faces)) > 0))  
        {  
            UF_CALL(UF_MODL_ask_extreme(faces[0], mtx, &mtx[3], &mtx[6],  
                &curve, point));  
            UF_free(feats);  
            UF_free(faces);  
        }  
        UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));  
        UF_CALL(UF_UNDO_delete_mark(mark_id, NULL));  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，主要实现了以下功能：
>
> 1. make_a_list：创建一个NX对象列表，并添加指定的对象到列表中。
> 2. allocate_memory：申请指定大小的内存。
> 3. make_an_array：从NX对象列表中提取对象，并将它们存储到一个数组中，然后删除列表。
> 4. ask_feat_faces：查询指定特征的面，并返回面的数组。
> 5. find_curve_extreme_point：找到曲线的极端点，实现方法是在曲线方向上拉伸曲线生成一个特征，然后查询该特征的面，最后计算该面的极端点。
>
> 总体来说，这段代码利用NX Open C++ API实现了NX对象列表的创建、内存的申请、对象的提取、特征查询以及极端点的计算等功能。
>
