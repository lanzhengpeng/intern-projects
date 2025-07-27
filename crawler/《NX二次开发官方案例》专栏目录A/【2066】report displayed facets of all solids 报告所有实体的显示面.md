### 【2066】report displayed facets of all solids 报告所有实体的显示面

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_facet.h>  
    #include <uf_disp.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_view.h>  
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
    static tag_t ask_next_solid_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype)  
            {  
                UF_CALL(UF_MODL_ask_body_type(body, &type));  
                if (type == UF_MODL_SOLID_BODY) return body;  
            }  
        }  
        return NULL_TAG;  
    }  
    static void display_temporary_line(double start[3], double end[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            start, end, &props));  
    }  
    static void display_temporary_facets(tag_t model)  
    {  
        int  
            ii,  
            jj,  
            n_facets,  
            n_verts,  
            n_total_verts = 0;  
        double   
            (* verts)[3];  
        UF_CALL(UF_FACET_ask_max_facet_verts(model, &n_verts));  
        ECHO("  Facet model %d contains max %d vertices\n", model, n_verts);  
        verts = (double(*)[3])malloc(n_verts * 3 * sizeof(double));  
        UF_CALL(UF_FACET_ask_n_facets_in_model(model, &n_facets));  
        for (ii = 0; ii < n_facets; ii++)  
        {  
            UF_CALL(UF_FACET_ask_vertices_of_facet(model, ii, &n_verts, verts));  
            n_total_verts += n_verts;  
            for (jj = 0; jj < (n_verts - 1); jj++)  
                display_temporary_line(verts[jj], verts[jj+1]);  
            display_temporary_line(verts[n_verts-1], verts[0]);  
        }  
        free(verts);  
        ECHO("  Total vertices in facet model %d: %d\n", model, n_total_verts);  
    }  
    static void ask_display_facet_parameters(UF_FACET_parameters_p_t params)  
    {  
        int  
            units;  
        tag_t  
            part = UF_PART_ask_display_part();  
        double  
            cf = 1,  
            view_center[3],  
            view_scale;  
        UF_DISP_system_params_t  
            sys_params;  
        UF_CALL(UF_DISP_ask_system_parameters(&sys_params));  
        UF_CALL(uc6430("", view_center, &view_scale));  
        UF_CALL(UF_PART_ask_units(part, &units));  
        if (units == UF_PART_METRIC) cf = 25.4;  
        UF_FACET_INIT_PARAMETERS(params);  
        params->specify_surface_tolerance = TRUE;  
        params->surface_dist_tolerance = cf * sys_params.facet_chord_tol/view_scale;  
        params->surface_angular_tolerance = sys_params.facet_angle_tol * DEGRA;  
        params->specify_curve_tolerance = TRUE;  
        params->curve_dist_tolerance = cf * sys_params.facet_edge_tol/view_scale;  
        params->curve_angular_tolerance = sys_params.facet_angle_tol * DEGRA;  
    }  
    static void report_facet_model(tag_t model)  
    {  
        int  
            facet = UF_FACET_NULL_FACET_ID,  
            m_cnt,  
            n_faces;  
        UF_CALL(UF_FACET_ask_n_facets_in_model(model, &m_cnt));  
        UF_CALL(UF_FACET_ask_num_faces(model, &n_faces));  
        ECHO("  %d facets total in %d faces\n", m_cnt, n_faces);  
        UF_DISP_refresh();  
        display_temporary_facets(model);  
        uc1601("facets displayed", TRUE);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            model = NULL_TAG,  
            solid = NULL_TAG,  
            part = UF_PART_ask_display_part();  
       UF_FACET_parameters_t  
            params;  
        while ((solid = ask_next_solid_body(part, solid)) != NULL_TAG)  
        {  
            ask_display_facet_parameters(&params);  
            UF_CALL(UF_FACET_facet_solid(solid, &params, &model));  
            ECHO("\nFacet model using display like parameters on solid body[%d]:\n", solid);  
            report_facet_model(model);  
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

> 这段NX二次开发代码的主要功能是遍历当前打开的部件中的所有实体体，并基于显示参数对每个实体体进行面片化。
>
> 主要步骤包括：
>
> 1. 包含NX Open相关头文件，定义宏函数UF_CALL来简化UF函数调用并报告错误。
> 2. 定义函数ask_next_solid_body，用于遍历并获取部件中的下一个实体体。
> 3. 定义函数display_temporary_line，用于显示临时线。
> 4. 定义函数display_temporary_facets，用于显示模型的面片。
> 5. 定义函数ask_display_facet_parameters，用于获取面片化参数。
> 6. 定义函数report_facet_model，用于报告面片模型信息并显示面片。
> 7. 定义函数do_it，用于遍历所有实体体，基于显示参数进行面片化，并报告面片模型信息。
> 8. 定义ufusr函数，作为NX二次开发程序的入口，初始化后执行do_it函数，最后清理并退出。
> 9. 定义ufusr_ask_unload函数，用于卸载动态库。
>
> 该代码的主要功能是遍历部件中的所有实体体，基于显示参数对面片化，并显示面片结果，以帮助用户直观地了解面片化的效果。
>
