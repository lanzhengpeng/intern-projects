### 【2321】report selected cylindrical face endpoint and vector 报告选中的圆柱面端点和向量

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_brep.h>  
    #include <stdarg.h>  
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
    static int mask_for_cylindrical_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_CYLINDRICAL_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_cylindrical_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select cylindrical face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_cylindrical_faces, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
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
            blanks[UF_UI_MAX_STRING_LEN+1];  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (cols > 0)  
                if (ii == 0) ECHO("%s = ", name);  
                else ECHO("%s", blanks);  
            for (jj = 0; jj < cols; jj++)  
            {  
                ECHO("%f", array[kk++]);  
                if (jj < cols - 1) ECHO("%s", ", ");  
            }  
            if (cols > 0) ECHO("%s", "\n");  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            nstates,  
            face_type,  
            norm_dir;  
        tag_t  
            cyl_face;  
        double  
            endpoint[3],  
            point[3],  
            dir[3],  
            box[6],  
            radius,  
            rad_data,  
            vector[3];  
        UF_BREP_geometry_t  
            geometry;  
        UF_BREP_orientation_t  
            geom_sense;  
        UF_BREP_state_t  
            *states;  
        UF_BREP_topology_p_t  
            topo;  
        while ((cyl_face = select_a_cylindrical_face("Report endpoint and vector"))  
            != NULL_TAG)  
        {  
            FTN(uf5303)(&cyl_face, endpoint, vector);  
            UF_DISP_labeled_conehead(UF_DISP_ALL_ACTIVE_VIEWS, endpoint, vector, 0,  
                "uf5303");  
            WRITE3F(endpoint);  
            WRITE3F(vector);  
        // This gives a different (mid) point and sometimes the opposite direction  
            UF_CALL(UF_MODL_ask_face_data(cyl_face, &face_type, point, dir, box,  
                &radius, &rad_data, &norm_dir));  
            WRITE3F(point);  
            WRITE3F(dir);  
            UF_DISP_labeled_conehead(UF_DISP_ALL_ACTIVE_VIEWS, point, dir, 0,  
                "UF_MODL_ask_face_data");  
        // This matches the results from the old school uf5303  
            UF_CALL(UF_BREP_ask_topology(cyl_face, NULL, &topo, &nstates,  
                &states));  
            if (nstates > 0) UF_free(states);  
            UF_CALL(UF_BREP_ask_geometry(topo, NULL, NULL, &geometry,  
                &geom_sense, &nstates, &states));  
            if (nstates > 0) UF_free(states);  
            WRITE3F(geometry.geom.cylinder.pt_on_axis);  
            WRITE3F(geometry.geom.cylinder.axis);  
            UF_DISP_labeled_conehead(UF_DISP_ALL_ACTIVE_VIEWS,  
                geometry.geom.cylinder.pt_on_axis,  
                geometry.geom.cylinder.axis, 0, "UF_BREP_ask_geometry");  
            UF_CALL(UF_BREP_free_geometry_data(&geometry, NULL));  
            UF_CALL(UF_BREP_release_topology(topo, NULL));  
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

> 这段代码是一个NX的二次开发示例，主要功能是选择一个圆柱面，并获取该圆柱面的几何信息。
>
> 关键点如下：
>
> 1. 定义了一个选择圆柱面的函数select_a_cylindrical_face，它会弹出一个选择对话框，只允许选择圆柱面。
> 2. 定义了一个报告错误信息的函数report_error，用于在函数调用失败时输出错误码和错误信息。
> 3. 定义了一个显示数组数据的函数write_double_array_to_listing_window，用于将数组数据格式化输出到日志窗口。
> 4. 在函数do_it中，循环调用select_a_cylindrical_face选择圆柱面，然后调用FTN(uf5303)和UF_MODL_ask_face_data获取圆柱面的中心点和方向向量，并显示在图形区。
> 5. 同时，使用UF_BREP_ask_geometry获取圆柱面的BREP几何信息，并显示其中心点和轴线。
> 6. 最后，调用UF_BREP_free_geometry_data和UF_BREP_release_topology释放相关资源。
> 7. ufusr是NX调用的入口函数，它初始化NX环境，调用do_it函数执行主体逻辑，然后终止NX环境。
> 8. ufusr_ask_unload是卸载函数，用于卸载用户自定义的NX二次开发模块。
>
> 总体来说，这段代码通过NX提供的API实现了选择圆柱面、获取圆柱面几何信息并显示的功能，是一个典型的NX二次开发示例。
>
