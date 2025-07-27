### 【1213】export drawings from one part to another in batch mode 批量将一个部件的图纸导出到另一个部件

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
    static void do_it(char *part_fspec)  
    {  
        int  
            ii,  
            n_drawings,  
            n_curves,  
            n_objects;  
        tag_t  
            *drawings,  
            *curves,  
            *objects,  
            part = UF_PART_ask_display_part();  
        uf_list_p_t  
            object_list;  
        UF_PART_export_options_t  
            options = { FALSE, UF_PART_maintain_all_params, UF_PART_copy_exp_deeply };  
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
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_text.h>  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        int  
            ii;  
        char  
            msg[133];  
        if (status->n_parts > 0)  
        {  
            ECHO("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_get_fail_message(status->statuses[ii], msg);  
                ECHO("  %s - %s", status->file_names[ii], msg);  
            }  
            ECHO("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    /*  To run in external native mode NX:  
            program.exe /path/to/source.prt /path/to/target.prt  
        To run in external NX Manager mode:  
            program.exe -pim=yes @DB/sourcepartno/rev @DB/targetpartno/rev  
    */  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            source_name[MAX_FSPEC_SIZE+1],  
            target_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            UF_CALL(UF_TEXT_init_native_lang_support());  
            if ((uc4621(source_name) == 1) && (uc4621(target_name) == 1))  
            {  
                UF_CALL(UF_PART_open(source_name, &part, &status));  
                if (status.n_parts > 0) report_load_status(&status);  
                if (!status.failed)  
                {  
                    ECHO("copying drawings from %s to %s\n", source_name, target_name);  
                    do_it(target_name);  
                /*  
                    build_copy_partname(new_fspec);  
                    if (!UF_CALL(UF_PART_save_as(new_fspec)))  
                        ECHO("\tsaved as %s\n", new_fspec);  
                */  
                    UF_CALL(UF_PART_close_all());  
                }  
            }  
            ECHO("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段NX Open C++代码主要用于实现以下功能：
>
> 1. 打开源零件文件，并获取其tag。
> 2. 获取源零件中的所有图和模型曲线对象。
> 3. 将图和模型曲线对象导出为目标零件文件。
> 4. 关闭所有打开的零件文件。
>
> 关键函数和流程包括：
>
> 总的来说，这段代码通过NX Open C++ API实现了零件文件的打开、对象查询和导出等操作，具有一定的参考价值。
>
