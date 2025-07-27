### 【1071】display bounding box of selected component 显示选定组件的边界框

#### 代码

```cpp
    /*HEAD DISPLAY_BOUNDING_BOX_OF_SELECTED_COMPONENT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_vec.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
    #include <uf_csys.h>  
    #include <uf_trns.h>  
    #include <uf_facet.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_BUFSIZE];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), _TRUNCATE, format, args);  
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
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_components, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static tag_t create_component_csys(tag_t component)  
    {  
        tag_t  
            csys,  
            matrix;  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4],  
            zero[3] = { 0,0,0 };  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            partname[MAX_FSPEC_SIZE + 1],  
            refset_name[MAX_ENTITY_NAME_SIZE + 1];  
        UF_CALL(UF_ASSEM_ask_component_data(component, partname, refset_name,  
            instance_name, origin, csys_matrix, transform));  
        UF_CALL(UF_CSYS_create_matrix(csys_matrix, &matrix));  
        UF_CALL(UF_CSYS_create_temp_csys(zero, matrix, &csys));  
        return csys;  
    }  
    static void map_abs2csys(tag_t input_csys, double vec[3])  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12],  
            mx[12];  
        int  
            irc;  
        tag_t  
            csys_mx;  
        UF_CALL(UF_CSYS_ask_csys_info(input_csys, &csys_mx, csys));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, &csys[3]));  
        uf5940(abs_mx, csys, mx, &irc);  
        UF_CALL(irc);  
        uf5941(vec, mx);  
        UF_VEC3_sub(vec, csys, vec);  
    }  
    static void map_csys2abs(tag_t input_csys, double vec[3])  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12],  
            mx[12];  
        int  
            irc;  
        tag_t  
            csys_mx;  
        UF_CALL(UF_CSYS_ask_csys_info(input_csys, &csys_mx, csys));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, &csys[3]));  
        uf5940(csys, abs_mx, mx, &irc);  
        UF_CALL(irc);  
        uf5941(vec, mx);  
        UF_VEC3_sub(vec, csys, vec);  
    }  
    static void find_extremes(tag_t object, tag_t csys, double *mins, double *maxs,  
        logical *first)  
    {  
        tag_t  
            matrix,  
            subent;  
        double  
            dirs[9],  
            point[3],  
            origin[3],  
            xp[3],  
            yp[3],  
            zp[3],  
            xm[3],  
            ym[3],  
            zm[3];  
        UF_CALL(UF_CSYS_ask_csys_info(csys, &matrix, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(matrix, dirs));  
        UF_VEC3_copy(&dirs[0], xp);  
        UF_VEC3_copy(&dirs[3], yp);  
        UF_VEC3_copy(&dirs[6], zp);  
        UF_VEC3_negate(xp, xm);  
        UF_VEC3_negate(yp, ym);  
        UF_VEC3_negate(zp, zm);  
        UF_CALL(UF_MODL_ask_extreme(object, xp, yp, zp, &subent, point));  
        map_abs2csys(csys, point);  
        if (*first || (point[0] > maxs[0])) maxs[0] = point[0];  
        UF_CALL(UF_MODL_ask_extreme(object, xm, ym, zm, &subent, point));  
        map_abs2csys(csys, point);  
        if (*first || (point[0] < mins[0])) mins[0] = point[0];  
        UF_CALL(UF_MODL_ask_extreme(object, yp, zp, xp, &subent, point));  
        map_abs2csys(csys, point);  
        if (*first || (point[1] > maxs[1])) maxs[1] = point[1];  
        UF_CALL(UF_MODL_ask_extreme(object, ym, zm, xm, &subent, point));  
        map_abs2csys(csys, point);  
        if (*first || (point[1] < mins[1])) mins[1] = point[1];  
        UF_CALL(UF_MODL_ask_extreme(object, zp, xp, yp, &subent, point));  
        map_abs2csys(csys, point);  
        if (*first || (point[2] > maxs[2])) maxs[2] = point[2];  
        UF_CALL(UF_MODL_ask_extreme(object, zm, xm, ym, &subent, point));  
        map_abs2csys(csys, point);  
        if (*first || (point[2] < mins[2])) mins[2] = point[2];  
        *first = FALSE;  
        return;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static void find_facet_extremes(tag_t model, tag_t csys, double *mins,  
        double *maxs, logical *first)  
    {  
        int  
            ii,  
            facet_id = UF_FACET_NULL_FACET_ID,  
            max_verts,  
            n;  
        double  
            point[3],  
            *verts;  
        UF_CALL(UF_FACET_ask_max_facet_verts(model, &max_verts));  
        UF_CALL(allocate_memory(max_verts * 3 * sizeof(double), (void **)&verts));  
        while (!UF_CALL(UF_FACET_cycle_facets(model, &facet_id)) &&  
            (facet_id != UF_FACET_NULL_FACET_ID))  
        {  
            UF_CALL(UF_FACET_ask_vertices_of_facet(model, facet_id, &n,  
                (double(*)[3]) verts));  
            for (ii = 0; ii < n; ii++)  
            {  
                UF_VEC3_copy(&verts[ii * 3], point);  
                map_abs2csys(csys, point);  
                if (*first || (point[0] > maxs[0])) maxs[0] = point[0];  
                if (*first || (point[0] < mins[0])) mins[0] = point[0];  
                if (*first || (point[1] > maxs[1])) maxs[1] = point[1];  
                if (*first || (point[1] < mins[1])) mins[1] = point[1];  
                if (*first || (point[2] > maxs[2])) maxs[2] = point[2];  
                if (*first || (point[2] < mins[2])) mins[2] = point[2];  
                *first = FALSE;  
            }  
        }  
        UF_free(verts);  
        return;  
    }  
    static void display_temporary_box(double llc[3], double urc[3], tag_t csys)  
    {  
        double  
            dirs[9],  
            delta_x[3],  
            delta_y[3],  
            delta_z[3],  
            end[3],  
            mag,  
            sizes[3],  
            start[3];  
        tag_t  
            csys_mx;  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
            UF_OBJ_FONT_SOLID, FALSE };  
        UF_VEC3_copy(llc, start);  
        map_abs2csys(csys, start);  
        UF_VEC3_copy(urc, end);  
        map_abs2csys(csys, end);  
        UF_VEC3_sub(end, start, sizes);  
        UF_CALL(UF_CSYS_ask_csys_info(csys, &csys_mx, dirs));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, dirs));  
        UF_VEC3_unitize(&dirs[0], 0, &mag, &dirs[0]);  
        UF_VEC3_unitize(&dirs[3], 0, &mag, &dirs[3]);  
        UF_VEC3_unitize(&dirs[6], 0, &mag, &dirs[6]);  
        UF_VEC3_scale(sizes[0], &dirs[0], delta_x);  
        UF_VEC3_scale(sizes[1], &dirs[3], delta_y);  
        UF_VEC3_scale(sizes[2], &dirs[6], delta_z);  
        UF_VEC3_add(llc, delta_x, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            llc, end, &props));  
        UF_VEC3_add(llc, delta_y, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            llc, end, &props));  
        UF_VEC3_add(llc, delta_z, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            llc, end, &props));  
        UF_VEC3_sub(urc, delta_x, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            urc, end, &props));  
        UF_VEC3_sub(urc, delta_y, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            urc, end, &props));  
        UF_VEC3_sub(urc, delta_z, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            urc, end, &props));  
        UF_VEC3_add(llc, delta_x, start);  
        UF_VEC3_add(start, delta_y, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            start, end, &props));  
        UF_VEC3_add(start, delta_z, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            start, end, &props));  
        UF_VEC3_sub(end, delta_x, start);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            start, end, &props));  
        UF_VEC3_add(start, delta_y, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            start, end, &props));  
        UF_VEC3_sub(end, delta_z, start);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            start, end, &props));  
        UF_VEC3_add(start, delta_x, end);  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            start, end, &props));  
    }  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(status->file_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static tag_t open_component(tag_t component)  
    {  
        tag_t  
            part;  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            partname[MAX_FSPEC_SIZE + 1],  
            refset_name[MAX_ENTITY_NAME_SIZE + 1];  
        UF_PART_load_status_t  
            error_status;  
        UF_CALL(UF_ASSEM_ask_component_data(component, partname, refset_name,  
            instance_name, origin, csys_matrix, transform));  
        if (UF_PART_is_loaded(partname) == 0)  
        {  
            UF_CALL(UF_PART_open_quiet(partname, &part, &error_status));  
            if (error_status.n_parts > 0) report_load_status(&error_status);  
            return part;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        logical  
            first;  
        int  
            subtype,  
            type;  
        tag_t  
            component,  
            csys,  
            object = NULL_TAG,  
            temp_open;  
        double  
            llc[3],  
            urc[3];  
        while ((component = select_a_component("Display bounding box")) != NULL_TAG)  
        {  
            csys = create_component_csys(component);  
            UF_CALL(UF_DISP_set_display(UF_DISP_SUPPRESS_DISPLAY));  
            temp_open = open_component(component);  
            first = TRUE;  
            while (!UF_CALL(UF_ASSEM_cycle_objs_in_comp(component, &object)) &&  
                (object != NULL_TAG))  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
                if ((type == UF_solid_type) && (subtype == UF_solid_face_subtype))  
                    find_extremes(object, csys, llc, urc, &first);  
                if (type == UF_faceted_model_type)  
                    find_facet_extremes(object, csys, llc, urc, &first);  
            }  
            UF_CALL(UF_DISP_set_display(UF_DISP_UNSUPPRESS_DISPLAY));  
            if (temp_open != NULL_TAG) UF_CALL(UF_PART_close(temp_open, 0, 1));  
            if (first == TRUE)  
            {  
                uc1601("Component has no faces or facets", TRUE);  
                continue;  
            }  
            map_csys2abs(csys, llc);  
            map_csys2abs(csys, urc);  
            display_temporary_box(llc, urc, csys);  
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

> 这段NX二次开发代码实现了以下功能：
>
> 1. 组件选择：通过用户交互界面，让用户选择一个组件，并获取其tag。
> 2. 创建组件坐标系：为所选组件创建一个坐标系，并保存其tag。
> 3. 模型遍历：遍历组件中的所有模型，区分实体模型和网格模型，并分别获取它们的极值点。
> 4. 极值计算：计算每个模型在组件坐标系下的极值点，并更新组件的极值范围。
> 5. 显示边界盒：根据组件的极值范围，在组件坐标系下显示一个临时的边界框。
> 6. 坐标系转换：实现了组件坐标系到绝对坐标系的转换。
> 7. 错误处理：通过封装的UF_CALL宏进行NX API调用，实现错误处理。
> 8. 内存管理：在获取网格模型顶点时，动态分配内存，并在结束后释放。
> 9. 模型打开：如果组件对应的模型未打开，会调用UF_PART_open_quiet进行打开。
> 10. 状态报告：在打开模型时，如果有错误，会报告加载状态。
>
> 总的来说，这段代码实现了在NX中为所选组件显示边界框的功能，并考虑了坐标系转换、错误处理、内存管理等因素，实现了相对完善的NX二次开发功能。
>
