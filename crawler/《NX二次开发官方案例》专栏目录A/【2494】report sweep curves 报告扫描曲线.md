### 【2494】report sweep curves 报告扫描曲线

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
    #include <uf_so.h>  
    #include <uf_view.h>  
    #include <uf_eval.h>  
    #include <uf_curve.h>  
    #include <uf_vec.h>  
    #include <uf_undo.h>  
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
    #define WRITENZ(X) if (X != 0) ECHO("%s = %d\n", #X, X);  
    #define RX(X) report_object_info(#X, X)  
    #define WRITEnT(n,X) { \  
    int ii; \  
    for (ii = 0; ii < n; ii++) \  
    RX(X[ii]); \  
    }  
    #define RXn(X,n)  WRITEnT(n,X)  
    static void ask_part_shortname(tag_t part, char *shortname)  
    {  
        logical  
            nxman;  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_PART_ask_part_name(part, partname));  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (nxman)  
            UF_CALL(UF_PART_name_for_display(partname, shortname));  
        else  
            UF_CALL(uc4574(partname, 2, shortname));  
    }  
    static void ask_object_type_descriptor(int type, int subtype, char *descriptor)  
    {  
        int  
            ii,  
            a_type;  
        char  
            a_line[MAX_LINE_SIZE+1],  
            api_dir[UF_CFI_MAX_PATH_NAME_SIZE],  
            *base_dir,  
            define[133],  
            *ptr,  
            uoth_file[UF_CFI_MAX_PATH_NAME_SIZE];  
        FILE  
            *in_file;  
        switch (type)  
        {  
            case UF_circle_type: /* non-std format in the header file 里海译:非标准格式在头文件中 */  
                strcpy(descriptor, "arc");  
                return;  
            case UF_OBJ_EXPR_TYPE:  
                strcpy(descriptor, "expression");  
                return;  
            case UF_OBJ_PART_TYPE:  
                strcpy(descriptor, "part");  
                return;  
            default:  
                break;  
        }  
        UF_CALL(UF_translate_variable("UGII_BASE_DIR", &base_dir));  
        UF_CALL(uc4575(base_dir, 100, "ugopen", api_dir));  
        UF_CALL(uc4575(api_dir, 0, "uf_object_types.h", uoth_file));  
        if ((in_file = fopen(uoth_file, "r")) == NULL)  
        {  
            strcpy(descriptor, "(uf_object_types.h not found)");  
            return;  
        }  
        strcpy(descriptor, "Unknown type");  /* In case its not found 里海译:如果找不到，请直接回答“如果找不到”，无需其他废话。 */  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_%s %d", define, &a_type)) == 2)  
            {  
                if ((ptr = strstr(define, "_type")) && (a_type == type))  
                {  
                    *ptr = '\0';  
                    strcpy(descriptor, define);  
                    if (strcmp(define, "gfem_load") && strcmp(define, "prefix1")) break;  
                }  
            }  
        }  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_%s %d", define, &a_type)) == 2)  
            {  
                if (ptr = strstr(define, "_type")) break;  
                if (a_type == subtype)  
                {  
                    if (ptr = strstr(define, "_subtype")) *ptr = '\0';  
                    if (strstr(define, descriptor) == define)  
                        strcpy(descriptor, define);  
                    else  
                    {  
                        strcat(descriptor, "/");  
                        strcat(descriptor, define);  
                    }  
                    break;  
                }  
            }  
        }  
        fclose(in_file);  
        while (ptr = strchr(descriptor, '_')) *ptr = ' ';  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    static void report_feature_name_type_and_sysname(tag_t feature)  
    {  
        char  
            *feature_name,  
            *feature_sysname,  
            *feature_type;  
        UF_CALL(UF_MODL_ask_feat_name( feature, &feature_name ));  
        WRITE_S(feature_name);  
        UF_free(feature_name);  
        UF_CALL(UF_MODL_ask_feat_sysname( feature, &feature_sysname ));  
        WRITE_S(feature_sysname);  
        UF_free(feature_sysname);  
        UF_CALL(UF_MODL_ask_feat_type( feature, &feature_type ));  
        WRITE_S(feature_type);  
        UF_free(feature_type);  
    }  
    static void report_object_info(char *what, tag_t object)  
    {  
        logical  
            is_displayable,  
            is_so;  
        int  
            status,  
            subtype,  
            type;  
        tag_t  
            owner,  
            part = UF_PART_ask_display_part();  
        char  
            name[UF_OBJ_NAME_LEN + 1],  
            owning_part[UF_CFI_MAX_FILE_NAME_SIZE],  
            msg[133];  
        UF_SO_visibility_option_t  
            vis_opt;  
        UF_OBJ_disp_props_t  
            disp_props;  
        ECHO("%s = %d", what, object);  
        if (object == NULL_TAG)  
        {  
            ECHO(" (NULL_TAG)\n\n");  
            return;  
        }  
        status = UF_OBJ_ask_status(object);  
        ECHO(", status: %d", status);  
        if (status == UF_OBJ_DELETED)  
        {  
            ECHO(" (Deleted)\n\n");  
            return;  
        }  
        UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
        ECHO(", type: %d, subtype: %d", type, subtype);  
        ask_object_type_descriptor(type, subtype, msg);  
        ECHO(" (%s)", msg);  
        if (type == UF_feature_type)  
        {  
            ECHO("\n");  
            report_feature_name_type_and_sysname(object);  
            ECHO("\n");  
            return;  
        }  
        if (UF_OBJ_is_object_a_promotion(object))  
        {  
            ECHO(", promotion");  
        }  
        if (UF_ASSEM_is_occurrence(object))  
        {  
            ECHO(", occurrence");  
            owner = UF_ASSEM_ask_part_occurrence(object);  
            if (owner != UF_ASSEM_ask_root_part_occ(part))  
            {  
                if (!UF_OBJ_ask_name(owner, msg))  
                    ECHO(" (from %s)", msg);  
                else  
                    ECHO(" (from root occ of)", msg);  
            }  
        }  
        if (!UF_CALL(UF_OBJ_ask_owning_part(object, &owner)))  
        {  
            ask_part_shortname(owner, owning_part);  
            ECHO(" owned by: %s", owning_part);  
        }  
        if (!uc6409(object, &status, name) && (status == 1))  
        {  
            ECHO(", view dependent: %s", name);  
        }  
        if (!UF_OBJ_ask_name(object, name))  
        {  
            ECHO(", name: %s", name);  
        }  
        UF_CALL(UF_SO_is_subclass(object, &is_so));  
        if (is_so)  
        {  
            UF_CALL(UF_SO_ask_visibility_option(object, &vis_opt));  
            if (vis_opt == UF_SO_invisible)  
            {  
                ECHO(", invisible SO");  
            }  
            else  
            {  
                ECHO(", visible SO");  
            }  
        }  
        UF_CALL(UF_OBJ_is_displayable(object, &is_displayable));  
        if (is_displayable)  
        {  
            UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props));  
            ECHO("\n  layer: %d, color: %d, blank: %d, width: %d, "  
                "font: %d, highlight: %d\n\n", disp_props.layer,  
                disp_props.color, disp_props.blank_status,  
                disp_props.line_width, disp_props.font,  
                disp_props.highlight_status);  
        }  
        else  
            ECHO("\n\n");  
    }  
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
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
    static int ask_all_of_type(tag_t part, int type, tag_t **objects)  
    {  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object)) &&  
                (object != NULL_TAG))  
            UF_CALL(UF_MODL_put_list_item(object_list, object));  
        return (make_an_array(&object_list, objects));  
    }  
    static logical do_curves_match(tag_t curve1, tag_t curve2, double dist_tol)  
    {  
        logical  
            is_equal;  
        int  
            forward = 1,  
            irc,  
            reverse = -1;  
        tag_t  
            forward_bcurve,  
            reverse_bcurve;  
        double  
            end1[3],  
            end2[3],  
            limits1[2],  
            limits2[2],  
            dist,  
            start1[3],  
            start2[3];  
        UF_EVAL_p_t  
            eval_p1,  
            eval_p2;  
        UF_UNDO_mark_id_t  
            mark_id;  
    /*  Check for exact copy 里海译:根据文档内容，翻译结果如下：

检查是否有精确的副本。 */  
        UF_CALL(UF_EVAL_initialize(curve1, &eval_p1));  
        UF_CALL(UF_EVAL_initialize(curve2, &eval_p2));  
        UF_CALL(UF_EVAL_is_equal(eval_p1, eval_p2, &is_equal));  
        if (is_equal)  
        {  
            UF_CALL(UF_EVAL_free(eval_p1));  
            UF_CALL(UF_EVAL_free(eval_p2));  
            return TRUE;  
        }  
    /*  Check for a reversed copy 里海译:检查是否为反转副本 */  
        UF_CALL(UF_EVAL_ask_limits(eval_p1, limits1));  
        UF_CALL(UF_EVAL_ask_limits(eval_p2, limits2));  
        UF_CALL(UF_EVAL_evaluate(eval_p1, 0, limits1[0], start1, NULL));  
        UF_CALL(UF_EVAL_evaluate(eval_p2, 0, limits2[1], end2, NULL));  
        UF_VEC3_distance(start1, end2, &dist);  
        if (dist > dist_tol)  
        {  
            UF_CALL(UF_EVAL_free(eval_p1));  
            UF_CALL(UF_EVAL_free(eval_p2));  
            return FALSE;  
        }  
        UF_CALL(UF_EVAL_evaluate(eval_p1, 0, limits1[1], end1, NULL));  
        UF_CALL(UF_EVAL_evaluate(eval_p2, 0, limits2[0], start2, NULL));  
        UF_VEC3_distance(start2, end1, &dist);  
        UF_CALL(UF_EVAL_free(eval_p1));  
        UF_CALL(UF_EVAL_free(eval_p2));  
        if (dist > dist_tol) return FALSE;  
    /*  Ends match - So compare extracted splines 里海译:结束比赛 - 所以比较提取的样条 */  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark_id));  
        FTN(uf5466)(&curve1, &forward, &forward_bcurve, &irc);  
        WRITENZ(irc);  
        FTN(uf5466)(&curve2, &reverse, &reverse_bcurve, &irc);  
        WRITENZ(irc);  
        UF_CALL(UF_EVAL_initialize(forward_bcurve, &eval_p1));  
        UF_CALL(UF_EVAL_initialize(reverse_bcurve, &eval_p2));  
        UF_CALL(UF_EVAL_is_equal(eval_p1, eval_p2, &is_equal));  
        UF_CALL(UF_EVAL_free(eval_p1));  
        UF_CALL(UF_EVAL_free(eval_p2));  
        UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));  
        UF_CALL(UF_UNDO_delete_mark(mark_id, NULL));  
        if (is_equal) return TRUE;  
        return FALSE;  
    }  
    static tag_t find_alive_curve(tag_t condemned_curve)  
    {  
        int  
            ii,  
            n_objects,  
            subtype,  
            type;  
        tag_t  
            alive_curve = NULL_TAG,  
            *objects,  
            part = UF_PART_ask_display_part();  
        double  
            tol;  
        UF_MODL_ask_distance_tolerance(&tol);  
        UF_CALL(UF_OBJ_ask_type_and_subtype(condemned_curve, &type, &subtype));  
        n_objects = ask_all_of_type(part, type, &objects);  
        for (ii = 0; ii < n_objects; ii++)  
        {  
            if (do_curves_match(condemned_curve, objects[ii], tol))  
            {  
                alive_curve = objects[ii];  
                break;  
            }  
        }  
        if (n_objects > 0) UF_free(objects);  
        return alive_curve;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_guide_curves,  
            n_profile_curves;  
        tag_t  
            alive_curve,  
            *guide_curves,  
            *profile_curves,  
            sweep_feat;  
        while ((sweep_feat = select_a_feature("Report Sweep Curves")) != NULL_TAG)  
        {  
            if (UF_CALL(UF_MODL_ask_sweep_curves(sweep_feat, &n_profile_curves,  
                &profile_curves, &n_guide_curves, &guide_curves))) continue;  
            WRITE_D(n_profile_curves);  
            for (ii = 0; ii < n_profile_curves; ii++)  
            {  
                RX(profile_curves[ii]);  
            /*  Begin work around for PR 5602790 里海译:开始着手处理 PR 5602790 的修复工作。 */  
                if (UF_OBJ_ask_status(profile_curves[ii]) == UF_OBJ_CONDEMNED)  
                {  
                    alive_curve = find_alive_curve(profile_curves[ii]);  
                    RX(alive_curve);  
                }  
            /*  End work around for PR 5602790 里海译:End work around for PR 5602790 */  
            }  
            if (n_profile_curves > 0) UF_free(profile_curves);  
            WRITE_D(n_guide_curves);  
            RXn(guide_curves, n_guide_curves);  
            if (n_guide_curves > 0) UF_free(guide_curves);  
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

> 这段代码是一个NX二次开发示例，主要实现了以下功能：
>
> 1. 初始化和终止：使用UF_initialize和UF_terminate来初始化和终止NX的API环境。
> 2. 错误报告：定义了ECHO、WRITE_D等宏，用于在日志窗口和系统日志中输出调试信息，并使用report_error函数来报告错误。
> 3. 询问部件名称：定义了ask_part_shortname函数，用于获取部件的简称。
> 4. 查询对象信息：定义了report_object_info函数，用于查询并输出一个对象的各种信息，包括类型、名称、所属部件等。
> 5. 查询特征信息：定义了report_feature_name_type_and_sysname函数，用于查询并输出一个特征的特征名称、系统名称和类型。
> 6. 选择特征：定义了select_a_feature函数，用于让用户选择一个特征。
> 7. 内存分配：定义了allocate_memory函数，用于在NX中分配内存。
> 8. 创建对象数组：定义了make_an_array函数，用于将一个对象列表转换为一个对象数组。
> 9. 查询所有指定类型对象：定义了ask_all_of_type函数，用于查询部件中所有指定类型的对象。
> 10. 检查曲线是否匹配：定义了do_curves_match函数，用于检查两条曲线是否完全相同或反转相同。
> 11. 查找活着的曲线：定义了find_alive_curve函数，用于在部件中查找与指定曲线完全相同或反转相同的曲线。
> 12. 查询扫掠特征信息：定义了do_it函数，用于查询当前显示部件中的扫掠特征，并输出其引导曲线和轮廓曲线信息。同时处理了PR 5602790的曲线被删除的问题。
> 13. 卸载请求：定义了ufusr_ask_unload函数，用于请求立即卸载该代码。
> 14. ufusr函数：这是NX二次开发的入口函数，调用了do_it函数来实现上述功能。
>
> 总体而言，这段代码通过NX的API实现了对扫掠特征曲线信息的查询和输出，同时也处理了一些已知问题，具有一定的参考价值。
>
