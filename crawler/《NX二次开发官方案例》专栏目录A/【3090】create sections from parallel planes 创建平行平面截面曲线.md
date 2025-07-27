### 【3090】create sections from parallel planes 创建平行平面截面曲线

#### 代码

```cpp
    /*HEAD CREATE_SECTIONS_FROM_PARALLEL_PLANES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
    #include <uf_undo.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
    #include <uf_so.h>  
    #include <uf_wave.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：
注意：UF_print_syslog在V18中是新增的。 */  
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
    /* 里海 */  
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_bodies(char *prompt, tag_t **bodies)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select solids or sheets", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp, &cnt,  
            bodies));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*bodies)[ii], FALSE));  
        return cnt;  
    }  
    /* 里海 */  
    static int mask_for_planes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_plane_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_plane(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a plane", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_planes, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical ask_yes_or_no(char *prompt, int *deflt)  
    {  
        int  
            resp;  
        char  
            options[2][38] = { "Yes", "No" };  
        resp = 2 - *deflt;  
        resp = uc1603(prompt, resp, options, 2);  
        if ((resp > 4) && (resp < 19))  
        {  
            *deflt = 6 - resp;  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static uf_list_p_t make_a_list(int count, tag_t *item_array)  
    {  
        int  
            ii;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < count; ii++)  
            UF_CALL(UF_MODL_put_list_item(list, item_array[ii]));  
        return (list);  
    }  
    static void display_conehead(double origin[3], double vec[3])  
    {  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
    }  
    static logical show_plane_direction(tag_t plane)  
    {  
        double  
            normal[3],  
            origin[3];  
        if (!UF_CALL(UF_MODL_ask_plane(plane, origin, normal)))  
        {  
            display_conehead(origin, normal);  
            return TRUE;  
        }  
        else  
            return FALSE;  
    }  
    static logical prompt_for_three_numbers(char *prompt, char *item1, char *item2,  
        char * item3, double *number1, double *number2, double *number3)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[3][16];  
        double  
            da[3];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        strncpy(&menu[2][0], item3, 15);  
        da[0] = *number1;  
        da[1] = *number2;  
        da[2] = *number3;  
        resp = uc1609(prompt, menu, 3, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            *number3 = da[2];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static tag_t ask_part_occ_of_prom_occ(tag_t prom_object)  
    {  
        tag_t  
            owning_part,  
            part_occ = prom_object,  
            proto;  
        proto = UF_ASSEM_ask_prototype_of_occ(prom_object);  
        UF_CALL(UF_OBJ_ask_owning_part(proto, &owning_part));  
        while (!UF_CALL(UF_ASSEM_ask_parent_component(part_occ, &part_occ))  
            && (UF_ASSEM_ask_prototype_of_occ(part_occ) != owning_part));  
        return part_occ;  
    }  
    static tag_t create_dumb_body_copy(tag_t body)  
    {  
        tag_t  
            feat,  
            part_occ,  
            proto,  
            xform,  
            dumb_body;  
        uf_list_p_t  
            list;  
        if (UF_ASSEM_is_occurrence(body))  
            proto = UF_ASSEM_ask_prototype_of_occ(body);  
        else  
            proto = body;  
        if (UF_OBJ_is_object_a_promotion(proto))  
            part_occ = ask_part_occ_of_prom_occ(body);  
        else  
            part_occ = UF_ASSEM_ask_part_occurrence(body);  
        if (part_occ == NULL_TAG)  
            UF_CALL(UF_WAVE_create_linked_body(proto,NULL_TAG,body,FALSE,&feat));  
        else  
        {  
            UF_CALL(UF_SO_create_xform_assy_ctxt(body, part_occ, NULL_TAG, &xform));  
            UF_CALL(UF_WAVE_create_linked_body(proto, xform, body, FALSE, &feat));  
        }  
        UF_CALL(UF_MODL_ask_feat_body(feat, &dumb_body));  
        if (dumb_body != NULL_TAG)  
        {  
            list = make_a_list(1, &dumb_body);  
            UF_CALL(UF_MODL_delete_object_parms(list));  
            UF_CALL(UF_MODL_delete_list(&list));  
        }  
        return dumb_body;  
    }  
    static int ask_major_ug_version_number(void)  
    {  
        int  
            rel_no;  
        char  
            *major;  
        UF_CALL(UF_get_release(&major));  
        sscanf(major, "NX V%d", &rel_no);  
        if (rel_no == 0) sscanf(major, "V%d", &rel_no);  
        UF_free(major);  
        return rel_no;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            occs = 0,  
            ug_version;  
        tag_t  
            section_feat;  
        UF_CURVE_section_general_data_t  
            general_data = { NULL, 0, 0, 0, 0, 0.0 };  
        UF_CURVE_section_parallel_data_t  
            parllel_data = { NULL_TAG, 0.5, 0.0, 2.0 };  
        UF_UNDO_mark_id_t  
            mark;  
        ug_version = ask_major_ug_version_number();  
        UF_MODL_ask_distance_tolerance(&general_data.tolerance);  
        while (((general_data.num_objects =  
            select_bodies("Section Curves", &general_data.objects)) > 0) &&  
            ((parllel_data.base_plane =  
                select_a_plane("Base Plane")) != NULL_TAG) &&  
                show_plane_direction(parllel_data.base_plane) &&  
                prompt_for_three_numbers("Plane Location", "Step Distance",  
                    "Start Distance", "End Distance", &parllel_data.step_distance,  
                    &parllel_data.start_distance, &parllel_data.end_distance))  
        {  
        /*  Associate is only valid for solids within the work part 译:只对工作部分内的实体有效。 */  
            for (ii = 0; ii < general_data.num_objects; ii++)  
                if (UF_ASSEM_is_occurrence(general_data.objects[ii])) occs++;  
            if (occs == 0)  
                ask_yes_or_no("Associative Output", &general_data.associate);  
            else  
                general_data.associate = FALSE;  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible,  
                "Section from parallel planes", &mark));  
        /*  This is the work around for PR 1336323 which is fixed in NX 4*/  
            if ((occs > 0) && (ug_version < 4))  
            {  
                for (ii = 0; ii < general_data.num_objects; ii++)  
                    general_data.objects[ii] =  
                        create_dumb_body_copy(general_data.objects[ii]);  
                UF_CALL(UF_CURVE_section_from_parallel_planes(&general_data,  
                    &parllel_data, &section_feat));  
                UF_CALL(UF_OBJ_delete_array_of_objects(general_data.num_objects,  
                    general_data.objects, NULL));  
            }  
            else  
                UF_CALL(UF_CURVE_section_from_parallel_planes(&general_data,  
                    &parllel_data, &section_feat));  
            UF_free(general_data.objects);  
        }  
    }  
    /* 里海 */  
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

> 这段代码是用于NX的二次开发，主要实现了从平行平面创建剖面曲线的功能。代码的主要内容和特点如下：
>
> 1. 引入NX提供的各种头文件，包括UF库、UI库、模型库、曲线库、撤销库、装配库、对象库等。
> 2. 定义了错误报告函数report_error，用于在出错时输出错误信息和调用堆栈。
> 3. 定义了两个选择函数mask_for_bodies和mask_for_planes，用于在UI中选择实体和基准面。
> 4. 定义了显示平面方向的函数show_plane_direction。
> 5. 定义了提示输入三个数字的函数prompt_for_three_numbers。
> 6. 定义了获取原型体的函数ask_part_occ_of_prom_occ。
> 7. 定义了创建哑实体副本的函数create_dumb_body_copy。
> 8. 定义了获取NX主版本号的函数ask_major_ug_version_number。
> 9. 定义了主函数do_it，其中实现了选择实体、基准面，设置参数，创建平行平面剖切，以及撤销标记等功能。
> 10. 定义了ufusr入口函数，在NX启动时调用do_it函数。
> 11. 定义了卸载函数ufusr_ask_unload。
>
> 总体而言，这段代码实现了从平行平面创建剖面曲线的交互式NX二次开发功能，包括实体选择、基准面选择、参数设置、创建剖切等功能。代码结构清晰，注释详细，易于理解和修改。
>
