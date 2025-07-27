### 【2542】section solids at points 在点上生成实体截面

#### 代码

```cpp
    /*HEAD SECTION_SOLIDS_AT_POINTS CCC UFUN */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 版本中新增的函数。 */  
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
    static logical select_points(char *prompt, tag_p_t *points, int *cnt)  
    {  
        int  
            i,  
            resp;  
        UF_UI_selection_options_t  
            opts;  
        UF_UI_mask_t  
            mask;  
        opts.num_mask_triples = 1;  
        opts.mask_triples = &mask;  
        opts.scope = UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY;  
        mask.object_type = UF_point_type;  
        mask.object_subtype = 0;  
        mask.solid_type = 0;  
        UF_CALL(UF_UI_select_by_class(prompt, &opts, &resp, cnt, points));  
        if (resp == 3)  
        {  
            for (i=0; i<*cnt; i++)  
                UF_CALL(UF_DISP_set_highlight(*(*points+i),FALSE));  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static logical select_bodies(char *prompt, tag_p_t *bodies, int *cnt)  
    {  
        int  
            i,  
            resp;  
        UF_UI_selection_options_t  
            opts;  
        UF_UI_mask_t  
            mask;  
        opts.num_mask_triples = 1;  
        opts.mask_triples = &mask;  
        opts.scope = UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY;  
        mask.object_type = UF_solid_type;  
        mask.object_subtype = 0;  
        mask.solid_type = UF_UI_SEL_FEATURE_BODY;  
        UF_CALL(UF_UI_select_by_class(prompt, &opts, &resp, cnt, bodies));  
        if (resp == 3)  
        {  
            for (i=0; i<*cnt; i++)  
                UF_CALL(UF_DISP_set_highlight(*(*bodies+i),FALSE));  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            i, j,  
            irc,  
            num_bodies,  
            num_curves,  
            num_points,  
            ten = 10;  
        tag_p_t  
            bodies,  
            planes,  
            points;  
        tag_t  
            curves[10];  
        double  
            point_coords[3],  
            x_point[3],  
            y_point[3];  
        if (select_points("Select points", &points, &num_points))  
        {  
            planes = points;  /* store the planes on top of the points 里海译:将飞机存储在点的顶部 */  
            for (i=0; i<num_points; i++)  
            {  
                UF_CALL( UF_CURVE_ask_point_data( points[i], point_coords));  
                UF_VEC3_copy( point_coords, x_point);  
                ++x_point[0];  
                UF_VEC3_copy( point_coords, y_point);  
                ++y_point[1];  
                FTN (uf5374) (point_coords, x_point, y_point, &planes[i]);  
            }  
            if (select_bodies("Select bodies", &bodies, &num_bodies))  
            {  
                for (i=0; i<num_points; i++)  
                {  
                    FTN (uf6524)( bodies, &num_bodies, &planes[i], &ten,  
                            &irc, &num_curves, curves );  
                    UF_CALL( irc );  
                    for (j=0; j<num_curves; j++)  
                        UF_CALL(UF_DISP_add_item_to_display(curves[j]));  
                }  
                UF_free(bodies);  
            }  
            UF_free(points);  
        }  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX的二次开发示例，主要功能是允许用户选择点，然后在每个点上创建一个平面，然后选择实体，在每个平面上创建与实体接触的曲线。
>
> 代码主要包含以下部分：
>
> 1. 头文件包含：包含了UF框架相关的头文件。
> 2. 错误处理宏：定义了UF_CALL宏，用于调用UF函数时自动处理错误。
> 3. 选择点函数：实现了选择点的功能。
> 4. 选择实体函数：实现了选择实体的功能。
> 5. 主体函数：实现了创建平面和曲线的功能。
> 6. ufusr函数：作为二次开发的入口函数，用于初始化和终止UF框架，并调用主体函数。
> 7. 卸载函数：定义了立即卸载二次开发代码的卸载函数。
>
> 这段代码实现了创建平面和曲线的基本功能，为NX的二次开发提供了示例。
>
