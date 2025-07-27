### 【1199】export all bodies 导出所有实体

#### 代码

```cpp
    /*  
    This example will export all of the solids in the current displayed part.    
    If that part is an assembly this will include any bodies in the component parts that are in the reference sets being displayed in the assembly.    
    For example if you run it on the toycar_assy.prt, it will export 8 bodies (5 wheels, 2 axles, and the chassis)   
    in their correct assembly locations without any parameters or assembly structure.  
    */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_cfi.h>  
    #include <uf_assem.h>  
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
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        do  
        {  
            UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body));  
            if (body)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
                if (!subtype) return body;  
            }  
        } while (body);  
        return NULL_TAG;  
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
    static void build_temp_partname(char *copy_fspec)  
    {  
        logical  
            tceng;  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            *tmp_dir;  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(UF_is_ugmanager_active(&tceng));  
        if (tceng)  
        {  
            UF_CALL(UF_UGMGR_decode_part_filename(part_fspec, part_name, dspec,  
                dspec, dspec));  
        }  
        else  
        {  
            UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
            UF_CALL(uc4578(part_name, 2, part_name));  
        }  
        strcat(part_name, "-tmp");  
        UF_CALL(uc4575(tmp_dir, 2, part_name, copy_fspec));  
    }  
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
                ECHO("  %s - %s\n", status->file_names[ii], msg);  
            }  
            ECHO("\n");  
            UF_CALL(UF_PART_free_load_status(status));  
        }  
    }  
    static void ensure_fully_loaded(tag_t item)  
    {  
        tag_t  
            part;  
        char  
            partname[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_PART_load_status_t  
            error_status;  
        if (UF_ASSEM_is_occurrence(item))  
            item = UF_ASSEM_ask_prototype_of_occ(item);  
        UF_CALL(UF_OBJ_ask_owning_part(item, &part));  
        UF_PART_ask_part_name(part, partname);  
        if ((UF_PART_is_loaded(partname)) != 1)  
        {  
            UF_CALL(UF_PART_open_quiet(partname, &part, &error_status));  
            if (error_status.n_parts > 0) report_load_status(&error_status);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            body = NULL_TAG,  
            *bodies = NULL,  
            part = UF_PART_ask_display_part();  
        uf_list_p_t  
            body_list;  
        char  
            fspec[MAX_FSPEC_SIZE+1];  
        UF_PART_export_options_t  
            options = { TRUE, UF_PART_remove_params, UF_PART_copy_exp_shallowly };  
        UF_CALL(UF_MODL_create_list(&body_list));  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
        {  
            ensure_fully_loaded(body);  
            UF_CALL(UF_MODL_put_list_item(body_list, body));  
        }  
        n = make_an_array(&body_list, &bodies);  
        build_temp_partname(fspec);  
        uc4561(fspec, 2);  
        UF_CALL(UF_PART_export_with_options(fspec, n, bodies, &options));  
        UF_free(bodies);  
        ECHO("%d bodies exported to %s\n", n, fspec);  
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

> 这段代码是一个用于NX的二次开发示例，其主要功能是导出当前显示的部件中的所有实体。以下是代码的主要功能和流程：
>
> 1. 头文件包含：代码包含了NX二次开发所需的各种头文件，包括UF模块的头文件。
> 2. 错误处理：定义了ECHO函数用于输出错误信息，以及UF_CALL宏用于处理UF函数调用可能出现的错误。
> 3. 内存管理：定义了allocate_memory函数用于申请内存，以及make_an_array函数用于将对象列表转换为对象数组。
> 4. 部件处理：定义了ask_next_body函数用于遍历部件中的下一个实体，以及build_temp_partname函数用于构建临时部件文件名。
> 5. 加载状态处理：定义了report_load_status函数用于报告加载状态，以及ensure_fully_loaded函数用于确保部件完全加载。
> 6. 导出功能：定义了do_it函数用于导出当前显示部件中的所有实体，构建临时文件名，并调用UF_PART_export_with_options函数进行导出。
> 7. UFUSR函数：定义了ufusr函数作为NX二次开发程序的入口函数，用于初始化、执行导出操作，以及终止NX。
> 8. 卸载处理：定义了ufusr_ask_unload函数用于立即卸载二次开发程序。
>
> 整体来看，这段代码实现了在NX中导出当前显示部件中所有实体的功能，并提供了错误处理和内存管理等功能。
>
