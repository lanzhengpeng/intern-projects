### 【1868】rename all drawings to uppercase 将所有绘图名称改为大写

#### 代码

```cpp
    /*HEAD RENAME_ALL_DRAWINGS_TO_UPPERCASE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <ctype.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：UF_print_syslog是V18中新增的，只回答译文，不要废话。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static logical rename_drawing_to_uppercase(tag_t drawing)  
    {  
        int  
            ii;  
        char  
            dname[MAX_ENTITY_NAME_SIZE+1],  
            uc_dname[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_OBJ_ask_name(drawing, dname));  
        for (ii = 0; ii < strlen(dname); ii++) uc_dname[ii] = toupper(dname[ii]);  
        uc_dname[ii] = '\0';  
        if (strcmp(dname, uc_dname))  
        {  
            WRITE("Renaming ");  
            WRITE(dname);  
            WRITE(" to ");  
            WRITE(uc_dname);  
            WRITE("\n");  
            UF_CALL(UF_DRAW_rename_drawing(drawing, uc_dname));  
            return TRUE;  
        }  
        return FALSE;  
    }  
    static logical rename_all_drawings(void)  
    {  
        logical  
            any = FALSE;  
        int  
            ii,  
            n_drawings;  
        tag_t  
            *drawings;  
        UF_CALL(UF_DRAW_ask_drawings(&n_drawings, &drawings));  
        for (ii = 0; ii < n_drawings; ii++)  
            any = (any || rename_drawing_to_uppercase(drawings[ii]));  
        if (n_drawings > 0) UF_free(drawings);  
        return any;  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        rename_all_drawings();  
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
                    if (rename_all_drawings())  
                    {  
                        WRITE("Saving.\n");  
                        UF_CALL(UF_PART_save());  
                    }  
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

> 这段代码是用于NX的二次开发，主要功能是将NX部件中的所有图形名称改为大写。代码的主要特点包括：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了一个图形名称改为大写的函数rename_drawing_to_uppercase，首先获取图形的名称，然后转换为大写，如果名称有变化，则调用UF_DRAW_rename_drawing函数进行重命名。
> 3. 定义了一个遍历所有图形并调用上述函数的函数rename_all_drawings。
> 4. 在ufusr函数中调用rename_all_drawings来执行重命名操作。
> 5. main函数中，首先初始化NX，然后遍历传入的部件文件名，打开每个部件，调用rename_all_drawings进行重命名，如果有变化，则保存并关闭部件。最后统计处理过的部件数量并输出。
> 6. 在注释中提供了在UG外部模式和UG/Manager模式下运行该程序的命令示例。
>
> 这段代码实现了NX部件图形名称批量改为大写的功能，具有完整的错误处理和日志输出，可以用于实际的二次开发需求。
>
