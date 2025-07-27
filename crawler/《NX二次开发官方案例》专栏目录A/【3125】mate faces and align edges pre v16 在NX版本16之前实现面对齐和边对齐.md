### 【3125】mate faces and align edges pre v16 在NX版本16之前实现面对齐和边对齐

#### 代码

```cpp
    /*HEAD MATE_FACES_AND_ALIGN_EDGES CCC UFUN */  
    /* This sample program will mate two faces *  
     * and align two straight edges of two     *  
     * components. First, select two faces to  *  
     * mate and then select two straight edges *  
     * to align.                               */  
    #include <stdio.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的。因此，翻译为：“注意：UF_print_syslog是在V18版本中新增的。” */  
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
             printf("UF_ASSEM_structure_ok\n");  
             break;  
           case UF_ASSEM_no_component:  
             printf("UF_ASSEM_no_component\n");  
             break;  
           case UF_ASSEM_invalid_entity_type:  
             printf("UF_ASSEM_invalid_entity\n");  
             break;  
           case UF_ASSEM_invalid_occ:  
             printf("UF_ASSEM_invalid_occ\n");  
             break;  
           case UF_ASSEM_invalid_from_part:  
             printf("UF_ASSEM_invalid_from_part\n");  
             break;  
           case UF_ASSEM_invalid_to_part:  
             printf("UF_ASSEM_invalid_to_part\n");  
             break;  
           case UF_ASSEM_circular_mating:  
             printf("UF_ASSEM_circular_mating\n");  
             break;  
           case UF_ASSEM_promoted_object:  
             printf("UF_ASSEM_promo_obj\n");  
             break;  
           case UF_ASSEM_invalid_offset:  
             printf("UF_ASSEM_invalid_offset\n");  
             break;  
           default:  
             break;  
         }  
         switch(stat)  
         {  
           case UF_ASSEM_mc_solved:  
             printf("UF_ASSEM_mc_solve\n");  
             break;  
           case UF_ASSEM_mc_not_solved:  
             printf("UF_ASSEM_mc_NOT_solved\n");  
             break;  
           case UF_ASSEM_mc_no_mating_specified:  
             printf("UF_ASSEM_mc_no_mating_specified\n");  
             break;  
           default:  
             printf("Nothing came back\n");  
         }  
    }  
    static tag_t select_an_edge(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            irc;  
        tag_t  
            edge,  
            view;  
        UF_UI_selection_options_t  
            opts;  
        UF_UI_mask_t  
            mask;  
        opts.num_mask_triples = 1;  
        opts.mask_triples = &mask;  
        opts.scope = UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY;  
        mask.object_type = UF_solid_type;  
        mask.object_subtype = 0;  
        mask.solid_type = UF_UI_SEL_FEATURE_ANY_EDGE;  
        UF_CALL(UF_UI_select_single(prompt, &opts, &irc, &edge, cp, &view));  
        if (irc == 4 || irc == 5)  
        {  
            UF_CALL(UF_DISP_set_highlight(edge, FALSE));  
            return edge;  
        }  
        else return NULL_TAG;  
    }  
    static tag_t select_a_face(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            irc;  
        tag_t  
            face,  
            view;  
        UF_UI_selection_options_t  
            opts;  
        UF_UI_mask_t  
            mask;  
        opts.num_mask_triples = 1;  
        opts.mask_triples = &mask;  
        opts.scope = UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY;  
        mask.object_type = UF_solid_type;  
        mask.object_subtype = 0;  
        mask.solid_type = UF_UI_SEL_FEATURE_ANY_FACE;  
        UF_CALL(UF_UI_select_single(prompt, &opts, &irc, &face, cp, &view));  
        if (irc == 4 || irc == 5)  
        {  
            UF_CALL(UF_DISP_set_highlight(face, FALSE));  
            return face;  
        }  
        else return NULL_TAG;  
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
        face1 = select_a_face("Select face to mate FROM");  
        face2 = select_a_face("Select face to mate TO");  
        edge1 = select_an_edge("Select edge to align FROM");  
        edge2 = select_an_edge("Select edge to align TO");  
        /* Get prototypes of occurrences 里海译:获取发生原型的示例。 */  
       mating_cond.constraints[0].from = UF_ASSEM_ask_prototype_of_occ( face1);  
       mating_cond.constraints[0].to = UF_ASSEM_ask_prototype_of_occ( face2);  
       mating_cond.constraints[1].from = UF_ASSEM_ask_prototype_of_occ( edge1);  
       mating_cond.constraints[1].to = UF_ASSEM_ask_prototype_of_occ( edge2);  
              /* Get the tag of the part occurrences  里海译:获取部件出现的标签 */  
     mating_cond.constraints[0].from_part_occ = UF_ASSEM_ask_part_occurrence( face1);  
     mating_cond.constraints[0].to_part_occ = UF_ASSEM_ask_part_occurrence( face2);  
     mating_cond.constraints[1].from_part_occ = UF_ASSEM_ask_part_occurrence( edge1);  
     mating_cond.constraints[1].to_part_occ = UF_ASSEM_ask_part_occurrence( edge2);  
              /* Get Instance Tag of the FROM part Occurrence 里海译:获取FROM部分实例的标签 */  
      mating_cond.mated_object = UF_ASSEM_ask_inst_of_part_occ(  
                                   mating_cond.constraints[0].from_part_occ);  
              /*  Assign other structure values 里海译:将其他结构值分配 */  
      mating_cond.name = NULL_TAG;  
      mating_cond.user_name = FALSE;  
      mating_cond.num_constraints = 2;  
      mating_cond.suppressed = FALSE;  
     mating_cond.constraints[0].from_status = UF_ASSEM_ok;  
     mating_cond.constraints[0].to_status = UF_ASSEM_ok;  
     mating_cond.constraints[0].mate_type = UF_ASSEM_mate;  
     mating_cond.constraints[0].from_type = UF_ASSEM_planar_face;  
     mating_cond.constraints[0].to_type = UF_ASSEM_planar_face;  
     mating_cond.constraints[0].offset = NULL_TAG;  
     mating_cond.constraints[0].name = NULL_TAG;  
     mating_cond.constraints[0].user_name = FALSE;  
     mating_cond.constraints[1].from_status = UF_ASSEM_ok;  
     mating_cond.constraints[1].to_status = UF_ASSEM_ok;  
     mating_cond.constraints[1].mate_type = UF_ASSEM_align;  
     mating_cond.constraints[1].from_type = UF_ASSEM_straight_edge;  
     mating_cond.constraints[1].to_type = UF_ASSEM_straight_edge;  
     mating_cond.constraints[1].offset = NULL_TAG;  
     mating_cond.constraints[1].name = NULL_TAG;  
     mating_cond.constraints[1].user_name = FALSE;  
      UF_CALL( UF_ASSEM_check_mc_structure( &mating_cond, &mc_struct_state, &invalid_tag));  
              /* Apply mating condition 里海译:将匹配条件应用于目标 */  
      UF_CALL(UF_ASSEM_apply_mc_data( &mating_cond, &mc_struct_state, &mc_stat ));  
      check_mc_returns(mc_struct_state, mc_stat );  
      UF_CALL(UF_MODL_update());  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这是段用于在NX中实现面面配对和边边对齐的二次开发代码。代码的主要功能和步骤如下：
>
> 1. 定义了错误报告函数report_error，用于在函数调用出错时打印错误信息。
> 2. 定义了检查配对条件返回状态的函数check_mc_returns，用于打印配对条件的结构状态和求解状态。
> 3. 定义了选择边的函数select_an_edge，通过交互选择边。
> 4. 定义了选择面的函数select_a_face，通过交互选择面。
> 5. 定义了主函数do_it，实现面面配对和边边对齐：通过交互选择源面和目标面通过交互选择源边和目标边获取面的原型和边的原型获取面的部件实例和边的部件实例设置配对条件，包括配对类型、面类型等检查配对条件的有效性应用配对条件，进行面面配对和边边对齐更新模型显示
> 6. 通过交互选择源面和目标面
> 7. 通过交互选择源边和目标边
> 8. 获取面的原型和边的原型
> 9. 获取面的部件实例和边的部件实例
> 10. 设置配对条件，包括配对类型、面类型等
> 11. 检查配对条件的有效性
> 12. 应用配对条件，进行面面配对和边边对齐
> 13. 更新模型显示
> 14. 定义了ufusr函数，作为NX二次开发程序的入口函数，用于初始化NX系统，调用主函数do_it，并终止NX系统。
> 15. 定义了卸载函数ufusr_ask_unload，用于在程序卸载时释放资源。
>
> 总的来说，这段代码通过交互方式实现了NX中的面面配对和边边对齐功能，是NX二次开发的一个典型应用示例。
>
