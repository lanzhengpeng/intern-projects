### 【1081】display exact bound box of selected solid pre nx3 在NX3版本之前，显示选定实体的精确边界框

#### 代码

```cpp
    /*HEAD DISPLAY_EXACT_BOUND_BOX_OF_SELECTED_SOLID_PRE_NX3 CCC UFUN */  
    /*  Starting in NX 3.0.2 use UF_MODL_ask_bounding_box_exact instead of the methods  
        demonstrated in this example.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_vec.h>  
    #include <uf_modl.h>  
    #include <uf_csys.h>  
    #include <uf_trns.h>  
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
    /* qq3123197280 */  
    static int mask_for_csys(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_coordinate_system_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_csys(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a coordinate system",  
            prompt, UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_csys, NULL, &resp,  
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
    static int ask_body_faces(tag_t body, tag_p_t *body_faces)  
    {  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_ask_body_faces(body, &face_list));  
        return make_an_array(&face_list, body_faces);  
    }  
    static void map_point_to_csys(tag_t csys, double *pos)  
    {  
        int  
            irc;  
        tag_t  
            matrix;  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys_mx[12] = { 0,0,0,0,0,0,0,0,0,0,0,0},  
            mx[12],  
            origin[3];  
        UF_CALL(UF_CSYS_ask_csys_info(csys, &matrix, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(matrix, &csys_mx[3]));  
        FTN(uf5940)(abs_mx, csys_mx, mx, &irc);  
        FTN(uf5941)(pos, mx);  
    }  
    static void map_point_from_csys(tag_t csys, double *pos)  
    {  
        int  
            irc;  
        tag_t  
            matrix;  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys_mx[12] = { 0,0,0,0,0,0,0,0,0,0,0,0},  
            mx[12],  
            origin[3];  
        UF_CALL(UF_CSYS_ask_csys_info(csys, &matrix, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(matrix, &csys_mx[3]));  
        FTN(uf5940)(csys_mx, abs_mx, mx, &irc);  
        FTN(uf5941)(pos, mx);  
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
        UF_CALL( UF_MODL_ask_extreme( object, xp, yp, zp, &subent, point ));  
        map_point_to_csys(csys, point);  
        if (*first || ( point[0] > maxs[0] )) maxs[0] = point[0];  
        UF_CALL( UF_MODL_ask_extreme( object, xm, ym, zm, &subent, point ));  
        map_point_to_csys(csys, point);  
        if (*first || ( point[0] < mins[0] )) mins[0] = point[0];  
        UF_CALL( UF_MODL_ask_extreme( object, yp, zp, xp, &subent, point ));  
        map_point_to_csys(csys, point);  
        if (*first || ( point[1] > maxs[1] )) maxs[1] = point[1];  
        UF_CALL( UF_MODL_ask_extreme( object, ym, zm, xm, &subent, point ));  
        map_point_to_csys(csys, point);  
        if (*first || ( point[1] < mins[1] )) mins[1] = point[1];  
        UF_CALL( UF_MODL_ask_extreme( object, zp, xp, yp, &subent, point ));  
        map_point_to_csys(csys, point);  
        if (*first || ( point[2] > maxs[2] )) maxs[2] = point[2];  
        UF_CALL( UF_MODL_ask_extreme( object, zm, xm, ym, &subent, point ));  
        map_point_to_csys(csys, point);  
        if (*first || ( point[2] < mins[2] )) mins[2] = point[2];  
        *first = FALSE;  
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
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_VEC3_copy(llc, start);  
        map_point_to_csys(csys, start);  
        UF_VEC3_copy(urc, end);  
        map_point_to_csys(csys, end);  
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
    static void do_it(void)  
    {  
        logical  
            first;  
        int  
            ii,  
            n_faces;  
        tag_t  
            body,  
            csys,  
            *faces;  
        double  
            llc[3],  
            urc[3];  
        while ( ((body = select_a_solid("Display bounding box")) != NULL_TAG) &&  
                ((csys = select_a_csys("Csys to align box")) != NULL_TAG) &&  
                ((n_faces = ask_body_faces(body, &faces)) > 0))  
        {  
            first = TRUE;  
            for (ii = 0; ii < n_faces; ii++)  
                find_extremes(faces[ii], csys, llc, urc, &first);  
            UF_free(faces);  
            map_point_from_csys(csys, llc);  
            map_point_from_csys(csys, urc);  
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

> 这段NX二次开发代码的主要功能是显示选中的实体的精确边界框。具体包括以下几个关键部分：
>
> 1. 选择实体：代码使用UF_UI_select_with_single_dialog函数，通过对话框让用户选择一个实体。
> 2. 选择坐标系：同样，代码使用UF_UI_select_with_single_dialog函数，通过对话框让用户选择一个坐标系。
> 3. 获取实体的所有面：通过调用UF_MODL_ask_body_faces函数获取实体的所有面。
> 4. 计算边界框：对每个面，调用UF_MODL_ask_extreme函数计算其在每个坐标轴方向上的最大最小值。然后将这些值映射到用户选择的坐标系下。
> 5. 显示边界框：在用户选择的坐标系下，使用UF_DISP_display_temporary_line函数绘制边界框。
> 6. 错误处理：通过UF_call宏进行错误处理，如果函数调用失败，会输出错误信息。
> 7. 内存管理：在获取面列表后，使用UF_MODL_delete_list释放内存。在计算完边界框后，使用UF_free释放面数组内存。
> 8. 初始化和终止：在ufusr函数中，先调用UF_initialize进行初始化，然后调用do_it执行主体功能，最后调用UF_terminate终止。
> 9. 卸载函数：提供了卸载函数ufusr_ask_unload，以便NX可以在适当的时候卸载该代码。
>
> 总体来说，这段代码实现了选择实体和坐标系，计算精确边界框并显示的功能。
>
