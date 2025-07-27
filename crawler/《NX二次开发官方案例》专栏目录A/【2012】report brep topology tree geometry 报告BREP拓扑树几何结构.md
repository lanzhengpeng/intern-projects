### 【2012】report brep topology tree geometry 报告BREP拓扑树几何结构

#### 代码

```cpp
    static void report_brep_topology_tree_geometry(UF_BREP_topology_t *topology)  
    {  
        int  
            ii,  
            nstates;  
        UF_BREP_geometry_t  
            geometry;  
        UF_BREP_orientation_t  
            geom_sense;  
        UF_BREP_state_t  
            *states;  
        UF_UI_open_listing_window();  
        report_brep_topology(topology);  
        if (topology->type > UF_BREP_SHELL_TOPO)  
        {  
            if (!UF_CALL(UF_BREP_ask_geometry(topology, NULL, NULL, &geometry,  
                &geom_sense, &nstates, &states)))  
            {  
                report_brep_states(nstates, states);  
                report_brep_geometry(&geometry);  
                UF_CALL(UF_BREP_free_geometry_data(&geometry, NULL));  
            }  
        }  
        UF_UI_write_listing_window("\n");  
        for (ii = 0; ii < topology->num_children; ii++)  
            report_brep_topology_tree_geometry(topology->children[ii].child);  
    }

```

#### 代码解析

> 这段代码是用于报告NX BREP（边界表示）模型拓扑树中的几何信息的。
>
> 主要功能包括：
>
> 1. 拓扑树结构报告：使用report_brep_topology函数报告当前BREP拓扑节点的类型、方向、父子关系等信息。
> 2. 几何信息报告：若当前节点类型大于SHELL，则调用UF_BREP_ask_geometry获取节点的几何信息（几何体、几何方向、状态数、状态数组），然后使用report_brep_states和report_brep_geometry报告几何状态和几何体信息，并在最后释放几何体数据。
> 3. 递归遍历：遍历当前节点的所有子节点，递归调用report_brep_topology_tree_geometry以报告子节点的几何信息。
> 4. 输出格式：使用UI的listing窗口输出报告信息，便于用户查看。
>
> 总体来说，这段代码实现了对NX BREP模型拓扑树结构的遍历和几何信息的报告，为用户提供了一个检查BREP模型结构信息的有效途径。
>
