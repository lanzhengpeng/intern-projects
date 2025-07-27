### 【2520】save all open parts to temp directory 将所有打开的部件保存到临时目录

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_cfi.h>  
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
    static void ensure_part_fully_loaded(tag_t part)  
    {  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            error_status;  
        if (!UF_CALL(UF_PART_ask_part_name(part, partname))  
            && ((UF_PART_is_loaded(partname)) != 1))  
        {  
            UF_CALL(UF_PART_open_quiet(partname, &part, &error_status));  
            if (error_status.n_parts > 0) report_load_status(&error_status);  
        }  
    }  
    /* This magically works with both native and NXManager mode encoded names! 里海译:神奇地适用于原生模式和NXManager模式编码的名字！ */  
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
    static void save_all(void)  
    {  
        int  
            count,  
            *error_list,  
            ii;  
        tag_t  
            *parts_list;  
        char  
            failure[133],  
            partname[UF_CFI_MAX_FILE_NAME_SIZE];  
        UF_CALL(UF_PART_save_all(&count, &parts_list, &error_list));  
        if (count > 0)  
        {  
            ECHO("%s\n", "Save notes:");  
            for (ii = 0; ii < count; ii++)  
            {  
                UF_PART_ask_part_name(parts_list[ii], partname);  
                UF_get_fail_message(error_list[ii], failure);  
                ECHO("  %s - %s\n", partname, failure);  
            }  
            UF_free(parts_list);  
            UF_free(error_list);  
        }  
    }  
    static void do_it(void)  
    {  
        logical  
            tcin;  
        int  
            ii,  
            n_parts = UF_PART_ask_num_parts();  
        tag_t  
            part;  
        char  
            part_spec[UF_CFI_MAX_PATH_NAME_SIZE],  
            temp_spec[UF_CFI_MAX_PATH_NAME_SIZE];  
        for (ii = 0; ii < n_parts; ii++)  
        {  
            part = UF_PART_ask_nth_part(ii);  
            UF_PART_ask_part_name(part, part_spec);  
            build_similar_temp_filespec(part_spec, 2, temp_spec);  
            ensure_part_fully_loaded(part);  
            UF_CALL(UF_PART_rename(part, temp_spec));  
        }  
        save_all();  
    //  Do not leave any now native mode parts open in the TCIN mode session!  
        UF_CALL(UF_is_ugmanager_active(&tcin));  
        if (tcin) UF_CALL(UF_PART_close_all());  
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

> 根据代码内容，这是一个用于NX的二次开发代码示例，其主要功能如下：
>
> 1. 初始化和终止：代码首先进行NX的初始化，并在结束时进行终止处理。
> 2. 错误处理：定义了一个错误报告函数report_error，用于在出错时输出错误信息。
> 3. 加载状态报告：定义了一个函数report_load_status，用于输出零件加载状态信息。
> 4. 确保零件完全加载：定义了一个函数ensure_part_fully_loaded，用于确保零件被完全加载。
> 5. 临时文件名生成：定义了一个函数build_similar_temp_filespec，用于生成临时文件名。
> 6. 保存所有零件：定义了一个函数save_all，用于保存所有打开的零件。
> 7. 主处理函数：定义了一个函数do_it，用于重命名所有打开的零件并保存。
> 8. 程序入口：定义了一个函数ufusr作为程序的入口，进行初始化、主处理并终止。
> 9. 卸载处理：定义了一个函数ufusr_ask_unload，用于在程序卸载时进行清理。
> 10. 命令行参数处理：在main函数中处理命令行参数，可以打开多个零件文件进行处理。
> 11. 日志输出：使用ECHO宏进行日志输出。
>
> 总体来说，这是一个用于NX的二次开发代码示例，实现了零件的重命名和保存等功能，具有完善的错误处理和日志输出机制。
>
