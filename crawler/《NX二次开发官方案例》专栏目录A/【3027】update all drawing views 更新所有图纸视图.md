### 【3027】update all drawing views 更新所有图纸视图

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_drf.h>  
    #include <uf_draw.h>  
    #include <uf_cfi.h>  
    #include <uf_text.h>  
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
    static void do_it(void)  
    {  
        int  
            ii,  
            n_drawings;  
        tag_t  
            *drawings;  
        char  
            dname[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_DRAW_ask_drawings(&n_drawings, &drawings));  
        if (n_drawings > 0)  
        {  
            for (ii = 0; ii < n_drawings; ii++)  
            {  
                UF_CALL(UF_OBJ_ask_name(drawings[ii], dname));  
                ECHO("\tUpdating views on %s\n", dname);  
                UF_CALL(UF_DRF_update_views(dname, UF_DRF_UPDATE_ALL, NULL));  
            }  
            UF_free(drawings);  
        }  
        else  
            ECHO("%s\n", "\tNo drawings found");  
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

> 这段代码是用于NX CAD系统的二次开发，主要实现了以下功能：
>
> 1. 初始化和结束：在主函数中，首先调用UF_initialize()来初始化NX环境，然后在结束时调用UF_terminate()来结束NX环境。
> 2. 处理零件文件：通过循环调用uc4621()函数，获取用户输入的零件文件名，然后打开零件文件，并处理更新图纸中的视图。如果处理成功，则保存零件文件的副本。
> 3. 更新图纸视图：在do_it()函数中，首先获取当前打开的所有图纸，然后遍历每个图纸，调用UF_DRF_update_views()函数更新图纸中的所有视图。
> 4. 错误处理：通过定义宏UF_CALL来简化错误处理，在调用NX API函数后检查返回码，如果有错误，则报告错误信息。
> 5. 加载状态报告：在report_load_status()函数中，报告零件加载的状态信息。
> 6. 构建副本文件名：在build_copy_partname()函数中，根据原始零件文件名构建新的副本文件名。
> 7. NX Manager模式支持：主函数支持外部NX Manager模式，可以通过命令行参数传入零件的编号和版本号。
> 8. 日志输出：通过ECHO()函数输出日志信息到控制台和NX日志窗口。
>
> 综上所述，这段代码实现了打开零件文件、更新图纸视图、保存副本等功能，并具有完善的错误处理和日志输出。
>
