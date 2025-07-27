### 【3086】create copy of assembly with component paths based on text file search directories 根据文本文件搜索路径

#### 代码

```cpp
    /*HEAD CREATE_COPY_OF_ASSEMBLY_WITH_COMPONENT_PATHS_BASED_ON_TEXT_FILE_SEARCH_DIRECTORIES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
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
    static int ask_data_from_text_file(char *fspec, char ***data)  
    {  
        int  
            ii = 0,  
            n_lines = 0;  
        char  
            a_line[MAX_LINE_SIZE+1];  
        FILE  
            *the_file;  
        if ((the_file = fopen(fspec, "r")) != NULL)  
        {  
            while ((fgets(a_line, MAX_LINE_SIZE, the_file)) != NULL) n_lines++;  
            rewind(the_file);  
            UF_CALL(allocate_memory(n_lines * sizeof(char *), (void **)data));  
            for (ii = 0; ii < n_lines; ii++)  
            {  
                fgets(a_line, MAX_LINE_SIZE, the_file);  
            /*  strip off the carriage return 译:去掉回车换行 */  
                if (ii < (n_lines - 1)) a_line[strlen(a_line) - 1] = '\0';  
                UF_CALL(allocate_memory((strlen(a_line) + 1) * sizeof(char),  
                    (void **)&(*data)[ii]));  
                strcpy((*data)[ii], a_line);  
            }  
            fclose(the_file);  
        }  
        else  
            ECHO("file cannot be read %s\n", fspec);  
        return n_lines;  
    }  
    static void set_search_directories_from_text_file(char *fspec, logical subs)  
    {  
        logical  
            *do_subs;  
        int  
            ii,  
            n_dirs;  
        char  
            **dirs;  
        UF_ASSEM_options_t  
            options;  
        UF_CALL(UF_ASSEM_ask_assem_options(&options));  
        options.load_options = UF_ASSEM_load_from_search_dirs;  
        UF_CALL(UF_ASSEM_set_assem_options(&options));  
        n_dirs = ask_data_from_text_file(fspec, &dirs);  
        if (n_dirs == 0) return;  
        UF_CALL(allocate_memory(n_dirs * sizeof(logical), (void **)&do_subs));  
        for (ii = 0; ii < n_dirs; ii++) do_subs[ii] = subs;  
        UF_CALL(UF_ASSEM_set_search_directories(n_dirs, dirs, do_subs));  
        UF_free_string_array(n_dirs, dirs);  
        UF_free(do_subs);  
    }  
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
    static void build_copy_partname(char *copy_fspec)  
    {  
        logical  
            tceng;  
        char  
            dspec[MAX_FSPEC_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_fspec[MAX_FSPEC_SIZE+1],  
            part_name[MAX_ENTITY_NAME_SIZE+1],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            *tmp_dir;  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(UF_is_ugmanager_active(&tceng));  
        if (!tceng)  
        {  
            UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
            UF_CALL(uc4578(part_name, 2, part_name));  
            strcat(part_name, "-new");  
            UF_CALL(uc4575(dspec, 2, part_name, copy_fspec));  
        }  
        else  
        {  
            UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
            UF_CALL(UF_UGMGR_decode_part_filename(part_fspec, part_number,  
                part_revision, part_file_type, part_file_name));  
            UF_CALL(uc4575(tmp_dir, 2, part_number, copy_fspec));  
        }  
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
    static logical add_proto_to_list(tag_t comp, void *user_data)  
    {  
        tag_t  
            proto = UF_ASSEM_ask_prototype_of_occ(comp);  
        uf_list_p_t  
            *list = (uf_list_p_t *)user_data;  
        UF_CALL(UF_MODL_put_list_item(*list, proto));  
        return TRUE;  
    }  
    static int ask_all_assembly_parts(tag_t part, tag_t **parts)  
    {  
        tag_t  
            cset;  
        uf_list_p_t  
            part_list;  
        UF_PART_load_status_t  
            status;  
        UF_CALL(UF_MODL_create_list(&part_list));  
        UF_CALL(UF_MODL_put_list_item(part_list, part));  
        UF_CALL(UF_ASSEM_ask_all_comp_cset(part, &cset));  
        if (cset != NULL_TAG)  
        {  
            UF_CALL(UF_PART_open_cset(cset, &status));  
            report_load_status(&status);  
            UF_CALL(UF_ASSEM_apply_to_cset(cset, add_proto_to_list, &part_list));  
        }  
        return make_an_array(&part_list, parts);  
    }  
    static void close_all_other_parts(tag_t dontClose)  
    {  
        int  
            ii,  
            n_parts;  
        tag_t  
            *parts;  
        uf_list_p_t  
            parts_list;  
        if ((n_parts = UF_PART_ask_num_parts()) == 0) return;  
        UF_CALL(UF_MODL_create_list(&parts_list));  
        for (ii = 0; ii < n_parts; ii++)  
            UF_CALL(UF_MODL_put_list_item(parts_list, UF_PART_ask_nth_part(ii)));  
        n_parts = ask_all_assembly_parts(dontClose, &parts);  
        for (ii = 0; ii < n_parts; ii++)  
            UF_CALL(UF_MODL_delete_list_item(&parts_list, parts[ii]));  
        if (n_parts > 0) UF_free(parts);  
        n_parts = make_an_array(&parts_list, &parts);  
        for (ii = 0; ii < n_parts; ii++)  
            UF_CALL(UF_PART_close(parts[ii], 0, 1));  
        if (n_parts > 0) UF_free(parts);  
    }  
    /*  To run in external native mode NX:  
            program.exe C:\path\to\dir_list.txt C:\path\to\part1.prt \path\to\part2.prt [...]  
        To run in external NX Manager mode:  
            program.exe -pim=yes C:\path\to\native\dir_list.txt @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            dir_list[MAX_FSPEC_SIZE+1],  
            msg[UF_UI_MAX_STRING_LEN],  
            new_fspec[MAX_FSPEC_SIZE+1],  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            if (uc4621(dir_list) == 1)  
                set_search_directories_from_text_file(dir_list, FALSE);  
            while (uc4621(part_name) == 1)  
            {  
                sprintf(msg, "%d.  %s\n", ++cnt, part_name);  
                ECHO(msg);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) report_load_status(&status);  
                if (!status.failed)  
                {  
                    build_copy_partname(new_fspec);  
                    close_all_other_parts(part);  
                    if (!UF_CALL(UF_PART_save_as(new_fspec)))  
                        printf("\tsaved as %s\n", new_fspec);  
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

> 这段NX二次开发代码的主要功能是：
>
> 1. 从文本文件中读取目录路径，并设置NX的搜索目录。
> 2. 逐个打开传入的部件文件，如果成功打开，则关闭其他所有部件，然后保存当前打开的部件到一个新文件，并关闭所有部件。
> 3. 重复以上过程，直到没有更多的部件文件传入。
> 4. 在控制台输出处理过的部件数量。
>
> 代码的主要流程如下：
>
> 1. 初始化NX环境。
> 2. 从命令行读取目录列表文件，设置NX的搜索目录。
> 3. 循环读取传入的部件文件名。
> 4. 打开每个部件文件，并报告打开状态。
> 5. 如果成功打开，构建新部件的文件名。
> 6. 关闭其他所有部件。
> 7. 保存当前部件到新文件。
> 8. 关闭所有部件。
> 9. 重复以上过程，直到没有更多部件文件传入。
> 10. 输出处理过的部件数量。
> 11. 终止NX环境。
>
> 代码中使用了NX的UF模块，如UF_PART、UF_ASSEM、UF_MODL等，用于打开、关闭、保存部件，查询装配结构，管理部件列表等。同时，使用了标准C库函数，如fopen、fgets、fclose等，用于文件操作。代码还定义了一些辅助函数，如report_error、allocate_memory、ask_data_from_text_file等，用于错误报告、内存分配和文本文件读取。
>
