### 【1462】intersect a body with a ray 用射线与实体求交

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_mtx.h>  
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
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
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
                ECHO("%.3f", array[kk++]);  
                if (jj < cols - 1) ECHO("%s", ", ");  
            }  
            if (cols > 0) ECHO("%s", "\n");  
        }  
    }  
    /* qq3123197280 */  
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
    static tag_t select_a_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
        else return FALSE;  
    }  
    static logical specify_vector(char *prompt, double vec[3], double point[3])  
    {  
        int  
            mode = UF_UI_NO_DEFAULT,  
            resp;  
        UF_CALL(UF_UI_specify_vector(prompt, &mode, UF_UI_DISP_TEMP_VECTOR,  
            vec, point, &resp));  
        if (resp == UF_UI_OK) return TRUE;  
        else return FALSE;  
    }  
    #define MAX_HITS 10  
    static void do_it(void)  
    {  
        int  
            ii,  
            irc,  
            n_hits,  
            one = 1,  
            max_hits = MAX_HITS;  
        tag_t  
            body,  
            hit_bodies[MAX_HITS];  
        double  
            hit_points[3*MAX_HITS],  
            identity3[9],  
            identity4[16],  
            junk[3],  
            hit_normals[3*MAX_HITS],  
            ray_origin[3] = { -248.35, 2052.06, 358.99 },  
            ray_direction[3] = { 1, 0, 0 },  
            zero[3] = { 0, 0, 0 };  
        char  
            msg[133];  
        UF_MODL_ray_hit_point_info_t  
            *hit_list;  
        UF_MTX4_identity(identity4);  
        UF_MTX3_identity(identity3);  
        if (body = select_a_body("Select body to intersect"))  
            while (specify_point("Ray Origin", ray_origin) &&  
                specify_vector("Ray Direction", ray_direction, junk))  
            {  
                ECHO("\n");  
                WRITE3F(ray_origin);  
                WRITE3F(ray_direction);  
                UF_DISP_refresh();  
                UF_DISP_labeled_conehead(UF_DISP_WORK_VIEW_ONLY, ray_origin,  
                    ray_direction, 0, "Ray");  
                if (UF_CALL(UF_MODL_trace_a_ray(1, &body, ray_origin,  
                    ray_direction, identity4, 0, &n_hits, &hit_list))) continue;  
                ECHO("\nUsing UF_MODL_trace_a_ray:  ");  
                WRITE_D(n_hits);  
                for (ii = 0; ii < n_hits; ii++)  
                {  
                    ECHO("Hit #%d:  %.3f, %.3f, %.3f\n", ii+1,  
                        hit_list[ii].hit_point[0],  
                        hit_list[ii].hit_point[1],  
                        hit_list[ii].hit_point[2]);  
                    sprintf(msg, "Hit #%d", ii+1);  
                    UF_DISP_labeled_conehead(UF_DISP_WORK_VIEW_ONLY,  
                        hit_list[ii].hit_point, hit_list[ii].hit_normal, 0, msg);  
                }  
                if (n_hits > 0) UF_free(hit_list);  
        //  Sometimes the older function finds more hits.  See PR 6632277  
                FTN(uf6545)(&body, &one, identity3, zero, &max_hits, ray_origin,  
                    ray_direction, &irc, &n_hits, hit_points, hit_normals,  
                    hit_bodies);  
                if (irc == 0)  
                {  
                    ECHO("\nUsing uf6545:  ");  
                    WRITE_D(n_hits);  
                    for (ii = 0; ii < n_hits; ii++)  
                    {  
                        ECHO("Hit #%d:  %.3f, %.3f, %.3f\n", ii+1,  
                            hit_points[(ii*3) + 0], hit_points[(ii*3) + 1],  
                            hit_points[(ii*3) + 2]);  
                        sprintf(msg, "Hit #%d", ii+1);  
                        UF_DISP_labeled_conehead(UF_DISP_WORK_VIEW_ONLY,  
                            &(hit_points[ii*3]), &(hit_normals[ii*3]), 0, msg);  
                    }  
                }  
                else  
                    ECHO("uf6545 returned an error code = %d\n", irc);  
            }  
    }  
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

> 这段代码是用于NX二次开发的示例代码，主要实现了以下功能：
>
> 1. 定义了错误报告函数report_error，用于报告函数调用失败时的错误信息。
> 2. 定义了输出函数WRITE_D和WRITE3F，用于在列表窗口输出整数和浮点数数组。
> 3. 定义了mask_for_bodies函数，用于设置选择遮罩，只允许选择实体。
> 4. 定义了select_a_body函数，用于通过对话框选择一个实体。
> 5. 定义了specify_point和specify_vector函数，用于指定点和向量。
> 6. 定义了do_it函数，实现了主要功能：选择一个实体指定点和方向，绘制射线使用UF_MODL_trace_a_ray函数获取射线与实体的交点信息，并绘制交点使用uf6545函数获取射线与实体的交点信息，并绘制交点
> 7. 选择一个实体
> 8. 指定点和方向，绘制射线
> 9. 使用UF_MODL_trace_a_ray函数获取射线与实体的交点信息，并绘制交点
> 10. 使用uf6545函数获取射线与实体的交点信息，并绘制交点
> 11. 定义了ufusr函数，作为NX二次开发的入口函数，调用do_it函数。
> 12. 定义了ufusr_ask_unload函数，用于卸载NX二次开发应用程序。
>
> 总体来说，这段代码实现了射线与实体求交的功能，并提供了交互式选择实体和指定射线的点与方向，最后绘制出射线与实体的交点。
>
