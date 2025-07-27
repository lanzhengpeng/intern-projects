### 【1633】mate faces and align edges 配对面和对齐边缘

#### 代码

```cpp
    /*HEAD MATE_FACES_AND_ALIGN_EDGES CCC UFUN */  
    /*  
        This sample program will mate a pair of faces and align a pair of  
        straight edges to mate the FROM component to the TO component in an  
        assembly.  Note:  both bodies must be in components under the current part,  
        not in the current part itself.  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据上下文，UF_print_syslog是V18版本中新增的函数。翻译为：

注意：UF_print_syslog是V18版本新增的函数。 */  
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
    static void check_mc_returns(UF_ASSEM_mc_structure_state_t struct_state,  
        UF_ASSEM_mc_status_t stat)  
    {  
         switch(struct_state)  
         {  
           case UF_ASSEM_structure_ok:  
             uc1601("UF_ASSEM_structure_ok", TRUE);  
             break;  
           case UF_ASSEM_no_component:  
             uc1601("UF_ASSEM_no_component", TRUE);  
             break;  
           case UF_ASSEM_invalid_entity_type:  
             uc1601("UF_ASSEM_invalid_entity", TRUE);  
             break;  
           case UF_ASSEM_invalid_occ:  
             uc1601("UF_ASSEM_invalid_occ", TRUE);  
             break;  
           case UF_ASSEM_invalid_from_part:  
             uc1601("UF_ASSEM_invalid_from_part", TRUE);  
             break;  
           case UF_ASSEM_invalid_to_part:  
             uc1601("UF_ASSEM_invalid_to_part", TRUE);  
             break;  
           case UF_ASSEM_circular_mating:  
             uc1601("UF_ASSEM_circular_mating", TRUE);  
             break;  
           case UF_ASSEM_promoted_object:  
             uc1601("UF_ASSEM_promo_obj", TRUE);  
             break;  
           case UF_ASSEM_invalid_offset:  
             uc1601("UF_ASSEM_invalid_offset", TRUE);  
             break;  
           default:  
             break;  
         }  
         switch(stat)  
         {  
           case UF_ASSEM_mc_solved:  
             uc1601("UF_ASSEM_mc_solve", TRUE);  
             break;  
           case UF_ASSEM_mc_not_solved:  
             uc1601("UF_ASSEM_mc_NOT_solved", TRUE);  
             break;  
           case UF_ASSEM_mc_no_mating_specified:  
             uc1601("UF_ASSEM_mc_no_mating_specified", TRUE);  
             break;  
           default:  
             uc1601("Nothing came back", TRUE);  
         }  
    }  
    /* qq3123197280 */  
    static int mask_for_linear_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_linear_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select linear edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_linear_edges, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* qq3123197280 */  
    static int mask_for_planar_face(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_PLANAR_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_planar_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select planar face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_planar_face, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            invalid_tag,  
            edge1,  
            edge2,  
            face1,  
            face2;  
        UF_ASSEM_mating_condition_t  
            mating_cond;  
        UF_ASSEM_mc_structure_state_t  
            mc_struct_state;  
        UF_ASSEM_mc_status_t  
            mc_stat;  
        UF_ASSEM_init_mc( &mating_cond);  
        if (((face1 = select_planar_face("mate FROM")) != NULL_TAG) &&  
            ((face2 = select_planar_face("mate TO")) != NULL_TAG) &&  
            ((edge1 = select_a_linear_edge("align FROM")) != NULL_TAG) &&  
            ((edge2 = select_a_linear_edge("align TO")) != NULL_TAG))  
        {  
        /* Get prototypes of occurrences 里海译:原型发生事件的获取 */  
            mating_cond.constraints[0].from = UF_ASSEM_ask_prototype_of_occ(face1);  
            mating_cond.constraints[0].to = UF_ASSEM_ask_prototype_of_occ(face2);  
            mating_cond.constraints[1].from = UF_ASSEM_ask_prototype_of_occ(edge1);  
            mating_cond.constraints[1].to = UF_ASSEM_ask_prototype_of_occ(edge2);  
        /* Get the tag of the part occurrences  里海译:获取零件出现的标签 */  
            mating_cond.constraints[0].from_part_occ =  
                UF_ASSEM_ask_part_occurrence(face1);  
            mating_cond.constraints[0].to_part_occ =  
                UF_ASSEM_ask_part_occurrence(face2);  
            mating_cond.constraints[1].from_part_occ =  
                UF_ASSEM_ask_part_occurrence(edge1);  
            mating_cond.constraints[1].to_part_occ =  
                UF_ASSEM_ask_part_occurrence(edge2);  
        /* Get Instance Tag of the FROM part Occurrence 里海译:获取FROM部分出现的实例标签 */  
            mating_cond.mated_object = UF_ASSEM_ask_inst_of_part_occ(  
                mating_cond.constraints[0].from_part_occ);  
        /*  Assign other structure values 里海译:其他结构赋值 */  
            mating_cond.name = NULL_TAG;  
            mating_cond.user_name = FALSE;  
            mating_cond.num_constraints = 2;  
            mating_cond.suppressed = FALSE;  
            mating_cond.constraints[0].from_status = UF_ASSEM_ok;  
            mating_cond.constraints[0].to_status = UF_ASSEM_ok;  
            mating_cond.constraints[0].mate_type = UF_ASSEM_v16_mate;  
            mating_cond.constraints[0].from_type = UF_ASSEM_planar_face;  
            mating_cond.constraints[0].to_type = UF_ASSEM_planar_face;  
            mating_cond.constraints[0].offset = NULL_TAG;  
            mating_cond.constraints[0].name = NULL_TAG;  
            mating_cond.constraints[0].user_name = FALSE;  
            mating_cond.constraints[1].from_status = UF_ASSEM_ok;  
            mating_cond.constraints[1].to_status = UF_ASSEM_ok;  
            mating_cond.constraints[1].mate_type = UF_ASSEM_v16_align;  
            mating_cond.constraints[1].from_type = UF_ASSEM_straight_edge;  
            mating_cond.constraints[1].to_type = UF_ASSEM_straight_edge;  
            mating_cond.constraints[1].offset = NULL_TAG;  
            mating_cond.constraints[1].name = NULL_TAG;  
            mating_cond.constraints[1].user_name = FALSE;  
            UF_CALL(UF_ASSEM_check_mc_structure(&mating_cond, &mc_struct_state,  
                &invalid_tag));  
          /* Apply mating condition 里海译:应用配合条件 */  
            UF_CALL(UF_ASSEM_apply_mc_data(&mating_cond, &mc_struct_state,  
                &mc_stat));  
            UF_CALL(UF_MODL_update());  
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

> 这段代码是一个NX的二次开发示例，主要实现了在装配体中从组件中选择两个平面和两个直线边，然后创建一个配合条件将它们配合在一起的功能。具体来说，代码主要包括以下几个关键点：
>
> 1. 包含了NX提供的API头文件，如uf.h、uf_assem.h等，以便调用NX的函数。
> 2. 定义了错误处理函数report_error，用于在调用NX函数出错时输出错误信息。
> 3. 定义了两个选择函数select_planar_face和select_a_linear_edge，用于选择平面和直线边。
> 4. 定义了配合条件结构体UF_ASSEM_mating_condition_t，并初始化了它，指定了两个配合约束：一个平面配合和一个直线边对齐。
> 5. 调用UF_ASSEM_check_mc_structure检查配合条件结构是否有效。
> 6. 调用UF_ASSEM_apply_mc_data应用配合条件，实现两个组件的配合。
> 7. 最后，调用ufusr函数实现整个配合流程。
>
> 综上所述，这段代码通过选择组件中的平面和直线边，创建配合条件，实现了组件之间的配合，展示了NX装配二次开发的基本流程。
>
