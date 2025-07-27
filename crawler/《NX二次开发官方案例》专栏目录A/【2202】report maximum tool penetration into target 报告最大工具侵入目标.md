### 【2202】report maximum tool penetration into target 报告最大工具侵入目标

#### 代码

```cpp
    /*HEAD REPORT_MAXIMUM_TOOL_PENETRATION_INTO_TARGET CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_mdlcmp.h>  
    #include <uf_undo.h>  
    #include <uf_part.h>  
    #include <uf_mtx.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能，用于打印系统日志。 */  
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
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* qq3123197280 */  
    static int mask_for_solid_bodies( UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_SOLID_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_solid(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( "Select a solid", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_solid_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            ii;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            subtract,  
            target,  
            tool;  
        double  
            no_trans[16],  
            gouge = 0;  
        UF_UNDO_mark_id_t  
            mark;  
        UF_MODL_compare_options_t  
            compare_options;  
        UF_MODL_compare_part_map_data_t  
            mapping_data;  
        if (((target = select_a_solid("Target")) == NULL_TAG) ||  
            ((tool = select_a_solid("Tool")) == NULL_TAG)) return;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark));  
        UF_CALL(UF_MODL_subtract_bodies_with_retained_options(target, tool, TRUE,  
            FALSE, &subtract));  
        UF_CALL(UF_MODL_ask_feat_body(subtract, &subtract));  
        UF_MODL_INIT_COMPARE_OPTIONS(&compare_options);  
        compare_options.identical_face_rule = UF_MODL_COMP_GEOM;  
        compare_options.compare_feat_and_exp = FALSE;  
        compare_options.generate_report = FALSE;  
        UF_MTX4_identity(no_trans);  
        UF_CALL(UF_MODL_model_compare_2(part, target, no_trans,  
            part, subtract, no_trans, &compare_options, &mapping_data));  
        for (ii = 0; ii < mapping_data.part2.faces.num_entities; ii++)  
        {  
            if (mapping_data.part2.faces.entity_info[ii].max_deviation > gouge)  
                gouge = mapping_data.part2.faces.entity_info[ii].max_deviation;  
        }  
        UF_CALL(UF_MODL_free_compare_data(&mapping_data));  
        UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
        UF_CALL(UF_UNDO_delete_mark(mark, NULL));  
        WRITE_F(gouge);  
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

> 这段代码是用于NX的二次开发代码，其主要功能是计算刀具在加工过程中与工件的最大侵入量。具体来说：
>
> 1. 使用UF_MODL_subtract_bodies_with_retained_options函数计算目标体与刀具体的差集，得到侵入区域。
> 2. 使用UF_MODL_model_compare_2函数比较目标体与侵入区域的差异，获取侵入量。
> 3. 遍历比较结果，找出侵入量最大的值。
> 4. 最后，将最大侵入量打印到列表窗口。
>
> 代码中定义了两个选择实体的函数select_a_solid，以及一个报告错误的宏REPORT_MAXIMUM_TOOL_PENETRATION_INTO_TARGET。主函数ufusr负责初始化NX，调用do_it函数执行侵入量计算，最后终止NX。
>
> 总的来说，这段代码实现了刀具最大侵入量的计算功能，并提供了友好的用户界面提示，以及错误处理机制。
>
