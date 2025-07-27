### 【0950】create tiff file of all drawings 创建所有图纸的tif文件

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
    #include <uf_plot.h>  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
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
    static void wait_for_file_to_exist(char *fspec)  
    {  
        int  
            not_there;  
        do  
        {  
            if (UF_CALL(UF_CFI_ask_file_exist(fspec, &not_there))) return;  
        } while (not_there);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_drawings;  
        tag_t  
            *drawings;  
        char  
            cgm_spec[UF_CFI_MAX_PATH_NAME_SIZE],  
            cmd[UF_UI_MAX_STRING_LEN + 1],  
            draw_name[MAX_ENTITY_NAME_SIZE+1],  
            export_spec[UF_CFI_MAX_PATH_NAME_SIZE],  
            tif_spec[UF_CFI_MAX_PATH_NAME_SIZE],  
            temp_spec[UF_CFI_MAX_PATH_NAME_SIZE];  
        build_similar_filespec(temp_spec, 0);  
        UF_CALL(UF_DRAW_ask_drawings(&n_drawings, &drawings));  
        for (ii = 0; ii < n_drawings; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_name(drawings[ii], draw_name));  
            sprintf(export_spec, "%s-%s", temp_spec, draw_name);  
        /*  UF_PLOT_* jobs are spawned to the NXPlot system which has trouble  
            doing more than one thing at a time, so wait for each one to finish  
            before submitting the next to insure success.  
        */  
            sprintf(cgm_spec, "%s.cgm", export_spec);  
            WRITE_S(cgm_spec);  
            uc4561(cgm_spec, 0); /* pre-delete any existing file 里海译:删除任何现有文件 */  
            if (UF_CALL(UF_PLOT_save_cgm(drawings[ii], NULL, NULL, NULL, cgm_spec)))  
                continue;  
            wait_for_file_to_exist(cgm_spec);  
            sprintf(tif_spec, "%s.tif", export_spec);  
            WRITE_S(tif_spec);  
            uc4561(tif_spec, 0); /* pre-delete any existing file 里海译:翻译为：删除任何现有的文件。 */  
        //  See PR 6996911  
        //  This fails in external mode if UGII_PLOT_CONVERT_OLD_BEHAVIOR=1:  
        //  UF_CALL(UF_PLOT_convert_file(cgm_spec, UF_PLOT_TIFF_FORMAT, tif_spec));  
        //  the cgm2tiff utility is "By far the best way to do a TIFF conversion"  
            sprintf(cmd, "cmd /c \"\"%s\" -mono \"%s\" \"%s\"\"",  
                "%UGII_BASE_DIR%\\NXPLOT\\cgm2tiff", cgm_spec, tif_spec);  
            ECHO("%s\n", cmd);  
            system(cmd);  
            wait_for_file_to_exist(tif_spec);  
        }  
        if (n_drawings > 0) UF_free(drawings);  
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
            part_name[MAX_FSPEC_SIZE+1];  
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

> 这段代码是用于NX的二次开发代码，主要实现了以下功能：
>
> 1. 定义了错误报告函数ECHO，用于打印错误信息。
> 2. 定义了宏UF_CALL，用于调用NX API，并报告错误。
> 3. 定义了write_string_to_listing_window函数，用于将字符串输出到NX的Listing窗口。
> 4. 实现了build_similar_temp_filespec函数，用于生成临时文件名。
> 5. 实现了wait_for_file_to_exist函数，用于等待文件生成完成。
> 6. 定义了do_it函数，用于遍历当前NX中的所有绘图，将其保存为CGM格式，然后转换为TIFF格式。
> 7. 实现了ufusr函数，用于初始化NX，调用do_it函数，然后结束NX。
> 8. 实现了main函数，用于处理命令行参数，打开指定的NX部件，调用do_it函数，最后结束NX。
> 9. 实现了check_load_status函数，用于检查部件加载状态，并打印错误信息。
>
> 总体来说，这段代码通过命令行参数指定NX部件，然后遍历每个部件中的绘图，将其保存为CGM格式并转换为TIFF格式。通过NX的二次开发接口实现了这一功能。
>
