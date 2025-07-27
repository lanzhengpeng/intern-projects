### 【1469】is edge sense forward on face 边相对于面的感觉是否为正向

#### 代码

```cpp
    static logical ask_edge_vs_face_sense(tag_t edge,  
        UF_BREP_topology_t *topo, UF_BREP_orientation_t *geom_sense)  
    {  
        logical  
            found = false;  
        int  
            ii,  
            nstates;  
        UF_BREP_orientation_t  
            edge_sense,  
            sp_sense;  
        UF_BREP_geometry_t  
            geometry;  
        UF_BREP_state_t  
            *states;  
    /*  get the sense from the FIN parent of the EDGE 译:从 FIN 父节点获取 EDGE 的感觉。 */  
        for (ii = 0; ii < topo->num_children; ii++)  
            if (topo->children[ii].child->tag == edge)  
            {  
                UF_CALL(UF_BREP_ask_geometry(topo, NULL, NULL, &geometry,  
                    &sp_sense, &nstates, &states));  
                if (nstates > 0) UF_free(states);  
                UF_CALL(UF_BREP_free_geometry_data(&geometry, NULL));  
                UF_CALL(UF_BREP_ask_geometry(topo->children[ii].child, NULL,  
                    NULL, &geometry, &edge_sense, &nstates, &states));  
                if (nstates > 0) UF_free(states);  
                UF_CALL(UF_BREP_free_geometry_data(&geometry, NULL));  
                if (sp_sense == edge_sense)  
                    *geom_sense = UF_BREP_ORIENTATION_FORWARD;  
                else  
                    *geom_sense = UF_BREP_ORIENTATION_REVERSE;  
                return TRUE;  
            }  
        for (ii = 0; ii < topo->num_children; ii++)  
        {  
            found = ask_edge_vs_face_sense(edge, topo->children[ii].child,  
                geom_sense);  
            if (found) break;  
        }  
        return found;  
    }  
    static logical is_edge_sense_forward_on_face(tag_t edge, tag_t face)  
    {  
        int  
            nstates;  
        UF_BREP_topology_p_t  
            topo;  
        UF_BREP_state_p_t  
            states;  
        UF_BREP_orientation_t  
            edge_sense;  
        UF_CALL(UF_BREP_ask_topology(face, NULL, &topo, &nstates, &states));  
        if (nstates > 0) UF_free(states);  
        ask_edge_vs_face_sense(edge, topo, &edge_sense);  
        UF_CALL(UF_BREP_release_topology(topo, NULL));  
        if (edge_sense == UF_BREP_ORIENTATION_FORWARD) return TRUE;  
        return FALSE;  
    }

```

#### 代码解析

> 这段NX Open C++代码用于检查边相对于面的正反方向。
>
> 主要功能包括：
>
> 1. ask_edge_vs_face_sense函数：检查指定边相对于指定面的正反方向。首先从边的FIN父节点获取边的方向，然后递归地调用自身，以确定边相对于面的方向。
> 2. is_edge_sense_forward_on_face函数：检查指定边相对于指定面的方向是否为正向。它首先获取面的拓扑结构，然后调用ask_edge_vs_face_sense函数，最后根据返回的方向值判断是否为正向。
> 3. 代码中使用了NX Open C++提供的UF_BREP类，用于查询BREP模型的拓扑和几何信息。
> 4. 在递归调用时，使用found变量来避免重复查询。
> 5. 释放了分配的内存以避免内存泄露。
> 6. 使用UF_CALL宏来简化错误处理。
>
> 这段代码的目的是为了在NX Open C++开发中，提供一种检查边相对于面方向的方法。
>
