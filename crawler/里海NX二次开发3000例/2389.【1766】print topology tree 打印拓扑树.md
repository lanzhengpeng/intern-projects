### 【1766】print topology tree 打印拓扑树

#### 代码

```cpp
    /*HEAD PRINT_TOPOLOGY_TREE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_brep.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：注意，UF_print_syslog在V18中是新增的。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    /* qq3123197280 */  
    static int mask_for_solids_and_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE } } ;  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_solid_or_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "Select solid or face",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_solids_and_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void print_topology_tree(UF_BREP_topology_t *a_topology, int depth)  
    {  
        int  
            ii,  
            unique_id;  
        char  
            buf1[133] = { "" },  
            buf2[133];  
        for (ii = 0; ii < depth; ii++) strcat(buf1, "  ");  
        switch (a_topology->type)  
        {  
            case UF_BREP_SOLID_TOPO:    strcat(buf1, "Solid");       break;  
            case UF_BREP_SHEET_TOPO:    strcat(buf1, "Sheet");       break;  
            case UF_BREP_TRIMSURF_TOPO: strcat(buf1, "Trimsurf");    break;  
            case UF_BREP_SHELL_TOPO:    strcat(buf1, "Shell");       break;  
            case UF_BREP_FACE_TOPO:     strcat(buf1, "Face");        break;  
            case UF_BREP_LOOP_TOPO:     strcat(buf1, "Loop");        break;  
            case UF_BREP_FIN_TOPO:      strcat(buf1, "Fin");         break;  
            case UF_BREP_EDGE_TOPO:     strcat(buf1, "Edge");        break;  
            case UF_BREP_VERTEX_TOPO:   strcat(buf1, "Vertex");      break;  
            default:                    strcat(buf1, "Unknown");     break;  
        }  
        UF_BREP_ask_identifier(a_topology, &unique_id);  
        sprintf(buf2,"%s:  UGtag=%d, id=%d\n", buf1, a_topology->tag, unique_id);  
        UF_UI_write_listing_window( buf2 );  
        if (depth >= 0)  
        for (ii = 0; ii < a_topology->num_children; ii++)  
            print_topology_tree(a_topology->children[ii].child, depth+1);  
    }  
    static void report_brep_states(int nstates, UF_BREP_state_t *states)  
    {  
        int  
            ii;  
        for (ii = 0; ii < nstates; ii++)  
        {  
            printf("%d.  state_code = %d, flag = %d, value = %f,%f,%f\n", ii+1,  
                states[ii].state_code, states[ii].flag,  
                states[ii].value[0], states[ii].value[1], states[ii].value[2]);  
            switch (states[ii].state_code)  
            {  
                case UF_BREP_STATE_unknown_child:  
                    printf ("\tUF_BREP_STATE_unknown_child\n");  
                    break;  
                case UF_BREP_STATE_duplicate_child:  
                    printf("\tUF_BREP_STATE_duplicate_child\n");  
                    printf("\t\t(Repeated parent-child relationship)\n");  
                    break;  
                case UF_BREP_STATE_wrong_child:  
                    printf("\tUF_BREP_STATE_wrong_child\n");  
                    printf("\t\t(Child topology type not consistent with parent)\n");  
                    break;  
                case UF_BREP_STATE_missing_child:  
                    printf("\tUF_BREP_STATE_missing_child\n");  
                    printf("\t\t(Parent topology has too few children)\n");  
                    break;  
                case UF_BREP_STATE_extra_child:  
                    printf("\tUF_BREP_STATE_extra_child\n");  
                    printf("\t\t(Parent topology has too many children)\n");  
                    break;  
                case UF_BREP_STATE_wrong_parent:  
                    printf("\tUF_BREP_STATE_wrong_parent\n");  
                    printf("\t\t(Vertex has both edge and loop parents)\n");  
                    break;  
                case UF_BREP_STATE_missing_parent:  
                    printf("\tUF_BREP_STATE_missing_parent\n");  
                    printf("\t\t(Child topology has too few parents)\n");  
                    break;  
                case UF_BREP_STATE_extra_parent:  
                    printf("\tUF_BREP_STATE_extra_parent\n");  
                    printf("\t\t(Child topology has too many parents)\n");  
                    break;  
                case UF_BREP_STATE_bad_loop:  
                    printf("\tUF_BREP_STATE_bad_loop\n");  
                    printf("\t\t(Loop edges' start & end vertices\n");  
                    printf("\t\t and sense do not match up)\n");  
                    break;  
                case UF_BREP_STATE_disjoint_shell:  
                    printf("\tUF_BREP_STATE_disjoint_shell\n");  
                    printf("\t\t(Shell's faces not all connected)\n");  
                    break;  
                case UF_BREP_STATE_non_manifold:  
                    printf("\tUF_BREP_STATE_non_manifold\n");  
                    printf("\t\t(Non-manifold vertex)\n");  
                    break;  
                case UF_BREP_STATE_topology_error:  
                    printf("\tUF_BREP_STATE_topology_error\n");  
                    printf("\t\t(Uncategorizable topology error)\n");  
                    break;  
                case UF_BREP_STATE_unknown_body_fault:  
                    printf("\tUF_BREP_STATE_unknown_body_fault\n");  
                    printf("\t\t(Uncategorizable body failure)\n");  
                    break;  
                case UF_BREP_STATE_body_inside_out:  
                    printf("\tUF_BREP_STATE_body_inside_out\n");  
                    break;  
                case UF_BREP_STATE_self_intersecting:  
                    printf("\tUF_BREP_STATE_self_intersecting\n");  
                    break;  
                case UF_BREP_STATE_degenerate_geometry:  
                    printf("\tUF_BREP_STATE_degenerate_geometry\n");  
                    break;  
                case UF_BREP_STATE_invalid_geometry:  
                    printf("\tUF_BREP_STATE_invalid_geometry\n");  
                    break;  
                case UF_BREP_STATE_face_face_inconsistency:  
                    printf("\tUF_BREP_STATE_face_face_inconsistency\n");  
                    break;  
                case UF_BREP_STATE_face_error:  
                    printf("\tUF_BREP_STATE_face_error\n");  
                    printf("\t\t(uncategorizable face problem)\n");  
                    break;  
                case UF_BREP_STATE_vertex_not_on_curve:  
                    printf("\tUF_BREP_STATE_vertex_not_on_curve\n");  
                    printf("\t\t(specify larger tolerance)\n");  
                    break;  
                case UF_BREP_STATE_edge_reversed:  
                    printf("\tUF_BREP_STATE_edge_reversed\n");  
                    printf("\t\t(vertex order appears backward)\n");  
                    break;  
                case UF_BREP_STATE_vertex_not_on_surface:  
                    printf("\tUF_BREP_STATE_vertex_not_on_surface\n");  
                    printf("\t\t(specify larger tolerance)\n");  
                    break;  
                case UF_BREP_STATE_edge_not_on_surface:  
                    printf("\tUF_BREP_STATE_edge_not_on_surface\n");  
                    printf("\t\t(specify larger tolerance)\n");  
                    break;  
                case UF_BREP_STATE_loop_inconsistent:  
                    printf("\tUF_BREP_STATE_loop_inconsistent\n");  
                    printf("\t\t(loop vertices out of order)\n");  
                    break;  
                case UF_BREP_STATE_face_or_edge_non_g1:  
                    printf("\tUF_BREP_STATE_face_or_edge_non_g1\n");  
                    printf("\t\t(not G1 continuous)\n");  
                    break;  
                case UF_BREP_STATE_invalid_entity_ids:  
                    printf("\tUF_BREP_STATE_invalid_entity_ids\n");  
                    break;  
                case UF_BREP_STATE_edge_tol_too_big:  
                    printf("\tUF_BREP_STATE_edge_tol_too_big\n");  
                    printf("\t\t(required tolerance exceeds user request)\n");  
                    break;  
                case UF_BREP_STATE_inconsistent_geom:  
                    printf("\tUF_BREP_STATE_inconsistent_geom\n");  
                    break;  
                case UF_BREP_STATE_invalid_geom:  
                    printf("\tUF_BREP_STATE_invalid_geom\n");  
                    printf("\t\t(geometry does not pass checks)\n");  
                    break;  
                case UF_BREP_STATE_unknown_geom_fault:  
                    printf("\tUF_BREP_STATE_unknown_geom_fault\n");  
                    printf("\t\t(possibly corrupt geometry data)\n");  
                    break;  
                case UF_BREP_STATE_bad_child_count:  
                    printf("\tUF_BREP_STATE_bad_child_count\n");  
                    printf("\t\t(child count illegal for parent topology)\n");  
                    break;  
                case UF_BREP_STATE_null_child_array:  
                    printf("\tUF_BREP_STATE_null_child_array\n");  
                    printf("\t\t(children exist but child array is NULL)\n");  
                    break;  
                case UF_BREP_STATE_null_child_pointer:  
                    printf("\tUF_BREP_STATE_null_child_pointer\n");  
                    printf("\t\t(a child pointer is NULL)\n");  
                    break;  
                case UF_BREP_STATE_multi_vertex_loop:  
                    printf("\tUF_BREP_STATE_multi_vertex_loop\n");  
                    printf("\t\t(loop can have only one vertex e.g. tip of cone)\n");  
                    break;  
                default:  
                    printf("\tUnknown state code\n");  
                    break;  
            }  
        }  
        if (nstates > 0) UF_free(states);  
    }  
    static void do_it(void)  
    {  
        int  
            nstates;  
        tag_t  
            object;  
        UF_BREP_topology_p_t  
            topo;  
        UF_BREP_state_p_t  
            states;  
        UF_CALL(UF_UI_open_listing_window());  
        while ((object = select_a_solid_or_face("Print topology")) != NULL_TAG)  
        {  
            UF_CALL(UF_BREP_ask_topology(object, 0, &topo, &nstates, &states));  
            report_brep_states(nstates, states);  
            print_topology_tree(topo, 0);  
            UF_CALL(UF_BREP_release_topology(topo, NULL));  
        }  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，实现了以下功能：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于在函数执行出错时输出错误信息。
> 2. 选择函数：定义了一个选择函数select_a_solid_or_face，用于让用户选择一个实体，并返回其tag。
> 3. 打印拓扑树：定义了一个打印函数print_topology_tree，用于递归打印BREP模型的拓扑树结构。
> 4. 状态报告函数：定义了一个状态报告函数report_brep_states，用于打印BREP模型的状态信息，包括错误状态等。
> 5. 主体函数：定义了一个主体函数do_it，循环调用选择函数，获取用户选择的实体，然后查询其BREP拓扑结构并打印。
> 6. 初始化和终止：在ufusr函数中，先进行NX的初始化，然后调用主体函数，最后终止NX。
> 7. 卸载函数：定义了一个卸载函数ufusr_ask_unload，用于在卸载时立即卸载用户函数。
>
> 总体来说，这段代码的主要功能是让用户选择一个实体，并打印其BREP拓扑结构和状态信息，方便用户检查和验证BREP模型的正确性。
>
