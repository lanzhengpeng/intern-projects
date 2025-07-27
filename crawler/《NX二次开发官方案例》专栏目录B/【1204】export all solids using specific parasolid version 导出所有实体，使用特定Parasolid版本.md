### 【1204】export all solids using specific parasolid version 导出所有实体，使用特定Parasolid版本

#### 代码

```cpp
    /*  
    This example exports the solid occurrences at the displayed part assembly  
    level.  This way the exported data is in the assembly location.  So for the  
    UGII_BASE_DIR\UGOPEN\SampleNXOpenApplications\.NET\AssemblyViewer\toycar_assy.prt  
    it exports 8 bodies (5 wheels, 2 axles, 1 body) to the single parasolid file.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_ps.h>  
    #include <uf_cfi.h>  
    #include <uf_ugmgr.h>  
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
        while (! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype) return body;  
        }  
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
    static int ask_all_solids(tag_t part, tag_t **solids)  
    {  
        tag_t  
            body = NULL_TAG;  
        uf_list_p_t  
            body_list;  
        UF_CALL(UF_MODL_create_list(&body_list));  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(body_list, body));  
        return (make_an_array(&body_list, solids));  
    }  
    static void build_similar_temp_filespec(char *fspec, int ftype, char *new_spec)  
    {  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            temp_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            *tmp_dir;  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4576(fspec, 0, dspec, temp_name));  
        if (strrchr(temp_name, '.')) strcpy(strrchr(temp_name, '.'), "");  
        UF_CALL(uc4575(tmp_dir, ftype, temp_name, new_spec));  
    }  
    static void build_similar_filespec(char *fspec, int ftype)  
    {  
        logical  
            is_ugmgr;  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1];  
        UF_CALL(UF_is_ugmanager_active(&is_ugmgr));  
        UF_PART_ask_part_name(part, part_fspec);  
        if (!is_ugmgr)  
        {  
            UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
            UF_CALL(uc4578(part_name, 2, part_name));  
            UF_CALL(uc4575(dspec, ftype, part_name, fspec));  
        }  
        else  
        {  
            UF_CALL(UF_UGMGR_decode_part_filename(part_fspec, part_number,  
                part_revision, part_file_type, part_file_name));  
            build_similar_temp_filespec(part_number, ftype, fspec);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n,  
            n_uns;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            *solids;  
        char  
            msg[133],  
            name[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_PS_unexported_t  
            *uns;  
        n = ask_all_solids(part, &solids);  
        if (n > 0)  
        {  
            build_similar_filespec(name, 0);  
            strcat(name, ".x_t");  
            uc4561(name, 0);  
            ECHO("Exporting %d solids to %s\n", n, name);  
                                                                /* 10.0 里海译:对于这个请求，我无法提供翻译服务，因为我的训练数据中包含了中文和英文的对话。我建议您提供需要翻译的文本，我会尽量提供准确的翻译。 */  
            if (!UF_CALL(UF_PS_export_linked_data(solids, n, name, 100,  
                NULL, &n_uns, &uns)))  
            {  
                for (ii = 0; ii < n_uns; ii++)  
                {  
                    UF_get_fail_message(uns[ii].fail_code, msg);  
                    ECHO("    failed tag %d - %s\n", uns[ii].ug_body_tag, msg);  
                }  
                if (n_uns > 0) UF_free(uns);  
            }  
            UF_free(solids);  
        }  
        else  
            ECHO("No solids to export\n");  
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
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                ECHO("%d.  %s\n", ++cnt, part_name);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) report_load_status(&status);  
                if (!status.failed)  
                {  
                    do_it();  
                    UF_PART_close_all();  
                }  
            }  
            ECHO("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++应用程序，用于导出当前显示的部件装配中的实体。主要功能包括：
>
> 1. 获取当前显示的部件：使用UF_PART_ask_display_part()函数获取当前显示的部件。
> 2. 遍历部件中的所有实体：使用UF_OBJ_cycle_objs_in_part()和UF_OBJ_ask_type_and_subtype()函数遍历部件中的所有实体，并判断是否为实体类型。
> 3. 创建实体列表：将遍历得到的实体添加到列表中。
> 4. 导出实体到 Parasolid 文件：使用UF_PS_export_linked_data()函数将实体列表中的实体导出到 Parasolid 文件。
> 5. 处理未导出的实体：处理导出过程中未成功导出的实体，并打印错误信息。
> 6. 释放内存：释放分配的内存。
> 7. 处理多个部件：处理命令行传入的多个部件文件路径，并打开每个部件文件进行实体导出。
> 8. 打印处理信息：打印处理了多少个部件，以及每个部件的加载状态。
>
> 总的来说，这个程序提供了导出当前显示的部件装配中所有实体的功能，并且可以处理多个部件文件。
>
