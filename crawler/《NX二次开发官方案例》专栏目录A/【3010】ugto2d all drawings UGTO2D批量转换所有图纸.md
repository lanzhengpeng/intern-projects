### 【3010】ugto2d all drawings UGTO2D批量转换所有图纸

#### 代码

```cpp
    /*HEAD UGTO2D_ALL_DRAWINGS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：UF_print_syslog是在V18中新加入的。 */  
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
    static tag_t ask_next_drawing(tag_t part, tag_t drawing)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drawing_type, &drawing));  
        return (drawing);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            drawing = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            cmd[UF_UI_MAX_STRING_LEN+1],  
            draw_name[MAX_ENTITY_NAME_SIZE+1],  
            export_fspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_dir[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            *prog_name;  
    #if defined(_WIN32)  
        prog_name = "%UGII_BASE_DIR%\\UGTO2D\\ugto2d";  
    #else  
        prog_name = "$UGII_BASE_DIR/ugto2d/ugto2d";  
    #endif  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(uc4576(part_fspec, 2, part_dir, part_name));  
        UF_CALL(uc4578(part_name, 2, part_name));  
        while ((drawing = ask_next_drawing(part, drawing)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_name(drawing, draw_name));  
            sprintf(export_fspec , "%s-%s", part_name, draw_name);  
            export_fspec[26] = '\0';  /* In case it is too long 里海译:如果太长 */  
            UF_CALL(uc4575(part_dir, 2, export_fspec, export_fspec));  
            sprintf(cmd,  "%s i=%s o=%s m=Modeling draw=%s", prog_name,  
                part_fspec, export_fspec, draw_name);  
            system(cmd);  
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

> 这段代码是用于NX的二次开发，主要功能包括：
>
> 1. 初始化NX环境并处理参数。
> 2. 循环获取当前活动部件中的每个绘图，并使用UGTO2D工具将其转换为2D图纸。
> 3. 打印错误信息。
> 4. 提供命令行参数来指定要处理的部件，支持绝对路径和数据库路径两种格式。
> 5. 打印处理进度和结果。
> 6. 安全关闭所有打开的部件。
>
> 主要特点包括：
>
> 整体上，该代码实现了利用UGTO2D工具批量将NX绘图转换为2D图纸的功能，具有较好的可扩展性和错误处理机制。
>
