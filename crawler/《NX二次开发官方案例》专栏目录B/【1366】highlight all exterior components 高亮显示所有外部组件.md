### 【1366】highlight all exterior components 高亮显示所有外部组件

#### 代码

```cpp
    /*HEAD HIGHLIGHT_ALL_EXTERIOR_COMPONENTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
    #include <uf_so.h>  
    #include <uf_assem.h>  
    #include <uf_linked_exterior.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype) return body;  
        }  
        return NULL_TAG;  
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
    static int ask_all_bodies(tag_t part, tag_t **solids)  
    {  
        tag_t  
            body = NULL_TAG;  
        uf_list_p_t  
            body_list;  
        UF_CALL(UF_MODL_create_list(&body_list));  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(body_list, body));  
        return (make_an_array(&body_list, solids));  
    }  
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int  
            ii,  
            subtype,  
            type;  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(objects[ii], &type, &subtype));  
            if (type == UF_occ_instance_type)  
                UF_CALL(UF_DISP_set_highlight(UF_ASSEM_ask_part_occ_of_inst(  
                    NULL_TAG, objects[ii]), flag));  
            else  
                UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_bodies,  
            n_comps,  
            n_faces,  
            *comp_index;  
        tag_t  
            *bodies,  
            *comps,  
            *faces,  
            part = UF_PART_ask_display_part(),  
            *xforms;  
        double  
            chordal_tol,  
            directions[6][3] =  
                { {1,0,0}, {0,1,0}, {0,0,1}, {-1,0,0}, {0,-1,0}, {0,0,-1}};  
        uf_list_p_t  
            comp_list;  
        char  
            msg[UF_UI_MAX_STRING_LEN];  
        if ((n_bodies = ask_all_bodies(part, &bodies)) == 0) return;  
        UF_MODL_ask_distance_tolerance(&chordal_tol);  
        UF_CALL(allocate_memory(n_bodies * sizeof(tag_t),  
            (void **)&xforms));  
        UF_CALL(allocate_memory(n_bodies * sizeof(tag_t),  
            (void **)&comps));  
        for (ii = 0; ii < n_bodies; ii++)  
        {  
            if (UF_ASSEM_is_occurrence(bodies[ii]))  
            {  
                comps[ii] = UF_ASSEM_ask_part_occurrence(bodies[ii]);  
                UF_CALL(UF_SO_create_xform_assy_ctxt(comps[ii],  
                    comps[ii], NULL_TAG, &xforms[ii]));  
                bodies[ii] = UF_ASSEM_ask_prototype_of_occ(bodies[ii]);  
            }  
            else  
            {  
                xforms[ii] = NULL_TAG;  
                comps[ii] = NULL_TAG;  
            }  
        }  
        UF_CALL(UF_MODL_identify_exterior_using_hl(n_bodies,  
            bodies, xforms, 6, directions, chordal_tol,  
            UF_LINKED_HL_RES_COARSE, &n_faces,  
            &faces, &comp_index));  
        UF_CALL(UF_MODL_create_list(&comp_list));  
        for (ii = 0; ii < n_faces; ii++)  
            UF_CALL(UF_MODL_put_list_item(comp_list, comps[comp_index[ii]]));  
        UF_free(bodies);  
        UF_free(xforms);  
        UF_free(comps);  
        if (n_faces > 0)  
        {  
            UF_free(faces);  
            UF_free(comp_index);  
        }  
        if ((n_comps = make_an_array(&comp_list, &comps)) > 0)  
        {  
            set_highlight_object_array(n_comps, comps, TRUE);  
            sprintf(msg, "%d - external components highlighted", n_comps);  
            uc1601(msg, TRUE);  
            set_highlight_object_array(n_comps, comps, FALSE);  
            UF_free(comps);  
        }  
    }  
    /* qq3123197280 */  
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

> 这是用于在NX中高亮显示所有外部组件的二次开发代码。其主要功能包括：
>
> 1. 定义错误处理宏，用于在出错时打印错误信息和调用信息。
> 2. 定义了一个函数，用于遍历部件中的所有实体，并返回下一个实体的标签。
> 3. 定义了内存分配和列表处理的辅助函数。
> 4. 定义了一个函数，用于获取部件中的所有实体列表。
> 5. 定义了一个函数，用于设置实体的高亮显示。
> 6. 在主函数do_it中，首先获取当前显示的部件，并获取其所有实体。然后为每个实体创建一个变换矩阵。接着使用模型识别API识别出所有外部面，并将这些面的组件添加到列表中。最后高亮显示这些外部组件，并打印出高亮显示的组件数量。
> 7. 定义了UFusr函数作为程序的入口，初始化并调用do_it函数，然后终止程序。
> 8. 定义了卸载函数，用于在程序卸载时立即清理资源。
>
> 通过以上步骤，这段代码实现了在NX中高亮显示所有外部组件的功能。
>
