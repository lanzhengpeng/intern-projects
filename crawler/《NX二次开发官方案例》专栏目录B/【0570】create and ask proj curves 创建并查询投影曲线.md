### 【0570】create and ask proj curves 创建并查询投影曲线

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_assem.h>  
    #include <uf_curve.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <stdarg.h>  
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
    /* 里海 */  
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[5] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, UF_all_subtype, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_point_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 5, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_curves(char *prompt, tag_t **curves)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select curves", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_curves, NULL, &resp, &cnt, curves));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*curves)[ii], FALSE));  
        return cnt;  
    }  
    /* 里海 */  
    static int mask_for_face_plane_datums(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_plane_type, 0, 0 },  
                        { UF_datum_plane_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_face_plane_datums(char *prompt, tag_t **items)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART,  
            mask_for_face_plane_datums, NULL, &resp, &cnt, items));  
        objs = *items;  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], FALSE));  
        return cnt;  
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
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void number_objects(tag_t *objects, int n_objects)  
    {  
        int  
            ii;  
        double  
            loc[3];  
        char  
            msg[133];  
        for (ii = 0; ii < n_objects; ii++)  
        {  
            sprintf(msg, "%d", ii+1);  
            if (UF_OBJ_ask_name_origin(objects[ii], loc))  
            {  
                UF_CALL(UF_OBJ_set_name(objects[ii], "X"));  
                UF_CALL(UF_OBJ_ask_name_origin(objects[ii], loc));  
                UF_CALL(UF_OBJ_delete_name(objects[ii]));  
            }  
            display_temporary_text(msg, loc);  
        }  
    }  
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int  
            ii,  
            subtype,  
            type;  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(objects[ii], &type, &subtype));  
            if (type == UF_occ_instance_type)  
                UF_CALL(UF_DISP_set_highlight(UF_ASSEM_ask_part_occ_of_inst(  
                    NULL_TAG, objects[ii]), flag));  
            else  
                UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
        }  
    }  
    static tag_t ask_occurrence_of_promotion(tag_t prom_object)  
    {  
        int  
            ii,  
            np;  
        tag_t  
            part_occ = NULL_TAG,  
           *path;  
        UF_CALL(UF_MODL_ask_promotion_path(prom_object, TRUE, &path, &np));  
        for (ii = np - 1; ii >= 0; ii--)  
            part_occ = UF_ASSEM_ask_part_occ_of_inst(part_occ, path[ii]);  
        UF_free(path);  
        return (UF_ASSEM_find_occurrence(part_occ, prom_object));  
    }  
    static void convert_promotions_to_occurrences(int n_objs, tag_t *objs)  
    {  
        int  
            ii;  
        for (ii = 0; ii < n_objs; ii++)  
            if (UF_OBJ_is_object_a_promotion(objs[ii]))  
                objs[ii] = ask_occurrence_of_promotion(objs[ii]);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_curves,  
            n_proj,  
            n_targets;  
        tag_t  
            *curves,  
            parent_feat,  
            parent_target,  
            parent_curve,  
            *proj_curves,  
            *targets,  
            proj_feat;  
        char  
            msg[133];  
        UF_CURVE_proj_t  
            proj_data = { 3, NULL_TAG, {0,0,0}, {0,0,0}, 1, 0, 0, {0,0,0}};  
        while (  
            (n_curves = select_curves("Project", &curves)) &&  
            (n_targets = select_face_plane_datums("Project onto", &targets)) &&  
            (specify_vector("Projection direction", proj_data.proj_vec,  
                proj_data.ref_pnt)))  
        {  
            convert_promotions_to_occurrences(n_targets, targets);  
            if (!UF_CALL(UF_CURVE_create_proj_curves(n_curves, curves, n_targets,  
                    targets, 3, &proj_data, &proj_feat)))  
            {  
                UF_CALL(UF_CURVE_ask_proj_curves(proj_feat, &n_proj, &proj_curves));  
                number_objects(proj_curves, n_proj);  
                set_highlight_object_array(n_proj, proj_curves, TRUE);  
                sprintf(msg, "feat %d -> %d projection curves", proj_feat, n_proj);  
                uc1601(msg, TRUE);  
                set_highlight_object_array(n_proj, proj_curves, FALSE);  
                UF_DISP_refresh();  
                for (ii = 0; ii < n_proj; ii++)  
                {  
                    if (UF_CALL(UF_CURVE_ask_proj_curve_parents(proj_curves[ii],  
                        &parent_feat, &parent_target, &parent_curve))) continue;  
                    UF_CALL(UF_DISP_set_highlight(proj_curves[ii], TRUE));  
                    UF_CALL(UF_DISP_set_highlight(parent_target, TRUE));  
                    UF_CALL(UF_DISP_set_highlight(parent_curve, TRUE));  
                    sprintf(msg,"feat %d curve/target # %d", parent_feat, ii+1);  
                    uc1601(msg, TRUE);  
                    UF_CALL(UF_DISP_set_highlight(proj_curves[ii], FALSE));  
                    UF_CALL(UF_DISP_set_highlight(parent_target, FALSE));  
                    UF_CALL(UF_DISP_set_highlight(parent_curve, FALSE));  
                }  
                if (n_proj > 0) UF_free(proj_curves);  
            }  
            UF_free(curves);  
            UF_free(targets);  
        }  
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

```

#### 代码解析

> 这段NX二次开发代码的主要功能是进行曲线投影操作。具体来说，它实现了以下功能：
>
> 1. 选择曲线和投影面：使用UI选择对话框让用户选择曲线和投影面，并进行相应的处理。
> 2. 指定投影方向：使用UI对话框让用户指定投影方向。
> 3. 创建曲线投影：使用UF_CURVE_create_proj_curves函数根据选择的曲线和投影面创建曲线投影。
> 4. 高亮显示结果：高亮显示创建的曲线投影，并显示其父特征、目标面和源曲线。
> 5. 重复执行：重复上述步骤，直到用户选择退出。
> 6. 错误处理和消息提示：实现了错误处理机制，使用UF_get_fail_message函数获取错误信息，并显示在日志窗口和状态栏。
> 7. 内存管理：在适当的位置释放内存，避免内存泄漏。
> 8. 初始化和结束：在程序开始和结束时调用UF_initialize和UF_terminate进行初始化和结束操作。
>
> 总的来说，这段代码实现了曲线投影操作的核心功能，并且具有良好的用户交互和错误处理。
>
