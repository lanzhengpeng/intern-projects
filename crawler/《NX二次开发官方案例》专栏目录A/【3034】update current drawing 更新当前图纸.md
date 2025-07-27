### 【3034】update current drawing 更新当前图纸

#### 代码

```cpp
    /*HEAD UPDATE_CURRENT_DRAWING CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #include <uf_obj.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog 是 V18 版本新增的。 */  
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
    static void do_it(void)  
    {  
        tag_t  
            dwg;  
        logical  
            out_of_date;  
        char  
            drawing[UF_OBJ_NAME_LEN + 1];  
        UF_CALL(UF_DRAW_ask_current_drawing(&dwg));  
        UF_CALL(UF_DRAW_is_object_out_of_date(dwg, &out_of_date));  
        if (out_of_date)  
        {  
            UF_CALL(UF_OBJ_ask_name(dwg, drawing));  
            WRITE("\tupdating ");  
            WRITE(drawing);  
            WRITE("\n");  
            UF_CALL(UF_DRAW_upd_out_of_date_views ( dwg ));  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
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
                    UF_CALL(UF_PART_save());  
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

> 这段代码是一个NX二次开发的应用程序，主要实现以下功能：
>
> 1. 检查当前打开的NX图纸是否已过期，如果过期，则更新图纸。
> 2. 可以在NX外部模式下运行，接收多个零件文件路径作为参数，分别打开这些零件文件并检查图纸更新。
> 3. 使用宏定义UF_CALL来捕获NX API调用错误，并在发生错误时输出错误信息。
> 4. 使用UF_PRINT_SYSLOG函数将错误信息打印到NX日志文件中。
> 5. 通过UF_DRAW_UPD_OUT_OF_DATE_VIEWS函数来更新过期的图纸视图。
> 6. 可以在NX内部模式下运行，通过UGII_USER_FUNC_ADD命令加载。
> 7. 使用了NX提供的零件管理器接口，可以在NX外部模式下接收零件数据库路径作为参数。
> 8. 在主函数中，通过遍历传入的零件文件路径，打开每个零件并检查图纸更新，最后保存并关闭所有零件。
> 9. 输出处理的零件数量。
> 10. 使用了NX提供的错误报告机制，可以输出错误代码对应的错误信息。
> 11. 定义了卸载函数，可以在不需要时卸载该应用程序。
>
> 总体而言，这是一个典型的NX二次开发应用程序，实现了零件图纸的自动更新检查和更新功能。
>
