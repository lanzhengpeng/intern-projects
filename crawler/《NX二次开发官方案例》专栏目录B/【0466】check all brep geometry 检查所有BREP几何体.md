### 【0466】check all brep geometry 检查所有BREP几何体

#### 代码

```cpp
    static void check_all_brep_geometry(UF_BREP_topology_t *child_topology,  
        int *n_bad)  
    {  
        int  
            ii,  
            nstates,  
            unique_id;  
        char  
            name[MAX_ENTITY_NAME_SIZE+1];  
        UF_BREP_geometry_t  
            geometry;  
        UF_BREP_orientation_t  
            geom_sense;  
        UF_BREP_state_t  
            *states;  
        UF_BREP_ask_identifier(child_topology, &unique_id);  
        if (child_topology->type > UF_BREP_SHELL_TOPO)  
        {  
            if (UF_CALL(UF_BREP_ask_geometry(child_topology, NULL, NULL, &geometry,  
                &geom_sense, &nstates, &states)))  
            {  
                ++(*n_bad);  
                switch (child_topology->type)  
                {  
                    case UF_BREP_FACE_TOPO:  
                        printf ("Face");  
                        break;  
                    case UF_BREP_LOOP_TOPO:  
                        printf ("Loop");  
                        break;  
                    case UF_BREP_FIN_TOPO:  
                        printf ("Fin");  
                        break;  
                    case UF_BREP_EDGE_TOPO:  
                        printf ("Edge");  
                        break;  
                    case UF_BREP_VERTEX_TOPO:  
                        printf ("Vertex");  
                        break;  
                    default:  
                        printf ("Unknown");  
                        break;  
                }  
                if (child_topology->tag != NULL_TAG)  
                {  
                    sprintf(name, "BAD_%d", *n_bad);  
                    printf(":  UGtag=%d, id=%d, name=%s\n", child_topology->tag,  
                        unique_id, name);  
                    UF_CALL(UF_OBJ_set_name(child_topology->tag, name));  
                }  
                else  
                    printf(":  UGtag=%d, id=%d\n", child_topology->tag, unique_id);  
            }  
            else  
            {  
                if (nstates > 0) UF_free(states);  
                UF_CALL(UF_BREP_free_geometry_data(&geometry, NULL));  
            }  
        }  
        for (ii = 0; ii < child_topology->num_children; ii++)  
            check_all_brep_geometry(child_topology->children[ii].child, n_bad);  
    }

```

#### 代码解析

> 这段代码是用于检查NX BREP模型中所有几何体拓扑的函数，其主要功能如下：
>
> 1. 获取当前拓扑的唯一标识符。
> 2. 如果当前拓扑类型大于UF_BREP_SHELL_TOPO，即不是壳体拓扑，则获取其几何信息（包括几何体类型、几何体方向、状态数和状态数组）。
> 3. 如果获取几何信息失败，则认为当前拓扑存在问题，n_bad计数器加1，并打印出当前拓扑的类型、UG标签、唯一标识符等信息。如果当前拓扑有名称，则修改为"BAD_xxx"格式，以表示存在问题。
> 4. 递归调用check_all_brep_geometry函数，对当前拓扑的子拓扑进行相同的检查。
> 5. 如果获取几何信息成功，则释放状态数组，并释放几何体数据。
> 6. 通过n_bad计数器统计出存在问题的拓扑数量。
>
> 这段代码的主要作用是在NX BREP模型检查中，递归遍历所有拓扑，并统计出存在几何问题的拓扑数量，同时输出相关信息，以便定位和修复问题。
>
