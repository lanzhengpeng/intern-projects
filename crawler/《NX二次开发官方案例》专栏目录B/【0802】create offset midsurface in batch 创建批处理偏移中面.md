### 【0802】create offset midsurface in batch 创建批处理偏移中面

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_sf.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
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
    static tag_t ask_first_object_of_name_type_and_subtype(char *name,  
        int type, int subtype)  
    {  
        tag_t  
            object = NULL_TAG,  
            part = UF_ASSEM_ask_work_part();  
        int  
            this_type,  
            this_subtype;  
        while (!UF_CALL(UF_OBJ_cycle_by_name_and_type(part, name, type, TRUE,  
            &object)) && (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &this_type, &this_subtype));  
            if (this_subtype == subtype) return object;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat,  
            seed = ask_first_object_of_name_type_and_subtype("SEED",  
                UF_solid_type, UF_solid_face_subtype);  
        double  
            cliff_angle = 5,  // best practice = 5, max suggested = 15  
            percentage_distance = 50;  
        if (seed == NULL_TAG)  
        {  
            ECHO("%s\n", "\tSEED face not found in part - skipping");  
        }  
        UF_CALL(UF_SF_create_offset_midsrf(seed, cliff_angle, percentage_distance,  
            &feat));  
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
    /*  To run in external native mode NX:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external NX Manager mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            new_fspec[MAX_FSPEC_SIZE+1],  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            UF_CALL(UF_TEXT_init_native_lang_support());  
            while (uc4621(part_name) == 1)  
            {  
                ECHO("%d.  %s\n", ++cnt, part_name);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) report_load_status(&status);  
                if (!status.failed)  
                {  
                    do_it();  
                    build_copy_partname(new_fspec);  
                    uc4561(new_fspec, 0);  
                    if (!UF_CALL(UF_PART_save_as(new_fspec)))  
                        ECHO("\tsaved as %s\n", new_fspec);  
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

> 这段代码是一个NX Open C++应用程序，其主要功能是读取NX部件文件，对部件进行操作，然后保存为新的部件文件。
>
> 代码的主要部分包括：
>
> 1. UF_initialize()和UF_terminate()：用于初始化和关闭NX Open API。
> 2. do_it()：包含对部件的主要操作逻辑，包括：查找名为"SEED"的面使用该面创建偏移曲面
> 3. 查找名为"SEED"的面
> 4. 使用该面创建偏移曲面
> 5. ufusr()：作为NX Open的回调函数，用于执行主要的部件操作。
> 6. main()：程序的入口函数，负责处理命令行参数，打开部件文件，执行操作，保存新文件。
> 7. build_copy_partname()：根据原部件文件名生成新的部件文件名。
> 8. report_load_status()：用于报告部件加载状态。
> 9. 使用了NX Open中的UF_PART、UF_OBJ、UF_SF等模块，进行了部件操作、对象查询、曲面操作等。
> 10. 通过命令行参数传入要处理的部件文件路径，支持多个文件。
> 11. 使用NX Open的文件I/O接口打开、保存部件文件。
> 12. 输出操作信息到列表窗口和日志文件。
> 13. 使用了NX Open的UF_terminate()确保资源的正确释放。
>
> 总的来说，这段代码通过NX Open API实现了对NX部件文件的读取、修改、保存，展示了NX二次开发的基本流程。
>
