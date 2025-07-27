### 【1134】do clearance analysis between moving and static components 进行移动组件和静态组件之间的间隙分析

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_vec.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_clear.h>  
    #include <uf_part.h>  
    #include <uf_undo.h>  
    #include <uf_cfi.h>  
    #include <uf_abort.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    /* qq3123197280 */  
    static int mask_for_components(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_component_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_component(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a component", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_components, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
    static void add_solid_occs_to_list(tag_t comp, uf_list_p_t list)  
    {  
        int  
            subtype,  
            type;  
        tag_t  
            member = NULL_TAG;  
        while (!UF_CALL(UF_ASSEM_cycle_objs_in_comp(comp, &member)) &&  
            (member != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(member, &type, &subtype));  
            if (((type == UF_solid_type) && (subtype == UF_solid_body_subtype)) ||  
                (type == UF_faceted_model_type))  
                UF_CALL(UF_MODL_put_list_item(list, member));  
            if (type == UF_component_type)  
                add_solid_occs_to_list(member, list);  
        }  
    }  
    static int ask_solids_in_component(tag_t comp, tag_t **solids)  
    {  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        add_solid_occs_to_list(comp, list);  
        return make_an_array(&list, solids);  
    }  
    static void ask_component_origin_and_matrix(tag_t component, double origin[3],  
        double csys_matrix[9])  
    {  
        double  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            partname[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_ASSEM_ask_component_data(component, partname, refset_name,  
            instance_name, origin, csys_matrix, transform));  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static void write_interference_type(int interf_type)  
    {  
        switch (interf_type)  
        {  
            case UF_CLEAR_SOFT_INTERFERENCE:  
                WRITE("UF_CLEAR_SOFT_INTERFERENCE - ");  
                break;  
            case UF_CLEAR_TOUCH_INTERFERENCE:  
                WRITE("UF_CLEAR_TOUCH_INTERFERENCE - ");  
                break;  
            case UF_CLEAR_HARD_INTERFERENCE:  
                WRITE("UF_CLEAR_HARD_INTERFERENCE - ");  
                break;  
            case UF_CLEAR_CONT_1_IN_2_INTERFERENCE:  
                WRITE("UF_CLEAR_CONT_1_IN_2_INTERFERENCE - ");  
                break;  
            case UF_CLEAR_CONT_2_IN_1_INTERFERENCE:  
                WRITE("UF_CLEAR_CONT_2_IN_1_INTERFERENCE - ");  
                break;  
            default:  
                WRITE("Unknown Interference Type - ");  
        }  
        WRITE_D(interf_type);  
    }  
    static void display_temporary_colored_point(double *coords, int color)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        attrib.color = color;  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
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
    static logical prompt_for_an_integer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        int  
            da[1];  
        strncpy(&menu[0][0], item, 15);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        logical  
            user_abort = false;  
        int  
            color,  
            ii,  
            n1,  
            n2,  
            n_steps = 10,  
            num_interf;  
        tag_t  
            comp1,  
            comp2,  
            dataset,  
            inst,  
            object1,  
            object2,  
            part = UF_PART_ask_display_part(),  
            *solids1,  
            *solids2;  
        double  
            delta[3],  
            distance,  
            mag,  
            origin[3],  
            matrix[9],  
            pp1[3],  
            unit_delta[3];  
        UF_timer_t  
            timer;  
        UF_timer_values_t  
            times;  
        UF_CLEAR_interf_data_t  
            interf_data;  
        UF_UNDO_mark_id_t  
            mark;  
        if (((comp1 = select_a_component("Select static component")) == NULL_TAG) |  
            ((comp2 = select_a_component("Select moving component")) == NULL_TAG) |  
            !prompt_for_an_integer("Loop how many times?", "Steps", &n_steps) |  
            ((n1 = ask_solids_in_component(comp1, &solids1)) == 0) |  
            ((n2 = ask_solids_in_component(comp2, &solids2)) == 0))  
            return;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, "Moving Clearance Analysis",  
            &mark));  
        if (UF_CLEAR_create_dataset(part, "GTAC_TEST", &dataset))  
            UF_CLEAR_find_dataset(part, "GTAC_TEST", &dataset);  
        UF_CALL(UF_CLEAR_set_analysis_mode(dataset, UF_CLEAR_QUICK_FACET));  
        UF_CALL(UF_CLEAR_set_obj_list(dataset,1, UF_CLEAR_FIXED_LIST, n1,solids1));  
        UF_CALL(UF_CLEAR_set_obj_list(dataset,2, UF_CLEAR_FIXED_LIST, n2,solids2));  
        UF_free(solids1);  
        UF_free(solids2);  
    /*  Step from the origin of the moving component toward the origin of  
        the static component  
    */  
        ask_component_origin_and_matrix(comp1, pp1, matrix);  
        ask_component_origin_and_matrix(comp2, origin, matrix);  
        UF_VEC3_distance(pp1, origin, &distance);  
        UF_VEC3_sub(pp1, origin, delta);  
        UF_VEC3_unitize(delta, 0, &mag, unit_delta);  
        UF_VEC3_scale(distance/n_steps, unit_delta, delta);  
        inst = UF_ASSEM_ask_inst_of_part_occ(comp2);  
        UF_CALL(UF_begin_timer(&timer));  
        UF_CALL(UF_ABORT_enable_abort());  
        for (ii = 1; ii <= n_steps; ii++)  
        {  
            UF_CALL(UF_ABORT_ask_flag_status(&user_abort));  
            if (user_abort)  
            {  
                ECHO("UF_ABORT_ask_flag_status returned true.\n");  
                UF_CALL(UF_ABORT_clear_abort());  
                break;  
            }  
            ECHO("Step %d:\n", ii);  
            UF_VEC3_add(origin, delta, origin);  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark));  
            if (UF_CALL(UF_ASSEM_reposition_instance(inst, origin, matrix)) |  
                UF_CALL(UF_CLEAR_do_clearance_analysis(dataset)) |  
                UF_CALL(UF_MODL_update()) |  
                UF_CALL(UF_UNDO_delete_to_mark(mark, NULL)))  
                break;  
            UF_CALL(UF_CLEAR_count_interferences(dataset, &num_interf));  
            WRITE_D(num_interf);  
            if (num_interf == 0) continue;  
            object1 = NULL_TAG;  
            object2 = NULL_TAG;  
            while (!UF_CALL(UF_CLEAR_ask_next_interf(dataset, &object1, &object2))  
                && (object1 != NULL_TAG) && (object2 != NULL_TAG))  
            {  
                UF_CALL(UF_CLEAR_ask_interf_data(dataset, object1, object2,  
                    &interf_data));  
                write_interference_type(interf_data.type);  
                WRITE_D(interf_data.n_bodies);  
                if (interf_data.n_bodies > 0)  
                    UF_free(interf_data.interf_bodies);  
                WRITE3F(interf_data.point1);  
                color = ii % UF_OBJ_MAX_COLOR;  
                if (color == 0) color = UF_OBJ_MAX_COLOR;  
                display_temporary_colored_point(interf_data.point1, color);  
                WRITE3F(interf_data.point2);  
                display_temporary_colored_point(interf_data.point2, color);  
                WRITE_S(interf_data.text);  
                WRITE_D(interf_data.interf_num);  
                WRITE("\n");  
            }  
        }  
        UF_CALL(UF_end_timer(timer, &times));  
        WRITE_F(times.cpu_time);  
        WRITE_F(times.real_time);  
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

> 这段代码是一个NX Open API的二次开发示例，主要实现了以下功能：
>
> 1. 组件选择：允许用户选择两个组件作为静态组件和移动组件。
> 2. 组件的实体查询：查询两个组件中的所有实体，并存储在数组中。
> 3. 创建分析数据集：创建一个名为"GTAC_TEST"的分析数据集，用于存储后续的间隙分析结果。
> 4. 间隙分析：设置间隙分析模式为快速面分析，将两个组件的实体分别设置为固定列表，并添加到数据集中。
> 5. 移动组件位置：逐步移动移动组件的位置，模拟间隙分析的过程，每次移动后进行间隙分析，并记录结果。
> 6. 结果输出：输出每次间隙分析的结果，包括干涉类型、干涉数量、干涉点等信息，并在图形窗口中高亮显示。
> 7. 性能统计：统计整个分析过程所花费的CPU时间和实际时间。
> 8. 错误处理：在关键步骤添加错误处理，保证程序的健壮性。
> 9. 用户中断：支持用户中断操作。
>
> 综上所述，这段代码实现了在NX中通过API进行间隙分析的功能，并且提供了用户交互和错误处理的机制，具有一定的参考价值。
>
