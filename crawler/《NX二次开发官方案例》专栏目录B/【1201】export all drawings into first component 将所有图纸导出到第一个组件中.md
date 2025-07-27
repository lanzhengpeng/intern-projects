### 【1201】export all drawings into first component 将所有图纸导出到第一个组件中

#### 代码

```cpp
    //  This demonstrates breaking the master model paradigm and putting the  
    //  drawings from the drawing assembly part back into the model part.  
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
    static void ask_component_partname(tag_t component, char *partname)  
    {  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
    /* This returns the component part's fullpath based on the current load  
        options whether it is loaded or not.  
    */  
        UF_CALL(UF_ASSEM_ask_component_data(component, partname, refset_name,  
            instance_name, origin, csys_matrix, transform));  
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
    static void do_it(void)  
    {  
        int  
            ii,  
            n_drawings,  
            n_curves,  
            n_objects;  
        tag_t  
            comp = NULL_TAG,  
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
        comp = ask_next_component(part, NULL_TAG);  
        if (comp == NULL_TAG) return;  
        ask_component_partname(comp, part_fspec);  
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

> 这段代码是一个NX的二次开发示例，主要实现的功能是将NX装配中的绘制元素导出到一个单独的模型文件中。
>
> 主要功能包括：
>
> 1. 遍历装配中的所有组件，找到第一个零件组件。
> 2. 获取该零件组件的完整路径。
> 3. 查询当前会话中所有的绘制元素，并将它们添加到一个列表中。
> 4. 查询该零件组件中所有的模型曲线元素，并将它们也添加到列表中。
> 5. 将列表中的所有元素导出到指定的模型文件中。
>
> 代码中定义了多个辅助函数，例如遍历组件、查询元素、创建列表等。主函数do_it()实现了上述功能。另外，提供了两种运行方式：作为NX内置应用运行，或者作为外部应用接受参数运行。
>
> 代码通过标准的NX API函数调用实现，注释详细，结构清晰，是一个典型的NX二次开发示例。
>
