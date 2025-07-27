### 【3028】update all drawings 更新所有图纸

#### 代码

```cpp
    /*HEAD UPDATE_ALL_DRAWINGS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #include <uf_obj.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    static logical update_drawing(tag_t dwg, logical always)  
    {  
        logical  
            did_something = FALSE,  
            out_of_date;  
        int  
            ii,  
            n_views;  
        tag_t  
            *views;  
        char  
            drawing[MAX_ENTITY_NAME_SIZE+1],  
            msg[UF_UI_MAX_STRING_LEN],  
            view[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_CALL(UF_OBJ_ask_name(dwg, drawing)));  
        sprintf(msg, "  %s:  ", drawing);  
        ECHO(msg);  
        UF_CALL(UF_DRAW_is_object_out_of_date(dwg, &out_of_date));  
        if (always || out_of_date)  
        {  
            ECHO("updating views\n");  
            UF_CALL(UF_DRAW_ask_views(dwg, &n_views, &views));  
            for (ii = 0; ii < n_views; ii++)  
            {  
                UF_CALL(UF_OBJ_ask_name(views[ii], view));  
                sprintf(msg, "    %s:  ", view);  
                ECHO(msg);  
                UF_CALL(UF_DRAW_is_object_out_of_date(views[ii], &out_of_date));  
                if (always || out_of_date)  
                {  
                    did_something = TRUE;  
                    UF_CALL(UF_DRAW_update_one_view(dwg, views[ii]));  
                    ECHO("updated\n");  
                }  
                else  
                {  
                    ECHO("up to date\n");  
                }  
            }  
        }  
        else  
        {  
            ECHO("up to date\n");  
        }  
        if (n_views > 0) UF_free(views);  
        return did_something;  
    }  
    static int update_drawings(logical always)  
    {  
        int  
            update_cnt = 0,  
            ii,  
            n_dwgs;  
        tag_t  
            *dwgs;  
        UF_CALL(UF_DRAW_ask_drawings(&n_dwgs, &dwgs));  
        for (ii = 0; ii < n_dwgs; ii++)  
            update_cnt = update_cnt + update_drawing(dwgs[ii], always);  
        if (n_dwgs > 0) UF_free(dwgs);  
        return update_cnt;  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            update_drawings(FALSE);  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
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
            msg[UF_UI_MAX_STRING_LEN],  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                sprintf(msg, "%d.  %s\n", ++cnt, part_name);  
                ECHO(msg);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) report_load_status(&status);  
                if (!status.failed)  
                {  
                    if (update_drawings(FALSE) > 0)  
                        UF_CALL(UF_PART_save());  
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

> 这段代码是一个NX Open C++应用程序，用于更新NX装配体中的所有图纸。
>
> 主要功能包括：
>
> 1. 定义了一个报告错误函数report_error，用于打印出错误代码和错误信息。
> 2. 定义了一个更新图纸函数update_drawing，用于更新一个图纸中的所有视图。它会检查每个视图是否需要更新，如果需要则调用UF_DRAW_update_one_view函数更新视图。
> 3. 定义了一个更新所有图纸函数update_drawings，用于遍历装配体中的所有图纸，并调用update_drawing函数更新每个图纸。
> 4. 定义了一个用户函数ufusr，用于在NX启动时调用update_drawings函数更新所有图纸。
> 5. 主函数main中，遍历命令行传入的零件文件路径，打开每个零件，检查并更新图纸，然后保存并关闭零件。
> 6. 使用UF_PART_open函数打开零件文件，使用UF_PART_close_all函数关闭所有打开的零件。
> 7. 使用UF_DRAW相关的函数来获取图纸信息、更新图纸等。
> 8. 使用UF_OBJ相关的函数来获取对象名称。
> 9. 使用UF_SYS相关的函数来打印系统日志。
> 10. 使用标准C语言库函数进行字符串处理等。
>
> 总的来说，这段代码实现了NX装配体中所有图纸的自动更新，通过遍历零件文件路径，打开每个零件，检查图纸是否需要更新，并调用更新函数来实现。
>
