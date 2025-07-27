### 【3124】mate component faces 在装配体中将两个选定的面进行配合

#### 代码

```cpp
    /*HEAD MATE_COMPONENT_FACES CCC UFUN */  
    /*  
        This sample program will mate a pair of selected faces  
        Note:  The "from" face must be on a solid in a component under the work  
               part.  The "to" face can be in the work part itself or in a  
               component under the work part.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中新增的功能，用于打印系统日志。 */  
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
    static int filter_mate_faces(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            ii = 0,  
            *to_from = (int *)data;  
        tag_t  
            part_occ,  
            work_occ;  
    /*  The occurrence must be under the current work part in the assembly tree 里海译:必须在装配树中的当前工作部件下发生。 */  
        work_occ = UF_ASSEM_ask_work_occurrence();  
        part_occ = UF_ASSEM_ask_part_occurrence(object);  
        while ((part_occ != NULL_TAG) && (part_occ != work_occ))  
        {  
            UF_CALL(UF_ASSEM_ask_parent_component(part_occ, &part_occ));  
            ii++;  
        }  
        if ((work_occ != NULL_TAG) && (part_occ == NULL_TAG))  
            return (UF_UI_SEL_REJECT);  
        if ((*to_from == 1) && (ii == 0))  
            return (UF_UI_SEL_REJECT);  
        return (UF_UI_SEL_ACCEPT);  
    }  
    /* qq3123197280 */  
    static int mask_for_mate_face(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_PLANAR_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_mate_faces, NULL, type)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_mate_face(char *prompt, int to_from)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select planar face to mate",  
            prompt, UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_mate_face, &to_from,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
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
    static tag_t map_mate_object_to_work_part(tag_t face, tag_t *top_instance)  
    {  
        int  
            ii,  
            n_instances;  
        tag_t  
            part_occ,  
            instance,  
            *instances,  
            work_part,  
            work_occ;  
        uf_list_p_t  
            instance_list;  
        *top_instance = NULL_TAG;  
        part_occ = UF_ASSEM_ask_part_occurrence(face);  
        if (part_occ != NULL_TAG)  
            *top_instance = UF_ASSEM_ask_inst_of_part_occ(part_occ);  
        work_occ = UF_ASSEM_ask_work_occurrence();  
        if (part_occ == work_occ) return NULL_TAG;  
    /*  Walk up the displayed part to the work occurrence keeping track of  
        instances along the way */  
        UF_CALL(UF_MODL_create_list(&instance_list));  
        while (part_occ != work_occ)  
        {  
            instance = UF_ASSEM_ask_inst_of_part_occ(part_occ);  
            if (instance != NULL_TAG)  
            {  
                UF_CALL(UF_MODL_put_list_item(instance_list, instance));  
                *top_instance = instance;  
            }  
            UF_CALL(UF_ASSEM_ask_parent_component(part_occ, &part_occ));  
        }  
        n_instances = make_an_array(&instance_list, &instances);  
    /*  Walk down the same instance tree in the root occurrence of the work part 里海译:在工作部件的根出现中沿着相同的实例树向下走。 */  
        work_part = UF_ASSEM_ask_work_part();  
        part_occ = UF_ASSEM_ask_root_part_occ(work_part);  
        for (ii = n_instances; ii; ii--)  
            part_occ = UF_ASSEM_ask_part_occ_of_inst(part_occ, instances[ii-1]);  
        UF_free(instances);  
        return part_occ;  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void check_mc_returns(UF_ASSEM_mc_structure_state_t struct_state,  
        UF_ASSEM_mc_status_t stat)  
    {  
        WRITE("struct_state = ");  
        switch(struct_state)  
        {  
            case UF_ASSEM_structure_ok:  
                WRITE_D(UF_ASSEM_structure_ok);  
                break;  
            case UF_ASSEM_no_component:  
                WRITE_D(UF_ASSEM_no_component);  
                break;  
            case UF_ASSEM_invalid_entity_type:  
                WRITE_D(UF_ASSEM_invalid_entity_type);  
                break;  
            case UF_ASSEM_invalid_occ:  
                WRITE_D(UF_ASSEM_invalid_occ);  
                break;  
            case UF_ASSEM_invalid_from_part:  
                WRITE_D(UF_ASSEM_invalid_from_part);  
                break;  
            case UF_ASSEM_invalid_to_part:  
                WRITE_D(UF_ASSEM_invalid_to_part);  
                break;  
            case UF_ASSEM_circular_mating:  
                WRITE_D(UF_ASSEM_circular_mating);  
                break;  
            case UF_ASSEM_promoted_object:  
                WRITE_D(UF_ASSEM_promoted_object);  
                break;  
            case UF_ASSEM_invalid_offset:  
                WRITE_D(UF_ASSEM_invalid_offset);  
                break;  
            default:  
                WRITE("Unknown ");  
                WRITE_D(struct_state);  
                break;  
        }  
        WRITE("stat = ");  
        switch(stat)  
        {  
            case UF_ASSEM_mc_solved:  
                WRITE_D(UF_ASSEM_mc_solved);  
                break;  
            case UF_ASSEM_mc_not_solved:  
                WRITE_D(UF_ASSEM_mc_not_solved);  
                break;  
            case UF_ASSEM_mc_no_mating_specified:  
                WRITE_D(UF_ASSEM_mc_no_mating_specified);  
                break;  
            default:  
                WRITE("Unknown ");  
                WRITE_D(stat);  
                break;  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            inst,  
            face1,  
            face2;  
        UF_ASSEM_mating_condition_t  
            mating_cond;  
        UF_ASSEM_mc_structure_state_t  
            mc_struct_state;  
        UF_ASSEM_mc_status_t  
            mc_stat;  
        if (((face1 = select_mate_face("mate FROM", 1)) != NULL_TAG) &&  
            ((face2 = select_mate_face("mate TO", 2)) != NULL_TAG))  
        {  
            UF_CALL(UF_ASSEM_init_mc(&mating_cond));  
            mating_cond.constraints[0].from = UF_ASSEM_ask_prototype_of_occ(face1);  
            if (UF_ASSEM_is_occurrence(face2))  
                mating_cond.constraints[0].to =  
                    UF_ASSEM_ask_prototype_of_occ(face2);  
            else  
                mating_cond.constraints[0].to = face2;  
            mating_cond.constraints[0].from_part_occ =  
                map_mate_object_to_work_part(face1, &mating_cond.mated_object);  
            mating_cond.constraints[0].to_part_occ =  
                map_mate_object_to_work_part(face2, &inst);  
            mating_cond.num_constraints = 1;  
            if (!UF_CALL(UF_ASSEM_apply_mc_data(&mating_cond, &mc_struct_state,  
                &mc_stat))) UF_CALL(UF_MODL_update());  
            check_mc_returns(mc_struct_state, mc_stat);  
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

> 这段代码是一个NX二次开发示例，其主要功能是选择两个平面进行配对。以下是代码的主要功能：
>
> 1. 包含了NX提供的头文件，用于调用NX的API函数。
> 2. 定义了错误报告函数report_error，用于输出错误信息。
> 3. 定义了选择过滤函数filter_mate_faces，确保选中的平面位于组件内。
> 4. 定义了选择掩码设置函数mask_for_mate_face，用于设置选择平面时的过滤条件。
> 5. 定义了选择平面函数select_mate_face，用于让用户选择一个平面。
> 6. 定义了内存分配函数allocate_memory，用于申请内存。
> 7. 定义了数组创建函数make_an_array，用于将列表转换为数组。
> 8. 定义了映射实例到工作部件函数map_mate_object_to_work_part，用于将选中的平面映射到工作部件。
> 9. 定义了整数输出函数write_integer_to_listing_window，用于输出整数到列表窗口。
> 10. 定义了检查配对结果函数check_mc_returns，用于检查配对的结构状态和求解状态。
> 11. 定义了执行配对函数do_it，用于执行配对操作。
> 12. 定义了ufusr主函数，用于初始化NX并执行配对操作。
> 13. 定义了卸载函数ufusr_ask_unload，用于立即卸载二次开发代码。
>
> 综上所述，这段代码实现了NX中组件内平面的配对功能，通过用户选择两个平面，调用NX的API进行配对，并输出配对结果。
>
