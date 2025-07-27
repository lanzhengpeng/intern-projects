### 【1699】mate datum planes with offset 与偏移量配对基准面

#### 代码

```cpp
    /*HEAD MATE_DATUM_PLANES_WITH_OFFSET CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中新增的函数。 */  
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
    static tag_t ask_tag_of_expression_name(char *exp_name)  
    {  
        tag_t  
            exp_tag;  
        char  
            exp_str[UF_MAX_EXP_LENGTH+1] = { "" },  
            *lhs,  
            *rhs;  
        UF_CALL(UF_MODL_ask_exp(exp_name, exp_str));  
        if (!UF_CALL(UF_MODL_dissect_exp_string(exp_str, &lhs, &rhs, &exp_tag)))  
        {  
            UF_free(lhs);  
            UF_free(rhs);  
            return (exp_tag);  
        }  
        else  
            return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            from_occ = NULL_TAG,  
            offset_expression_tag = NULL_TAG,  
            part = UF_PART_ask_display_part(),  
            to_occ = NULL_TAG;  
        UF_ASSEM_mating_condition_t  
            mc_data;  
        UF_ASSEM_mc_structure_state_t  
            struct_status;  
        UF_ASSEM_mc_status_t  
            status;  
        offset_expression_tag = ask_tag_of_expression_name( "offset_dist" );  
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
            mc_data.constraints[0].mate_type = UF_ASSEM_align;  
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
            mc_data.constraints[0].offset = offset_expression_tag;  
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

> 这段代码是用于NX的二次开发，主要实现了以下功能：
>
> 1. 初始化和错误报告：代码首先包含NX API的头文件，并定义了ECHO和UF_CALL两个宏，分别用于打印变量值和报告函数调用错误。
> 2. 结构状态和约束状态：定义了check_mc_returns函数，用于检查结构状态和约束状态。
> 3. 表达式查询：ask_tag_of_expression_name函数用于查询表达式的tag。
> 4. 约束应用：do_it函数实现了查询两个坐标系的tag，并构造了约束条件，然后调用UF_ASSEM_apply_mc_data函数应用约束，最后检查约束状态。
> 5. NX的初始化和终止：在ufusr函数中，初始化NX，调用do_it函数，然后终止NX。
> 6. 卸载请求：ufusr_ask_unload函数用于请求立即卸载该二次开发代码。
>
> 总体来说，这段代码实现了在NX中查询两个坐标系，应用约束并检查约束状态的功能。
>
