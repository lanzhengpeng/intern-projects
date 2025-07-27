### 【2011】report brep states 报告BREP状态

#### 代码

```cpp
    static void report_brep_states(int nstates, UF_BREP_state_t *states)  
    {  
        int  
            ii;  
        if (nstates > 0)  
        {  
            UF_UI_open_listing_window();  
            WRITE_D(nstates);  
            for (ii = 0; ii < nstates; ii++)  
            {  
                WRITE_D(ii);  
                WRITE("states[ii].state_code = ");  
                switch (states[ii].state_code)  
                {  
                    case UF_BREP_STATE_unknown_child:  
                        WRITE_D( UF_BREP_STATE_unknown_child);  
                        break;  
                    case UF_BREP_STATE_duplicate_child:  
                        WRITE_D( UF_BREP_STATE_duplicate_child);  
                        break;  
                    case UF_BREP_STATE_wrong_child:  
                        WRITE_D( UF_BREP_STATE_wrong_child);  
                        break;  
                    case UF_BREP_STATE_missing_child:  
                        WRITE_D( UF_BREP_STATE_missing_child);  
                        break;  
                    case UF_BREP_STATE_extra_child:  
                        WRITE_D( UF_BREP_STATE_extra_child);  
                        break;  
                    case UF_BREP_STATE_wrong_parent:  
                        WRITE_D( UF_BREP_STATE_wrong_parent);  
                        break;  
                    case UF_BREP_STATE_missing_parent:  
                        WRITE_D( UF_BREP_STATE_missing_parent);  
                        break;  
                    case UF_BREP_STATE_extra_parent:  
                        WRITE_D( UF_BREP_STATE_extra_parent);  
                        break;  
                    case UF_BREP_STATE_bad_loop:  
                        WRITE_D( UF_BREP_STATE_bad_loop);  
                        break;  
                    case UF_BREP_STATE_disjoint_shell:  
                        WRITE_D( UF_BREP_STATE_disjoint_shell);  
                        break;  
                    case UF_BREP_STATE_non_manifold:  
                        WRITE_D( UF_BREP_STATE_non_manifold);  
                        break;  
                    case UF_BREP_STATE_topology_error:  
                        WRITE_D( UF_BREP_STATE_topology_error);  
                        break;  
                    case UF_BREP_STATE_unknown_body_fault:  
                        WRITE_D( UF_BREP_STATE_unknown_body_fault);  
                        break;  
                    case UF_BREP_STATE_body_inside_out:  
                        WRITE_D( UF_BREP_STATE_body_inside_out);  
                        break;  
                    case UF_BREP_STATE_self_intersecting:  
                        WRITE_D( UF_BREP_STATE_self_intersecting);  
                        break;  
                    case UF_BREP_STATE_degenerate_geometry:  
                        WRITE_D( UF_BREP_STATE_degenerate_geometry);  
                        break;  
                    case UF_BREP_STATE_invalid_geometry:  
                        WRITE_D( UF_BREP_STATE_invalid_geometry);  
                        break;  
                    case UF_BREP_STATE_face_face_inconsistency:  
                        WRITE_D( UF_BREP_STATE_face_face_inconsistency);  
                        break;  
                    case UF_BREP_STATE_face_error:  
                        WRITE_D( UF_BREP_STATE_face_error);  
                        break;  
                    case UF_BREP_STATE_vertex_not_on_curve:  
                        WRITE_D( UF_BREP_STATE_vertex_not_on_curve);  
                        break;  
                    case UF_BREP_STATE_edge_reversed:  
                        WRITE_D( UF_BREP_STATE_edge_reversed);  
                        break;  
                    case UF_BREP_STATE_vertex_not_on_surface:  
                        WRITE_D( UF_BREP_STATE_vertex_not_on_surface);  
                        break;  
                    case UF_BREP_STATE_edge_not_on_surface:  
                        WRITE_D( UF_BREP_STATE_edge_not_on_surface);  
                        break;  
                    case UF_BREP_STATE_loop_inconsistent:  
                        WRITE_D( UF_BREP_STATE_loop_inconsistent);  
                        break;  
                    case UF_BREP_STATE_face_or_edge_non_g1:  
                        WRITE_D( UF_BREP_STATE_face_or_edge_non_g1);  
                        break;  
                    case UF_BREP_STATE_invalid_entity_ids:  
                        WRITE_D( UF_BREP_STATE_invalid_entity_ids);  
                        break;  
                    case UF_BREP_STATE_edge_tol_too_big:  
                        WRITE_D( UF_BREP_STATE_edge_tol_too_big);  
                        break;  
                    case UF_BREP_STATE_inconsistent_geom:  
                        WRITE_D( UF_BREP_STATE_inconsistent_geom);  
                        break;  
                    case UF_BREP_STATE_invalid_geom:  
                        WRITE_D( UF_BREP_STATE_invalid_geom);  
                        break;  
                    case UF_BREP_STATE_unknown_geom_fault:  
                        WRITE_D( UF_BREP_STATE_unknown_geom_fault);  
                        break;  
                    case UF_BREP_STATE_bad_child_count:  
                        WRITE_D( UF_BREP_STATE_bad_child_count);  
                        break;  
                    case UF_BREP_STATE_null_child_array:  
                        WRITE_D( UF_BREP_STATE_null_child_array);  
                        break;  
                    case UF_BREP_STATE_null_child_pointer:  
                        WRITE_D( UF_BREP_STATE_null_child_pointer);  
                        break;  
                    case UF_BREP_STATE_multi_vertex_loop:  
                        WRITE_D( UF_BREP_STATE_multi_vertex_loop);  
                        break;  
                    default:  
                        WRITE("Unknown state code - ");  
                        WRITE_D(states[ii].state_code);  
                        break;  
                }  
                WRITE_D(states[ii].flag);  
                WRITE3F(states[ii].value);  
                if (states[ii].item != NULL) report_brep_topology(states[ii].item);  
                WRITE_D(states[ii].ug_tag);  
                WRITE_D(states[ii].misc);  
            }  
            UF_free(states);  
        }  
    }

```

#### 代码解析

> 这段NX Open C++代码用于报告BREP（边界表示）实体状态。其主要功能包括：
>
> 1. 打开NX的列表窗口，用于显示状态信息。
> 2. 遍历传入的states数组，该数组包含BREP实体的状态信息。对于每个状态，输出状态码、标志、值、拓扑信息、UG标签和杂项信息。
> 3. 对于每个状态，使用switch-case语句判断状态码，并输出对应的枚举值。
> 4. 输出状态的标志、值、UG标签和杂项信息。
> 5. 如果状态包含拓扑信息，调用report_brep_topology函数输出拓扑信息。
> 6. 释放states数组占用的内存。
> 7. 通过打开列表窗口、输出状态信息的方式，这段代码帮助开发者调试BREP实体，检查其状态是否正确，发现问题所在。
>
