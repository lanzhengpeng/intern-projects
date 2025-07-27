### 【1634】mate faces 配合面

#### 代码

```cpp
    /*HEAD MATE_FACES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的函数，其功能是用于打印系统日志。这个函数在V18版本中是新增的，主要用于打印系统日志。 */  
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
    static void do_it(void)  
    {  
      tag_t  
        lower,  
        higher;  
      UF_ASSEM_constraint_t  
        mate_constraint;  
      UF_ASSEM_mating_condition_t  
        mating_cond;  
      UF_ASSEM_mc_structure_state_t  
        mc_struct_state;  
      UF_ASSEM_mc_status_t  
        mc_stat;  
              /* Get Object ID's of named planar face occurrences 里海译:获取命名平面面出现的对象ID */  
      UF_CALL( uc5028( "batman", 6, &mate_constraint.from ));  
      UF_CALL( uc5028( "robin", 5, &mate_constraint.to ));  
              /* Get the tag of the part occurences  里海译:获取部件出现的标签 */  
      mate_constraint.from_part_occ = UF_ASSEM_ask_part_occurrence(  
        mate_constraint.from );  
      mate_constraint.to_part_occ = UF_ASSEM_ask_part_occurrence(  
        mate_constraint.to );  
              /* Get the prototype of the occurence 里海译:获取事件的原型。 */  
      mate_constraint.to = UF_ASSEM_ask_prototype_of_occ(mate_constraint.to);  
      mate_constraint.from = UF_ASSEM_ask_prototype_of_occ(mate_constraint.from);  
              /* Get Top Instance Tag of the from Part Occurrence 里海译:获取零件出现的顶级实例标签 */  
      lower = mate_constraint.from_part_occ;  
      while ((higher = UF_ASSEM_ask_part_occurrence( lower )) != NULL_TAG)  
      {  
          mating_cond.mated_object = lower;  
          lower = higher;  
      }  
      mating_cond.mated_object = UF_ASSEM_ask_inst_of_part_occ(  
        mating_cond.mated_object );  
              /*  Assign other structure values 里海译:将其他结构赋值给结构体 */  
      mate_constraint.mate_type = UF_ASSEM_v16_mate;  
      mate_constraint.from_type = UF_ASSEM_planar_face;  
      mate_constraint.to_type = UF_ASSEM_planar_face;  
      mate_constraint.offset = 1.0;  
      mate_constraint.name = NULL_TAG;  
      mate_constraint.user_name = FALSE;  
      mate_constraint.from_status = UF_ASSEM_ok;  
      mate_constraint.to_status = UF_ASSEM_ok;  
      mating_cond.name = NULL_TAG;  
      mating_cond.user_name = FALSE;  
      mating_cond.constraints[0] = mate_constraint;  
      mating_cond.num_constraints = 1;  
      mating_cond.suppressed = FALSE;  
              /* Apply mating condition 里海译:Apply mating condition 的中文翻译是“应用配合条件”。 */  
      UF_CALL(UF_ASSEM_apply_mc_data( &mating_cond, &mc_struct_state, &mc_stat ));  
      check_mc_returns(mc_struct_state, mc_stat );  
      UF_CALL(UF_MODL_update());  
    }  
    /* qq3123197280 */  
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

> 这段代码是一个用于NX的二次开发示例，其主要功能是演示如何使用NX的装配模块（UF_ASSEM）来创建和更新装配约束。
>
> 主要步骤包括：
>
> 1. 获取装配中两个平面面的对象ID，并赋值给mate_constraint结构体。
> 2. 获取这两个面所在的部件出现的标签，并赋值给mate_constraint结构体。
> 3. 获取这两个面的原型，并赋值给mate_constraint结构体。
> 4. 遍历从面所在部件的实例树，找到最顶层的实例，并赋值给mating_cond结构体。
> 5. 设置mate_constraint结构体的其他字段，如配合类型、面类型、偏移量等。
> 6. 将mate_constraint结构体添加到mating_cond结构体中，并设置其他相关字段。
> 7. 调用UF_ASSEM_apply_mc_data函数，应用这个配合条件，并检查返回状态。
> 8. 调用UF_MODL_update函数，更新模型。
> 9. 在UFusr函数中，先初始化NX环境，执行上述步骤，然后终止NX环境。
> 10. 提供一个卸载函数，用于卸载这个二次开发程序。
>
> 这个示例展示了如何使用NX的装配模块来创建和更新配合约束，包括获取面、设置配合条件、应用配合条件等步骤。对于学习NX二次开发具有一定的参考价值。
>
