### 【1107】correlate facet model vs face facet counts 关联facet模型与面的facet计数

#### 代码

```cpp
    #include <stdio.h>  
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
    #include <uf_undo.h>  
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
    static int ask_all_solids(tag_t part, tag_t **solids)  
    {  
        tag_t  
            solid = NULL_TAG;  
        uf_list_p_t  
            solid_list;  
        UF_CALL(UF_MODL_create_list(&solid_list));  
        while ((solid = ask_next_solid_body(part, solid)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(solid_list, solid));  
        return (make_an_array(&solid_list, solids));  
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
            n_verts;  
        double  
            (*verts)[3];  
        UF_CALL(UF_FACET_ask_max_facet_verts(model, &n_verts));  
        UF_CALL(allocate_memory(n_verts * 3 * sizeof(double), (void *)&verts));  
        UF_CALL(UF_FACET_ask_n_facets_in_model(model, &n_facets));  
        for (ii = 0; ii < n_facets; ii++)  
        {  
            UF_CALL(UF_FACET_ask_vertices_of_facet(model, ii, &n_verts, verts));  
            for (jj = 0; jj < (n_verts - 1); jj++)  
                display_temporary_line(verts[jj], verts[jj+1]);  
            display_temporary_line(verts[n_verts-1], verts[0]);  
        }  
        UF_free(verts);  
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
    #define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \  
        else ECHO("%s = FALSE\n", #X);  
    static void report_facet_model(tag_t model)  
    {  
        int  
            f_cnt,  
            facet = UF_FACET_NULL_FACET_ID,  
            ii,  
            m_cnt,  
            n_faces;  
        UF_CALL(UF_FACET_ask_n_facets_in_model(model, &m_cnt));  
        UF_CALL(UF_FACET_ask_num_faces(model, &n_faces));  
        ECHO("  %d facets total in %d faces\n", m_cnt, n_faces);  
        m_cnt = 0;  
        facet = UF_FACET_NULL_FACET_ID;  
        while (!UF_CALL(UF_FACET_cycle_facets(model, &facet)) &&  
            (facet != UF_FACET_NULL_FACET_ID)) m_cnt++;  
        ECHO("    cycling model found %d facets total\n", m_cnt);  
        m_cnt = 0;  
        for (ii = 0; ii < n_faces; ii++)  
        {  
            UF_CALL(UF_FACET_ask_num_facets_in_face(model, ii, &f_cnt));  
            ECHO("    face_id %d has %d facets\n", ii, f_cnt);  
            m_cnt = m_cnt + f_cnt;  
            f_cnt = 0;  
            facet = UF_FACET_NULL_FACET_ID;  
            if (UF_CALL(UF_FACET_cycle_facets_in_face(model, ii, &facet)))  
            {  
                ECHO("see PR 6458365\n");  
                break;  
            }  
            while (facet != UF_FACET_NULL_FACET_ID)  
            {  
                f_cnt++;  
                UF_FACET_cycle_facets_in_face(model, ii, &facet);  
            }  
            ECHO("      cycling face_id %d found %d facets\n", ii, f_cnt);  
        }  
        ECHO("  %d faces have %d total facets\n", n_faces, m_cnt);  
        UF_DISP_refresh();  
        display_temporary_facets(model);  
        uc1601("facets displayed", TRUE);  
    }  
    static void do_it(void)  
    {  
        logical  
            up_to_date;  
        int  
            ii, jj,  
            n_bodies,  
            n_models;  
        tag_t  
            *bodies,  
            model,  
            *models,  
            part = UF_PART_ask_display_part();  
        UF_FACET_parameters_t  
            params;  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, "", &mark_id));  
        UF_FACET_INIT_PARAMETERS(&params);  
        n_bodies = ask_all_solids(part, &bodies);  
        ECHO("n_bodies = %d\n", n_bodies);  
        for (ii = 0; ii < n_bodies; ii++)  
        {  
            UF_CALL(UF_FACET_ask_models_of_solid(bodies[ii], &n_models, &models));  
            ECHO(" bodies[%d] has %d facet models\n", ii, n_models);  
            for (jj = 0; jj < n_models; jj++)  
            {  
                UF_CALL(UF_FACET_ask_model_parameters(models[jj], &params));  
                ECHO("  Original model[%d]:\n", jj);  
                UF_CALL(UF_FACET_is_model_up_to_date(models[jj], &up_to_date));  
                WRITE_L(up_to_date);  
                if (!up_to_date)  
                {  
                    ECHO("  Updating...\n");  
                    UF_CALL(UF_FACET_update_model(models[jj], &params));  
                }  
                report_facet_model(models[jj]);  
                UF_CALL(UF_FACET_facet_solid(bodies[ii], &params, &model));  
                ECHO("\n  New model using parameters from model[%d]:\n", jj);  
                report_facet_model(model);  
            }  
            if (n_models > 0) UF_free(models);  
            UF_CALL(UF_FACET_ask_default_parameters(&params));  
            UF_CALL(UF_FACET_facet_solid(bodies[ii], &params, &model));  
            ECHO("\n  New model using default parameters on bodies[%d]:\n", ii);  
            report_facet_model(model);  
            ask_display_facet_parameters(&params);  
            UF_CALL(UF_FACET_facet_solid(bodies[ii], &params, &model));  
            ECHO("\n  New model using display like parameters on bodies[%d]:\n", ii);  
            report_facet_model(model);  
        }  
        if (n_bodies > 0) UF_free(bodies);  
        UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));  
        UF_CALL(UF_UNDO_delete_mark(mark_id, NULL));  
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
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            ECHO("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                ECHO("  ");  
                ECHO(status->file_names[ii]);  
                ECHO(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                ECHO(msg);  
                ECHO("\n");  
            }  
            ECHO("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    /*  To run in external native mode NX:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external NX Manager mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            msg[UF_UI_MAX_STRING_LEN],  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                sprintf(msg, "%d.  %s\n", ++cnt, part_name);  
                ECHO(msg);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) report_load_status(&status);  
                if (!status.failed)  
                {  
                    do_it();  
                    UF_CALL(UF_PART_close_all());  
                }  
            }  
            sprintf(msg, "\nProcessed %d parts.\n", cnt);  
            ECHO(msg);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段NX二次开发代码主要用于查询和显示NX模型中的面片(facet)信息。代码的主要逻辑和功能包括：
>
> 1. 初始化和资源释放：在函数ufusr中调用UF_initialize()初始化NX环境，并在结束时调用UF_terminate()释放资源。
> 2. 查询和显示面片信息：do_it()函数实现了查询和显示面片信息的主要逻辑。首先，它遍历模型中的所有实体，并查询每个实体的面片模型。然后，它会显示面片数量、面片与面的关系等统计信息，并调用display_temporary_facets()函数临时显示面片。此外，该函数还会更新面片模型，并使用默认和显示参数重新生成面片模型。
> 3. 面片模型查询：ask_all_solids()函数查询模型中的所有实体，并返回它们的数组。ask_next_solid_body()函数用于查询下一个实体。make_an_array()函数用于将实体列表转换为实体数组。
> 4. 错误处理和内存管理：代码定义了宏UF_CALL来简化错误处理。allocate_memory()函数用于申请内存。report_error()函数用于报告错误。ECHO()函数用于在控制台和日志中输出信息。
> 5. NX启动和参数输入：在main()函数中，代码首先初始化NX环境，然后通过uc4621()和uc4624()获取启动参数中的部件名称。对于每个部件，它打开并处理，然后关闭。在结束前，它会统计处理的部件数量。
> 6. 加载状态报告：report_load_status()函数用于报告部件加载状态，包括加载的部件数量、文件名以及每个部件的加载状态。
> 7. NX部件加载：代码通过UF_PART_open()函数打开部件，通过UF_PART_close_all()函数关闭所有部件。
>
> 总体来说，这段代码通过查询模型信息、显示面片、更新面片模型等操作，提供了对NX模型面片信息的全面处理。
>
