### 【1200】export all drawings into another loaded part 将所有图纸导出到另一个已加载的部件

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_cfi.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_view.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf(msg, UF_UI_MAX_STRING_LEN, format, args);  
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
    static int ask_all_model_curves(tag_t part, tag_t **curves)  
    {  
        int  
            flag,  
            ii, jj,  
            n,  
            type[5] = { UF_point_type,  
                        UF_line_type,  
                        UF_circle_type,  
                        UF_conic_type,  
                        UF_spline_type };  
        tag_t  
            *objects;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        for (ii = 0; ii < 5; ii++)  
        {  
            n = ask_all_of_type(part, type[ii], &objects);  
            for (jj = 0; jj < n; jj++)  
            {  
                UF_CALL(uc6409(objects[jj], &flag, view_name));  
                if (!flag)  
                    UF_CALL(UF_MODL_put_list_item(object_list, objects[jj]));  
            }  
            if (n > 0) UF_free(objects);  
        }  
        return make_an_array(&object_list, curves);  
    }  
    static int ask_all_other_parts(tag_t except_part, tag_t **parts)  
    {  
        int  
            err,  
            ii, jj,  
            n = UF_PART_ask_num_parts();  
        tag_t  
            a_part;  
        if (n > 1)  
        {  
            *parts = UF_allocate_memory((n-1) * sizeof(tag_t), &err);  
            if (UF_CALL(err)) return 0;  
            for (ii = 0, jj = 0; ii < n; ii++)  
            {  
                a_part = UF_PART_ask_nth_part(ii);  
                if (a_part != except_part) (*parts)[jj++] = a_part;  
            }  
        }  
        return n  - 1;  
    }  
    static int allocate_string_array(int n_elements, int n_chars, char ***where)  
    {  
        int  
            ii,  
            resp;  
        *where = UF_allocate_memory(n_elements * sizeof(char *), &resp);  
        for (ii = 0; ii < n_elements; ii++)  
        {  
            (*where)[ii] = UF_allocate_memory(n_chars * sizeof(char), &resp);  
            if (resp) return resp;  
        }  
        return 0;  
    }  
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
    static int choose_one_item(char *prompt, char **choices, int n_choices)  
    {  
        int  
            a = 0,  
            ii,  
            resp,  
            z;  
        char  
            options[14][38];  
        do  
        {  
            z = (n_choices - a < 14) ? n_choices - a : 14;  
            for (ii = 0; ii < z; ii++)  
            {  
                strncpy(&options[ii][0], choices[a+ii], 37);  
                options[ii][37] = '\0';  
            }  
            if ((z == 14) && ((a + z) < n_choices))  
                strcpy(&options[13][0], "More...");  
            switch (resp = uc1603(prompt, 0, options, z))  
            {  
                case 1:                     /* Back 里海译:Back */  
                    if (a == 0) return -1;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 里海译:取消 */  
                    return -1;  
                    break;  
                case 18:                    /* More... 里海译:翻译：更多... */  
                    if ((a + z) < n_choices)  
                    {  
                        a = a + 13;  
                        break;  
                    }  
                default:                    /* Picked one 里海译:Picked one */  
                    return a + resp - 5;  
                    break;  
            }  
        } while (TRUE);  
    }  
    static tag_t choose_another_loaded_part(char *prompt)  
    {  
        int  
            ii,  
            n_parts,  
            which;  
        tag_t  
            the_part,  
            this_part = UF_PART_ask_display_part(),  
            *parts;  
        char  
            **leaf_names;  
        n_parts = ask_all_other_parts(this_part, &parts);  
        if (n_parts == 0) return NULL_TAG;  
        if (n_parts == 1)  
        {  
            the_part = parts[0];  
            UF_free(parts);  
            return the_part;  
        }  
        UF_CALL(allocate_string_array(n_parts, UF_CFI_MAX_FILE_NAME_LEN,  
            &leaf_names));  
        for (ii = 0; ii < n_parts; ii++)  
            ask_part_shortname(parts[ii], leaf_names[ii]);  
        if ((which = choose_one_item(prompt, leaf_names, n_parts)) == -1)  
            the_part = NULL_TAG;  
        else  
            the_part = parts[which];  
        UF_free(parts);  
        UF_free_string_array(n_parts, leaf_names);  
        return the_part;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_drawings,  
            n_curves,  
            n_objects;  
        tag_t  
            which_part = NULL_TAG,  
            *drawings,  
            *curves,  
            *objects,  
            part = UF_PART_ask_display_part();  
        uf_list_p_t  
            object_list;  
        char  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_PART_export_options_t  
            options = { FALSE, UF_PART_maintain_all_params, UF_PART_copy_exp_deeply };  
        which_part = choose_another_loaded_part("Export drawings to");  
        if (which_part == NULL_TAG) return;  
        UF_CALL(UF_PART_ask_part_name(which_part, part_fspec));  
        UF_CALL(UF_MODL_create_list(&object_list));  
        UF_CALL(UF_DRAW_ask_drawings(&n_drawings, &drawings));  
        if (n_drawings == 0) return;  
        for (ii = 0; ii < n_drawings; ii++)  
            UF_CALL(UF_MODL_put_list_item(object_list, drawings[ii]));  
        UF_free(drawings);  
    //  Just in case any dimensions are associated to model mode curve objects in  
    //  this part.  See PR 6723668  
        n_curves = ask_all_model_curves(part, &curves);  
        for (ii = 0; ii < n_curves; ii++)  
            UF_CALL(UF_MODL_put_list_item(object_list, curves[ii]));  
        if (n_curves > 0) UF_free(curves);  
        n_objects = make_an_array(&object_list, &objects);  
        if (n_objects > 0)  
        {  
            ECHO("Exporting %d objects to %s\n", n_objects, part_fspec);  
            UF_CALL(UF_PART_export_with_options(part_fspec, n_objects,  
                objects, &options));  
            UF_free(objects);  
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

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用出错时输出错误信息。
> 2. 定义了内存分配函数allocate_memory，用于在NX中分配内存。
> 3. 定义了创建实体数组函数make_an_array，用于将实体列表转换为实体数组。
> 4. 定义了查询某类型所有实体函数ask_all_of_type，用于查询当前部件中所有指定类型的实体。
> 5. 定义了查询所有模型曲线函数ask_all_model_curves，用于查询当前部件中所有模型曲线实体。
> 6. 定义了查询除当前部件外的其他部件函数ask_all_other_parts，用于查询当前打开的NX部件中除当前部件外的其他部件。
> 7. 定义了字符串数组分配函数allocate_string_array，用于在NX中分配字符串数组。
> 8. 定义了获取部件简称函数ask_part_shortname，用于获取NX部件的简称。
> 9. 定义了选择列表项函数choose_one_item，用于在NX中选择一个列表项。
> 10. 定义了选择其他已加载部件函数choose_another_loaded_part，用于在NX中选择一个已加载的其他部件。
> 11. 定义了执行导出函数do_it，用于导出当前部件中的所有图纸到指定部件。
> 12. 定义了主函数ufusr，用于初始化NX环境并调用执行导出函数。
> 13. 定义了卸载函数ufusr_ask_unload，用于卸载当前NX应用程序。
>
> 总的来说，这段代码实现了从当前打开的NX部件中选择一个其他已加载的部件，并将当前部件中的所有图纸和模型曲线导出到该部件的功能。
>
