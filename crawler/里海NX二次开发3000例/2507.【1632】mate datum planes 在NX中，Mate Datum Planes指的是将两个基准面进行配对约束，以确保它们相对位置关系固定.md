### 【1632】mate datum planes 在NX中，Mate Datum Planes指的是将两个基准面进行配对约束，以确保它们相对位置关系固定

#### 代码

```cpp
    /*HEAD MATE_DATUM_PLANES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #define ECHO(X)    (say(#X, X))  
    static void say(char *variable, int value)  
    {  
        char  
            msg[133];  
        sprintf(msg, "%s = %d\n", variable, value);  
        printf(msg);  
        uc1601(msg, TRUE);  
    }  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能，用于打印系统日志消息。 */  
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
    static void do_it(void)  
    {  
        tag_t  
            from_occ = NULL_TAG,  
            part = UF_PART_ask_display_part(),  
            to_occ = NULL_TAG;  
        UF_ASSEM_mating_condition_t  
            mc_data;  
        UF_ASSEM_mc_structure_state_t  
            struct_status;  
        UF_ASSEM_mc_status_t  
            status;  
        if (!UF_CALL(UF_OBJ_cycle_by_name_and_type(part, "IN_COMP",  
                UF_datum_plane_type, TRUE, &from_occ))  
            && (from_occ != NULL_TAG)  
            && !UF_CALL(UF_OBJ_cycle_by_name_and_type(part, "IN_ASSY",  
                UF_datum_plane_type, TRUE, &to_occ))  
            && (to_occ != NULL_TAG))  
        {  
            ECHO(UF_ASSEM_is_occurrence(from_occ));  
            ECHO(UF_ASSEM_is_occurrence(to_occ));  
            mc_data.mated_object = UF_ASSEM_ask_inst_of_part_occ(  
                UF_ASSEM_ask_part_occurrence(from_occ));  
            mc_data.name = NULL;  
            mc_data.user_name = FALSE;  
            mc_data.num_constraints = 1;  
            mc_data.suppressed = FALSE;  
            mc_data.constraints[0].from_status = UF_ASSEM_ok;  
            mc_data.constraints[0].to_status = UF_ASSEM_ok;  
            mc_data.constraints[0].mate_type = UF_ASSEM_v16_mate;  
            mc_data.constraints[0].from_type = UF_ASSEM_planar_face;  
            mc_data.constraints[0].to_type = UF_ASSEM_planar_face;  
            mc_data.constraints[0].from =  
                UF_ASSEM_ask_prototype_of_occ(from_occ);  
            mc_data.constraints[0].from_part_occ =  
                UF_ASSEM_ask_part_occurrence(from_occ);  
            if (UF_ASSEM_is_occurrence(to_occ))  
            {  
                mc_data.constraints[0].to = UF_ASSEM_ask_prototype_of_occ(to_occ);  
                mc_data.constraints[0].to_part_occ =  
                    UF_ASSEM_ask_part_occurrence(to_occ);  
            }  
            else  
            {  
                mc_data.constraints[0].to = to_occ;  
                mc_data.constraints[0].to_part_occ =  
                    UF_ASSEM_ask_root_part_occ(UF_ASSEM_ask_work_part());  
            }  
            mc_data.constraints[0].offset = NULL_TAG;  
            mc_data.constraints[0].name = NULL;  
            mc_data.constraints[0].user_name = FALSE;  
            mc_data.constraints[0].has_help_point = FALSE;  
            UF_CALL(UF_ASSEM_apply_mc_data(&mc_data, &struct_status, &status));  
            check_mc_returns(struct_status, status);  
            UF_CALL(UF_MODL_update());  
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

> 这段代码是一个NX Open C++的二次开发示例，用于实现两个平面间的配对约束。
>
> 主要功能包括：
>
> 1. 初始化和终止：使用UF_initialize和UF_terminate来初始化和终止NX Open。
> 2. 查找组件中的平面：通过名称循环查找组件中的基准面，并获取它们的tag。
> 3. 设置配对约束条件：定义配对约束条件，包括配对类型、面类型、参与配对的实体等。
> 4. 应用配对约束：调用UF_ASSEM_apply_mc_data来应用配对约束，并检查返回的状态。
> 5. 更新模型：调用UF_MODL_update来更新模型显示。
> 6. 错误处理：通过宏UF_CALL进行错误处理，并打印错误信息。
> 7. 消息输出：使用uc1601函数在消息窗口输出信息。
> 8. 卸载函数：实现卸载函数ufusr_ask_unload，返回立即卸载标志。
>
> 总体而言，这段代码实现了一个简单的NX二次开发示例，用于设置两个平面间的配对约束，并提供了错误处理和消息输出功能。
>
