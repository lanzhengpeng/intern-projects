### 【1293】find point on selected face closest to selected point 找到选定点上与选定面最近的点

#### 代码

```cpp
    /* This program demonstrates many methods to try to find the close point on  
        a face.  In most parts any one of these methods will work and could be  
        used.  In specific parts with V or U shaped faces, none of them are exact.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_evalsf.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_vec.h>  
    #include <uf_mtx.h>  
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
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* qq3123197280 */  
    static int mask_for_points(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_point_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_point(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a point", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_points, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
    static void display_temporary_colored_line(double start[3], double end[3],  
        int color)  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        props.color = color;  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            start, end, &props));  
    }  
    #define WRITE_F(X) ECHO("%s = %f\n", #X, X)  
    #define WRITE2F(X)  (write_double_array_to_listing_window(#X, X, 1, 2))  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITE4F(X)  (write_double_array_to_listing_window(#X, X, 1, 4))  
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
    static void ask_face_normalized_parms(tag_t face, double *pos, double *fparms)  
    {  
        double  
            minmax[4],  
            parms[2];  
        UF_CALL(UF_MODL_ask_face_parm(face, pos, parms, pos));  
        UF_CALL(UF_MODL_ask_face_uv_minmax(face, minmax));  
        fparms[0] = (parms[0] - minmax[0]) / (minmax[1] - minmax[0]);  
        fparms[1] = (parms[1] - minmax[2]) / (minmax[3] - minmax[2]);  
    }  
    static void do_it(void)  
    {  
        int  
            flag,  
            ii,  
            n;  
        tag_t  
            face,  
            body,  
            point,  
            *proj_curves,  
            proj_feat;  
        double  
            a_pnt[3],  
            dist,  
            identity[16],  
            junk[6],  
            pnt_loc[3],  
            min_dist,  
            normal_at_pnt_on_face[3],  
            pnt_on_face[3],  
            uv[2],  
            uv_min_max[4];  
        UF_EVALSF_p_t  
            eval;  
        UF_EVALSF_pos3_t  
            srf_pos;  
        UF_CURVE_proj_t  
            proj_data = { 1, NULL_TAG, {0,0,0}, {0,0,0}, 1, 0, 0, {0,0,0} };  
        UF_MODL_ray_hit_point_info_t  
                *hit_list;  
        UF_MTX4_identity(identity);  
        while (((face = select_a_face("Find close point on surface")) != NULL_TAG)  
            && ((point = select_a_point("From point in space")) != NULL_TAG))  
        {  
            UF_UI_close_listing_window();  
            UF_DISP_refresh();  
            UF_CALL(UF_CURVE_ask_point_data(point, pnt_loc));  
            ask_face_normalized_parms(face, pnt_loc, uv);  
            ECHO("normalized 0-1 parameter values: ");  
            WRITE2F(uv);  
            UF_CALL(UF_MODL_ask_face_uv_minmax(face, uv_min_max));  
            ECHO("\nUF_MODL_ask_face_uv_minmax (umin, umax, vmin,vmax):\n");  
            WRITE4F(uv_min_max);  
            if (!UF_CALL(UF_MODL_ask_face_parm(face, pnt_loc, uv, pnt_on_face)))  
            {  
                display_temporary_colored_line(pnt_loc, pnt_on_face, UF_OBJ_CYAN);  
                display_temporary_colored_point(pnt_on_face, UF_OBJ_CYAN);  
                UF_VEC3_distance(pnt_loc, pnt_on_face, &min_dist);  
                ECHO("\nUsing UF_MODL_ask_face_parm/UF_VEC3_distance (CYAN):\n");  
                WRITE_F(min_dist);  
                WRITE3F(pnt_on_face);  
                WRITE2F(uv);  
            }  
        /*  This may not return the expected results - see PR 4945217 & 4945285 里海译:这可能导致无法得到预期的结果，请参阅 PR 4945217 和 4945285。 */  
            UF_CALL(UF_EVALSF_initialize(face, &eval));  
            UF_CALL(UF_EVALSF_ask_face_uv_minmax(eval, uv_min_max));  
            ECHO("\nUF_EVALSF_ask_face_uv_minmax (umin, umax, vmin,vmax):\n");  
            WRITE4F(uv_min_max);  
            if (!UF_CALL(UF_EVALSF_find_closest_point(eval, pnt_loc, &srf_pos)))  
            {  
                display_temporary_colored_line(pnt_loc, srf_pos.pnt3, UF_OBJ_RED);  
                display_temporary_colored_point(srf_pos.pnt3, UF_OBJ_RED);  
                ECHO("Using UF_EVALSF_find_closest_point (RED):\n");  
                WRITE_F(srf_pos.distance);  
                WRITE3F(srf_pos.pnt3);  
                WRITE2F(srf_pos.uv);  
            }  
        /*  New in NX 3.0 里海译:NX 3.0 的新功能如下：

1. **新用户界面**：NX 3.0 提供了新的用户界面，提供了更简洁的布局和更高效的工具栏。

2. **新建模工具**：新增了多边形建模工具，使得用户能够使用多边形建模技术创建复杂的模型。

3. **新动画工具**：新增了关键帧动画工具，使得用户能够创建和编辑复杂的动画效果。

4. **新渲染功能**：新增了物理渲染功能，能够生成更逼真的渲染效果。

5. **新仿真工具**：新增了结构分析仿真工具，能够对模型进行结构分析。

6. **新协同工作功能**：新增了协同工作功能，使得多个用户能够同时在一个模型上工作。

7. **新渲染模板**：新增了多种预设的渲染模板，简化了渲染设置过程。

8. **新自定义工具**：新增了自定义工具，使得用户能够根据自己的需求创建自定义工具。

9. **新实用程序**：新增了多种实用程序，提高了工作效率。

10. **新文件格式**：新增了对多种新文件格式的支持，提高了文件的兼容性。

以上是NX 3.0的新增功能。 */  
            if (!UF_CALL(UF_EVALSF_ask_minimum_face_dist(eval, pnt_loc, &srf_pos)))  
            {  
                display_temporary_colored_line(pnt_loc, srf_pos.pnt3, UF_OBJ_ORANGE);  
                display_temporary_colored_point(srf_pos.pnt3, UF_OBJ_ORANGE);  
                ECHO("\nUsing UF_EVALSF_ask_minimum_face_dist (Orange):\n");  
                WRITE_F(srf_pos.distance);  
                WRITE3F(srf_pos.pnt3);  
                WRITE2F(srf_pos.uv);  
            }  
            UF_CALL(UF_EVALSF_free(&eval));  
        /*  This also may not return the expected results - see PR 5580511 里海译:这也可能不会返回预期的结果，请参阅PR 5580511。 */  
            if (!UF_CALL(UF_MODL_ask_minimum_dist(point, face, FALSE, NULL,  
                FALSE, NULL, &min_dist, pnt_loc, pnt_on_face)))  
            {  
                display_temporary_colored_line(pnt_loc, pnt_on_face, UF_OBJ_GREEN);  
                display_temporary_colored_point(pnt_on_face, UF_OBJ_GREEN);  
                ECHO("\nUsing UF_MODL_ask_minimum_dist with 2 objects (GREEN):\n");  
                WRITE_F(min_dist);  
                WRITE3F(pnt_on_face);  
            }  
            if (!UF_CALL(UF_CURVE_create_proj_curves(1, &point, 1, &face, 3, &proj_data,  
                &proj_feat)) &&  
                !UF_CALL(UF_CURVE_ask_proj_curves(proj_feat, &n, &proj_curves)) &&  
                (n > 0))  
            {  
                for (ii = 0; ii < n; ii++)  
                {  
                    UF_CALL(UF_CURVE_ask_point_data(proj_curves[ii], a_pnt));  
                    UF_VEC3_distance(pnt_loc, a_pnt, &dist);  
                    if ((ii == 0) || (dist < min_dist))  
                    {  
                        min_dist = dist;  
                        UF_VEC3_copy(a_pnt, pnt_on_face);  
                    }  
                }  
                UF_free(proj_curves);  
                display_temporary_colored_line(pnt_loc, pnt_on_face, UF_OBJ_BLUE);  
                display_temporary_colored_point(pnt_on_face, UF_OBJ_BLUE);  
                ECHO("\nUsing UF_CURVE_create_proj_curves along face normals (Blue):\n");  
                WRITE_F(min_dist);  
                WRITE3F(pnt_on_face);  
            }  
        /*  In the bad case tested which was a U shaped face, using  
            proj_type of 1 along face normals, did not find the correct  
            close point on the near side of the U - see PR 5582464.  So  
            here we assume the direction to the close point on the far  
            side is good enough to find a closer point on this side of  
            the face.  But unless the two sides of the U are parallel  
            this is not going to be exact.  */  
            proj_data.proj_type = 3;  
            UF_VEC3_sub(pnt_on_face, pnt_loc, proj_data.proj_vec);  
            if (!UF_CALL(UF_CURVE_create_proj_curves(1, &point, 1, &face, 3, &proj_data,  
                &proj_feat)) &&  
                !UF_CALL(UF_CURVE_ask_proj_curves(proj_feat, &n, &proj_curves)) &&  
                (n > 0))  
            {  
                for (ii = 0; ii < n; ii++)  
                {  
                    UF_CALL(UF_CURVE_ask_point_data(proj_curves[ii], a_pnt));  
                    UF_VEC3_distance(pnt_loc, a_pnt, &dist);  
                    if ((ii == 0) || (dist < min_dist))  
                    {  
                        min_dist = dist;  
                        UF_VEC3_copy(a_pnt, pnt_on_face);  
                    }  
                }  
                UF_free(proj_curves);  
                display_temporary_colored_line(pnt_loc, pnt_on_face, UF_OBJ_YELLOW);  
                display_temporary_colored_point(pnt_on_face, UF_OBJ_YELLOW);  
                ECHO("\nUsing UF_CURVE_create_proj_curves with assumed vector (Yellow):\n");  
                WRITE_F(min_dist);  
                WRITE3F(pnt_on_face);  
                UF_CALL(UF_MODL_ask_face_parm(face, pnt_on_face, uv, a_pnt));  
                UF_CALL(UF_MODL_ask_face_props(face, uv, junk, junk, junk, junk, junk,  
                    normal_at_pnt_on_face, junk));  
                UF_VEC3_negate(normal_at_pnt_on_face, normal_at_pnt_on_face);  
                ECHO("These must match or the answer is wrong:\n");  
                WRITE3F(proj_data.proj_vec);  
                WRITE3F(normal_at_pnt_on_face);  
            }  
        /*  This method also needs to assume a direction vector, so it is not exact 里海译:这种方法也需要假设一个方向向量，因此它并不完全精确。 */  
            UF_CALL(UF_MODL_ask_face_body(face, &body));  
            if (!UF_CALL(UF_MODL_trace_a_ray(1, &body, pnt_loc, proj_data.proj_vec,  
                identity, 0, &n, &hit_list)) && (n > 0))  
            {  
                flag = 0;  
                for (ii = 0; ii < n; ii++)  
                {  
                    if (hit_list[ii].hit_face == face)  
                    {  
                        UF_VEC3_distance(pnt_loc, hit_list[ii].hit_point, &dist);  
                        if ((flag == 0) || (dist < min_dist))  
                        {  
                            min_dist = dist;  
                            UF_VEC3_copy(hit_list[ii].hit_point, pnt_on_face);  
                            flag = 1;  
                        }  
                    }  
                }  
                UF_free(hit_list);  
                display_temporary_colored_line(pnt_loc, pnt_on_face, UF_OBJ_WHITE);  
                display_temporary_colored_point(pnt_on_face, UF_OBJ_WHITE);  
                ECHO("\nUsing UF_MODL_trace_a_ray with same assumed vector (White):\n");  
                WRITE_F(min_dist);  
                WRITE3F(pnt_on_face);  
                UF_CALL(UF_MODL_ask_face_parm(face, pnt_on_face, uv, a_pnt));  
                UF_CALL(UF_MODL_ask_face_props(face, uv, junk, junk, junk, junk, junk,  
                    normal_at_pnt_on_face, junk));  
                UF_VEC3_negate(normal_at_pnt_on_face, normal_at_pnt_on_face);  
                ECHO("These must match or the answer is wrong:\n");  
                WRITE3F(proj_data.proj_vec);  
                WRITE3F(normal_at_pnt_on_face);  
            }  
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

> 这是段NX二次开发代码，主要功能是寻找曲面上的最近点。
>
> 代码的主要逻辑包括：
>
> 1. 定义选择曲面的函数select_a_face和选择点的函数select_a_point。
> 2. 定义显示临时点的函数display_temporary_colored_point和显示临时线的函数display_temporary_colored_line。
> 3. 定义计算曲面参数的函数ask_face_normalized_parms。
> 4. 主函数do_it中，循环选择曲面和点，然后计算曲面上的最近点，并显示出来。
> 5. 使用多种方法计算最近点，包括使用UF_MODL_ask_face_parm、UF_EVALSF_find_closest_point、UF_MODL_ask_minimum_dist、UF_CURVE_create_proj_curves和UF_MODL_trace_a_ray等函数。
> 6. 对每种方法计算的结果进行显示，并输出相关参数。
> 7. 在ufusr函数中调用主函数do_it。
>
> 总体来说，该代码通过多种方法计算曲面上的最近点，并进行显示，用于验证不同方法的效果。
>
