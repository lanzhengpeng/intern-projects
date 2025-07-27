### 【1910】report all drawings views out-of-date status 查询所有图纸视图的过时状态

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_ugmgr.h>  
    #include <uf_text.h>  
    #include <uf_draw.h>  
    #include <uf_obj.h>  
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
    static void do_it(void)  
    {  
        logical  
            out_of_date;  
        int  
            ii, jj,  
            n_dwgs,  
            n_views;  
        tag_t  
            *dwgs,  
            *views;  
        char  
            name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_DRAW_ask_drawings(&n_dwgs, &dwgs));  
        for (ii = 0; ii < n_dwgs; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_name(dwgs[ii], name));  
            UF_CALL(UF_DRAW_is_object_out_of_date(dwgs[ii], &out_of_date));  
            if (out_of_date)  
                ECHO(" %s is Out-Of-Date !\n", name);  
            else  
                ECHO(" %s is Up-To-Date\n", name);  
            UF_CALL(UF_DRAW_ask_views(dwgs[ii], &n_views, &views));  
            for (jj = 0; jj < n_views; jj++)  
            {  
                UF_CALL(UF_OBJ_ask_name(views[jj], name));  
                UF_CALL(UF_DRAW_is_object_out_of_date(views[jj], &out_of_date));  
                if (out_of_date)  
                    ECHO("  %s is Out-Of-Date !\n", name);  
                else  
                    ECHO("  %s is Up-To-Date\n", name);  
            }  
            if (n_views > 0) UF_free(views);  
        }  
        if (n_dwgs > 0) UF_free(dwgs);  
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
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
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

> 这段NX二次开发代码的功能主要包括以下几点：
>
> 1. 初始化和终止NX API。
> 2. 获取当前NX部件中所有图形和视图对象，并判断每个对象是否为最新。
> 3. 如果对象不是最新，会打印出提示信息。
> 4. 提供了错误报告函数，用于捕获并打印API调用错误。
> 5. 可以作为NX用户自定义函数运行，也可以作为独立程序运行，接受命令行参数指定的部件文件。
> 6. 打印出加载每个部件的提示信息，如果有加载失败，会打印出失败原因。
> 7. 在独立运行模式下，会遍历命令行参数指定的所有部件文件，并检查每个部件中的图形和视图对象。
> 8. 使用了NX提供的文件系统函数来获取参数指定的部件文件。
> 9. 整体结构清晰，包含错误处理和资源释放等，是一个典型的NX二次开发示例。
>
