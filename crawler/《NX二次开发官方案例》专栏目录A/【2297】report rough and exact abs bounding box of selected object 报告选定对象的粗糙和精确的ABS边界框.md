### 【2297】report rough and exact abs bounding box of selected object 报告选定对象的粗糙和精确的ABS边界框

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_csys.h>  
    #include <uf_disp.h>  
    #include <uf_vec.h>  
    #include <uf_trns.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
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
    static int mask_add_faces_and_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_anything(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog( "Select an object", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_add_faces_and_edges, NULL, &resp,  
            &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
        FTN(uf5940)(abs_mx, csys, mx, &irc);  
        UF_CALL(irc);  
        FTN(uf5941)(vec, mx);  
        UF_VEC3_sub(vec, csys, vec);  
    }  
    static void display_temporary_box(double llc[3], double urc[3], tag_t csys)  
    {  
        double  
            dirs[9] = { 1,0,0, 0,1,0, 0,0,1 },  
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
        UF_VEC3_copy(urc, end);  
        if (csys != NULL_TAG)  
        {  
            UF_CALL(UF_CSYS_ask_csys_info(csys, &csys_mx, dirs));  
            UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, dirs));  
            UF_VEC3_unitize(&dirs[0], 0, &mag, &dirs[0]);  
            UF_VEC3_unitize(&dirs[3], 0, &mag, &dirs[3]);  
            UF_VEC3_unitize(&dirs[6], 0, &mag, &dirs[6]);  
            map_abs2csys(csys, start);  
            map_abs2csys(csys, end);  
        }  
        UF_VEC3_sub(end, start, sizes);  
        ECHO("Box is %.3f X %.3f X %.3f\n", sizes[0], sizes[1], sizes[2]);  
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
    static void display_exact_bounding_box(tag_t object, tag_t csys)  
    {  
        int  
            ii,jj;  
        double  
            abs[9] = { 1,0,0, 0,1,0, 0,0,1 },  
            min[3],  
            dir[3][3],  
            dist[3],  
            max[3],  
            zero[3] = { 0,0,0};  
        tag_t  
            mx;  
        if (csys == NULL_TAG)  
        {  
            UF_CALL(UF_CSYS_create_matrix(abs, &mx));  
            UF_CALL(UF_CSYS_create_temp_csys(zero, mx, &csys));  
        }  
        UF_CALL(UF_MODL_ask_bounding_box_exact(object, csys, min, dir, dist ));  
        for (ii = 0; ii < 3; ii++)  
        {  
            max[ii] = min[ii];  
            for (jj = 0; jj < 3; jj++)  
                max[ii] += dir[jj][ii] * dist[jj];  
        }  
        display_temporary_box(min, max, csys);  
    }  
    static void display_rough_bounding_box(tag_t object)  
    {  
        double  
            box[6];  
        UF_CALL(UF_MODL_ask_bounding_box(object, box));  
        display_temporary_box(&box[0], &box[3], NULL_TAG);  
    }  
    static void display_conehead(double origin[3], double vec[3])  
    {  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
    }  
    static void report_object_dimensionality(tag_t object)  
    {  
        int  
            dimensionality;  
        double  
            data[6];  
        UF_CALL(UF_MODL_ask_obj_dimensionality(object, &dimensionality, data));  
        switch (dimensionality)  
        {  
            case UF_MODL_POINT:  
                ECHO("Object is a point at %.3f, %.3f, %.3f\n",  
                    data[0], data[1], data[2]);  
                break;  
            case UF_MODL_LINEAR:  
                ECHO("Object is linear\n");  
                display_conehead(data, &data[3]);  
                break;  
            case UF_MODL_PLANAR:  
                ECHO("Object is planar\n");  
                display_conehead(data, &data[3]);  
                break;  
            case UF_MODL_THREE_DIMENSIONAL:  
                ECHO("Object is 3D\n");  
                break;  
            default:  
                ECHO("Object has unknown dimensionality (%d)\n", dimensionality);  
                break;  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            object;  
        while ((object = select_anything("Report Bounding Box")) != NULL_TAG)  
        {  
            report_object_dimensionality(object);  
            ECHO("Rough ABS ");  
            display_rough_bounding_box(object);  
            ECHO("Exact ABS ");  
            display_exact_bounding_box(object, NULL_TAG);  
            ECHO("\n");  
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

> 这段NX二次开发代码主要用于报告和显示NX模型对象的相关信息。其功能包括：
>
> 1. 选择任意对象：使用UF_UI_select_with_single_dialog函数，用户可以自由选择模型中的对象，并获取其标签。
> 2. 报告对象维数：通过UF_MODL_ask_obj_dimensionality函数获取对象的维数信息，并输出其维数（点、线、面或体）。
> 3. 显示粗糙边界盒：使用UF_MODL_ask_bounding_box函数获取对象的粗糙边界盒，并使用display_temporary_box函数在模型上显示出来。
> 4. 显示精确边界盒：通过UF_MODL_ask_bounding_box_exact函数获取对象的精确边界盒，并使用display_temporary_box函数在模型上显示出来。
> 5. 显示方向箭头：使用display_conehead函数，根据对象的方向向量在模型上显示方向箭头。
> 6. 错误处理：定义了ECHO和report_error函数，用于在出现错误时打印错误信息和调用堆栈。
> 7. 初始化和清理：在ufusr函数中，首先调用UF_initialize进行初始化，然后调用do_it函数执行上述功能，最后调用UF_terminate进行清理。
> 8. 卸载处理：定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总体来说，这段代码实现了选择对象、获取对象信息、显示对象信息等核心功能，并具有完善的错误处理和初始化/清理机制。
>
