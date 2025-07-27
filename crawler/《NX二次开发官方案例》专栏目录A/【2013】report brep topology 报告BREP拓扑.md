### 【2013】report brep topology 报告BREP拓扑

#### 代码

```cpp
    static void report_brep_topology(UF_BREP_topology_t *topology)  
    {  
        int unique_id;  
        UF_UI_open_listing_window();  
        WRITE("topology->type = ");  
        switch (topology->type)  
        {  
            case UF_BREP_SOLID_TOPO:  
                WRITE_D( UF_BREP_SOLID_TOPO );  
                break;  
            case UF_BREP_SHEET_TOPO:  
                WRITE_D( UF_BREP_SHEET_TOPO );  
                break;  
            case UF_BREP_TRIMSURF_TOPO:  
                WRITE_D( UF_BREP_TRIMSURF_TOPO );  
                break;  
            case UF_BREP_SHELL_TOPO:  
                WRITE_D( UF_BREP_SHELL_TOPO );  
                break;  
            case UF_BREP_FACE_TOPO:  
                WRITE_D( UF_BREP_FACE_TOPO );  
                break;  
            case UF_BREP_LOOP_TOPO:  
                WRITE_D( UF_BREP_LOOP_TOPO );  
                break;  
            case UF_BREP_FIN_TOPO:  
                WRITE_D( UF_BREP_FIN_TOPO );  
                break;  
            case UF_BREP_EDGE_TOPO:  
                WRITE_D( UF_BREP_EDGE_TOPO );  
                break;  
            case UF_BREP_VERTEX_TOPO:  
                WRITE_D( UF_BREP_VERTEX_TOPO );  
                break;  
            default:  
                WRITE("Unknown topology type - ");  
                WRITE_D(topology->type);  
                break;  
        }  
        WRITE_D(topology->tag);  
        WRITE_D(topology->num_children);  
        UF_BREP_ask_identifier(topology, &unique_id);  
        WRITE_D(unique_id);  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是打印BREP(边界表示)拓扑结构的信息。具体来说，它实现了以下功能：
>
> 1. 首先，调用UF_UI_open_listing_window()打开NX的输出窗口，以便将信息输出到窗口中。
> 2. 然后，通过一个switch-case结构判断拓扑结构类型，并打印出相应的类型名称和编号。
> 3. 接着，打印出拓扑结构的标签(tag)、子节点数量(num_children)等信息。
> 4. 最后，调用UF_BREP_ask_identifier()获取拓扑结构的唯一标识符(unique_id)，并打印出来。
>
> 通过这段代码，可以输出BREP拓扑结构的重要信息，如类型、标签、子节点数量等，便于开发者检查和调试BREP数据结构。
>
