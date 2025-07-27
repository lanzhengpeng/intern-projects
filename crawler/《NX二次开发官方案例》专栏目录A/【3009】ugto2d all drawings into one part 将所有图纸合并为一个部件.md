### 【3009】ugto2d all drawings into one part 将所有图纸合并为一个部件

#### 代码

```cpp
    /*HEAD UGTO2D_ALL_DRAWINGS_INTO_ONE_PART CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #include <uf_view.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_cfi.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的内容，可以总结如下：

- UF_print_syslog是V18版本中新增的函数。
- 该函数用于打印系统日志信息。
- 翻译时只需回答UF_print_syslog是V18版本新增的函数，用于打印系统日志信息即可，不需要添加其他内容。 */  
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
    static void build_unique_temp_part_name(char *fspec)  
    {  
        char  
            *tmp_dir,  
            unique[UF_CFI_MAX_FILE_NAME_SIZE];  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4577(unique));  
        UF_CALL(uc4575(tmp_dir, 2, unique, fspec));  
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
    static int ask_drawing_objects(tag_t **objects)  
    {  
        int  
            ii, jj,  
            n_drawings,  
            n_views;  
        tag_t  
            *drawings,  
            drawing_view,  
            object = NULL_TAG,  
            *views;  
        char  
            dname[MAX_ENTITY_NAME_SIZE+3];  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        UF_CALL(UF_DRAW_ask_drawings(&n_drawings, &drawings));  
        for (ii = 0; ii < n_drawings; ii++)  
        {  
            UF_CALL(UF_MODL_put_list_item(object_list, drawings[ii]));  
            UF_CALL(UF_OBJ_ask_name(drawings[ii], dname));  
            strcat(dname, "@0");  
            UF_CALL(UF_VIEW_ask_tag_of_view_name(dname, &drawing_view));  
            while (!UF_CALL(UF_VIEW_cycle_objects(drawing_view,  
                UF_VIEW_DEPENDENT_OBJECTS, &object)) && (object != NULL_TAG))  
                UF_CALL(UF_MODL_put_list_item(object_list, object));  
            UF_CALL(UF_DRAW_ask_views(drawings[ii], &n_views, &views));  
            for (jj = 0; jj < n_views; jj++)  
            {  
                UF_CALL(UF_MODL_put_list_item(object_list, views[jj]));  
                while (!UF_CALL(UF_VIEW_cycle_objects(views[jj],  
                    UF_VIEW_DEPENDENT_OBJECTS, &object)) && (object != NULL_TAG))  
                    UF_CALL(UF_MODL_put_list_item(object_list, object));  
            }  
            if (n_views > 0) UF_free(views);  
        }  
        if (n_drawings > 0) UF_free(drawings);  
        return make_an_array(&object_list, objects);  
    }  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(status->file_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_drawings,  
            n_objects;  
        tag_t  
            *drawings,  
            new_part,  
            *objects,  
            part = UF_PART_ask_display_part();  
        char  
            cmd[UF_UI_MAX_STRING_LEN+1],  
            draw_name[MAX_ENTITY_NAME_SIZE+1],  
            export_fspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            import_fspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_dir[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            *prog_name;  
        UF_PART_export_options_t  
            options = { FALSE, UF_PART_remove_params, UF_PART_copy_exp_shallowly };  
        UF_PART_load_status_t  
            status;  
    #if defined(_WIN32)  
        prog_name = "%UGII_BASE_DIR%\\UGTO2D\\ugto2d";  
    #else  
        prog_name = "$UGII_BASE_DIR/ugto2d/ugto2d";  
    #endif  
        UF_CALL(UF_DRAW_ask_drawings(&n_drawings, &drawings));  
        if (n_drawings == 0) return;  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(uc4576(part_fspec, 2, part_dir, part_name));  
        UF_CALL(uc4578(part_name, 2, part_name));  
        sprintf(import_fspec , "%s_2d", part_name);  
        UF_CALL(uc4575(part_dir, 2, import_fspec, import_fspec));  
        for (ii = 0; ii < n_drawings; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_name(drawings[ii], draw_name));  
            if (ii == 0)  
            {  
                sprintf(cmd,  "%s i=%s o=%s m=Drafting draw=%s", prog_name,  
                    part_fspec, import_fspec, draw_name);  
                system(cmd);  
                continue;  
            }  
            build_unique_temp_part_name(export_fspec);  
            sprintf(cmd,  "%s i=%s o=%s m=Drafting d=%s", prog_name,  
                part_fspec, export_fspec, draw_name);  
            system(cmd);  
            if (UF_CALL(UF_PART_open(export_fspec, &new_part, &status)))  
            {  
                if (status.n_parts > 0) report_load_status(&status);  
                continue;  
            }  
            if ((n_objects = ask_drawing_objects(&objects)) > 0)  
            {  
                UF_CALL(UF_PART_export_with_options(import_fspec, n_objects,  
                    objects, &options));  
                UF_free(objects);  
            }  
            UF_CALL(UF_PART_close(new_part, 1, 1));  
            UF_CALL(uc4561(export_fspec, 2));  
        }  
        UF_free(drawings);  
        UF_CALL(UF_PART_set_display_part(part));  
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
    static void check_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        for (ii=0; ii<status->n_parts; ii++)  
        {  
            UF_get_fail_message(status->statuses[ii], msg);  
            printf("    %s - %s\n", status->file_names[ii], msg);  
        }  
        UF_free(status->statuses);  
        UF_free_string_array(status->n_parts, status->file_names);  
    }  
    /*  To run in external native mode UG:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external UG/Manager or IMAN mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            part_name[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                printf("%d.  %s\n", ++cnt, part_name);  
                if (UF_CALL(UF_PART_open(part_name, &part, &status))  
                    || status.n_parts > 0) check_load_status(&status);  
                else  
                {  
                    do_it();  
                    UF_PART_close_all();  
                }  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++的二次开发程序，主要功能是将NX中的所有图纸导出到一个部件中。以下是代码的主要功能和流程：
>
> 1. 定义了一个错误报告函数report_error，用于打印UF函数调用失败的错误信息。
> 2. 定义了一个生成临时部件名称的函数build_unique_temp_part_name。
> 3. 定义了一个内存分配函数allocate_memory。
> 4. 定义了一个从列表中获取对象标签的函数make_an_array。
> 5. 定义了一个获取图纸中的所有对象标签的函数ask_drawing_objects。
> 6. 定义了一个报告加载状态的函数report_load_status。
> 7. 定义了一个主函数do_it，用于执行以下流程：
>
> 1. 定义了ufusr函数作为程序的入口点，用于初始化和执行do_it函数。
> 2. 定义了ufusr_ask_unload函数，用于卸载动态库时立即释放内存。
> 3. 定义了主函数main，用于在NX外部模式下运行，可以传入多个部件路径，依次打开并处理每个部件。
>
> 总的来说，这段代码实现了将NX中的所有图纸内容合并到一个新部件中的功能，主要通过调用NX的UF函数实现。
>
