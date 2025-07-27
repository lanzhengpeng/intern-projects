### 【1518】list components visible in selected view 在选定的视图中列出可见的组件

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
    #include <uf_view.h>  
    #include <uf_part.h>  
    #include <uf_so.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_draw.h>  
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
            case UF_circle_type: /* non-std format in the header file 里海译:在头文件中使用非标准格式 */  
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
        strcpy(descriptor, "Unknown type");  /* In case its not found 里海译:如果找不到，请这样回答：如果找不到，请这样回答。 */  
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
    /* qq3123197280 */  
    static int mask_for_views(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[1] = { { UF_view_type, UF_all_subtype, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_member_view(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a view", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_views, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static tag_t ask_component_of_drafting_curve(tag_t curve)  
    {  
        int  
            gtype,  
            n_parents,  
            subtype;  
        tag_t  
            comp = NULL_TAG,  
            group,  
            *parents;  
        if (UF_ASSEM_is_occurrence(curve))  
            comp = UF_ASSEM_ask_part_occurrence(curve);  
        else  
        {  
            UF_CALL(UF_DRAW_ask_group_of_curve(curve, &group, &gtype, &subtype));  
            if ((group != NULL_TAG) && ((gtype == UF_solid_section_type) ||  
                    (gtype == UF_solid_silhouette_type) ||  
                    (gtype == UF_curve_group_type)))  
            {  
                UF_CALL(UF_SO_ask_parents(curve, UF_SO_ASK_ALL_PARENTS, &n_parents,  
                    &parents));  
                if (n_parents > 0)  
                {  
                    if (UF_ASSEM_is_occurrence(parents[0]))  
                        comp = UF_ASSEM_ask_part_occurrence(parents[0]);  
                    UF_free(parents);  
                }  
            }  
        }  
        return comp;  
    }  
    static tag_t ask_next_component(tag_t part, tag_t comp)  
    {  
        int  
            subtype,  
            type;  
        while  (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_component_type, &comp))  
            && (comp != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(comp, &type, &subtype));  
            if (subtype == UF_part_occurrence_subtype) return comp;  
        }  
        return comp;  
    }  
    static int ask_all_components(tag_t part, tag_t **components)  
    {  
        tag_t  
            component = NULL_TAG;  
        uf_list_p_t  
            comp_list;  
        UF_CALL(UF_MODL_create_list(&comp_list));  
        while ((component = ask_next_component(part, component)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(comp_list, component));  
        return make_an_array(&comp_list, components);  
    }  
    static tag_t map_across_parallel_arrays(tag_t map, tag_t *from, tag_t *to,  
        int n)  
    {  
        int ii;  
        for (ii = 0; ii < n; ii++) if (from[ii] == map) return to[ii];  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            flag = 0,  
            ii,  
            n_comps,  
            n_occs,  
            subtype,  
            type;  
        tag_t  
            comp,  
            *comps,  
            explosion,  
            object = NULL_TAG,  
            *occs,  
            *shadow_occs,  
            part = UF_PART_ask_display_part(),  
            view;  
        char  
            msg[MAX_LINE_SIZE+1];  
        uf_list_p_t  
            comp_list;  
    /*  set up in case we have to map back from explosion shadow occurrences 里海译:翻译如下：以防我们必须从爆炸阴影发生地回溯。 */  
        n_occs = ask_all_components(part, &occs);  
        UF_CALL(allocate_memory(n_occs * sizeof(tag_t), (void **)&shadow_occs));  
        while ((view = select_a_member_view("Report visible components")) != NULL_TAG)  
        {  
            RX(view);  
            UF_CALL(UF_MODL_create_list(&comp_list));  
        /*  If the view is exploded the components found will be shadow part  
            occurrences so set up to map back to the originals.  
        */  
            UF_CALL(UF_ASSEM_ask_view_explosion(view, &explosion));  
            RX(explosion);  
            if (explosion != NULL_TAG)  
            {  
                for (ii = 0; ii < n_occs; ii++)  
                {  
                    UF_CALL(UF_ASSEM_ask_exploded_object(explosion, occs[ii],  
                        &shadow_occs[ii]));  
                }  
            }  
        /*  This function returns components and solid bodies which are  
            inside the view bounds even though they cannot be seen -  
            see PR 5079430  
            The work around is to only process the visible wireframe objects and  
            work back to their parent components.  
        */  
            while (!UF_CALL(UF_VIEW_cycle_objects(view, UF_VIEW_VISIBLE_OBJECTS,  
                &object)) && (object != NULL_TAG))  
            {  
                comp = NULL_TAG;  
                UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
                switch (type)  
                {  
                    case UF_point_type:  
                    case UF_line_type:  
                    case UF_circle_type:  
                    case UF_conic_type:  
                    case UF_spline_type:  
                        comp = ask_component_of_drafting_curve(object);  
                        break;  
                    case UF_solid_type:  
                        if ((subtype == UF_solid_edge_subtype) &&  
                            UF_ASSEM_is_occurrence(object))  
                            comp = UF_ASSEM_ask_part_occurrence(object);  
                    default:  
                        break;  
                }  
            /*  if the view is exploded map from the returned shadow part  
                occurrence back to the original component */  
                if (comp != NULL_TAG)  
                {  
                    if (explosion != NULL_TAG) comp = map_across_parallel_arrays(  
                            comp, shadow_occs, occs, n_occs);  
                    UF_CALL(UF_MODL_put_list_item(comp_list, comp));  
                }  
            }  
            n_comps = make_an_array(&comp_list, &comps);  
            for (ii = 0; ii < n_comps; ii++)  
            {  
                RX(comps[ii]);  
                UF_CALL(UF_DISP_set_highlight(comps[ii], TRUE));  
            }  
            sprintf(msg, "%d components visible in view", n_comps);  
            uc1601(msg, TRUE);  
            UF_CALL(UF_PART_cleanup(UF_PART_cleanup_highlight));  
            if (n_comps > 0) UF_free(comps);  
        }  
        if (n_occs > 0)  
        {  
            UF_free(occs);  
            UF_free(shadow_occs);  
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

> 这段NX二次开发代码的主要功能是选择一个视图，并报告该视图可见的组件。以下是代码的主要流程：
>
> 1. 视图选择：首先，用户被提示选择一个视图，该视图会显示在NX中的当前工作部件。
> 2. 组件获取：然后，代码会获取该视图中的所有可见组件，并将它们高亮显示。为了实现这一点，代码首先遍历视图中的所有可见对象，然后根据对象类型获取其所属的组件。对于爆炸视图，需要将爆炸阴影组件映射回原始组件。
> 3. 组件报告：最后，代码报告了每个组件的信息，包括名称、类型、所属部件等，并将它们高亮显示在NX中。
> 4. 内存释放：在处理完所有组件后，代码会释放相关内存，并提示用户继续选择其他视图，或者结束程序。
>
> 这段代码的关键点包括视图遍历、组件映射、对象类型判断等。它利用了NX提供的对象选择、遍历、查询等二次开发接口，实现了选择视图并报告其中可见组件的功能。
>
