### 【1133】do body geometry checks 检查体几何

#### 代码

```cpp
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int ii;  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
    }  
    static void do_body_geometry_checks(tag_t body)  
    {  
        int  
            body_type,  
            *fault_tokens,  
            ii, jj,  
            n_tags,  
            n_boundaries,  
            *num_edges;  
        tag_t  
            *edge_tags,  
            *fault_tags;  
        char  
            msg[133];  
        UF_CALL(UF_MODL_ask_body_structures(body, &n_tags, &fault_tags));  
        if (n_tags > 0)  
        {  
            set_highlight_object_array(n_tags, fault_tags, TRUE);  
            sprintf(msg, "%d faces/edges failed data structures check", n_tags);  
            uc1601(msg, TRUE);  
            set_highlight_object_array(n_tags, fault_tags, FALSE);  
            UF_free(fault_tags);  
        }  
        else  
            uc1601("all faces/edges pass data structures check", TRUE);  
        UF_CALL(UF_MODL_ask_body_consistency(body, &n_tags, &fault_tokens,  
            &fault_tags));  
        if (n_tags > 0)  
        {  
            for (ii = 0; ii < n_tags; ii++)  
            {  
                UF_CALL(UF_DISP_set_highlight(fault_tags[ii], TRUE));  
                UF_get_fail_message(fault_tokens[ii], msg);  
                uc1601(msg, TRUE);  
                UF_CALL(UF_DISP_set_highlight(fault_tags[ii], FALSE));  
            }  
            UF_free(fault_tokens);  
            UF_free(fault_tags);  
        }  
        else  
            uc1601("all faces/edges pass consistency check", TRUE);  
        UF_CALL(UF_MODL_ask_face_face_intersect(body, &n_tags, &fault_tags));  
        if (n_tags > 0)  
        {  
            set_highlight_object_array(n_tags, fault_tags, TRUE);  
            sprintf(msg, "%d faces have a face to face intersection", n_tags);  
            uc1601(msg, TRUE);  
            set_highlight_object_array(n_tags, fault_tags, FALSE);  
            UF_free(fault_tags);  
        }  
        else  
            uc1601("no face to face intersections found", TRUE);  
        UF_CALL(UF_MODL_ask_body_type(body, &body_type));  
        if (body_type == UF_MODL_SHEET_BODY)  
        {  
            UF_CALL(UF_MODL_ask_body_boundaries(body, &n_boundaries, &num_edges,  
                &edge_tags));  
            if (n_boundaries > 0)  
            {  
                jj = 0;  
                for (ii = 0; ii < n_boundaries; ii++)  
                {  
                    set_highlight_object_array(num_edges[ii],&edge_tags[jj],TRUE);  
                    sprintf(msg, "boundary %d has %d edges", ii+1, num_edges[ii]);  
                    uc1601(msg, TRUE);  
                    set_highlight_object_array(num_edges[ii],&edge_tags[jj],FALSE);  
                    jj = jj + num_edges[ii];  
                }  
                UF_free(num_edges);  
                UF_free(edge_tags);  
            }  
            else  
                uc1601("no sheet boundaries found", TRUE);  
        }  
        else  
            uc1601("not a sheet body", TRUE);  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能包括：
>
> 1. 检查实体模型的数据结构和一致性，包括面和边的数据结构检查、一致性检查，以及面与面相交的检查。
> 2. 对于薄片体，检查其边界和边的信息。
> 3. 在检查过程中，使用高亮显示功能突出显示有问题的面和边。
> 4. 输出检查结果信息到用户界面。
> 5. 释放使用的内存。
> 6. 该代码封装了多个函数，分别用于执行不同类型的检查，并通过消息输出结果。
>
> 总体而言，这段代码通过NX提供的API实现了对模型质量的检查，并以高亮显示和消息提示的形式反馈检查结果，便于用户定位和修复模型中的问题。
>
