### 【0953】create top view jpeg for all parts in directory 创建目录中所有部件的顶视图JPEG

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_view.h>  
    #include <uf_obj.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_layout.h>  
    #include <uf_draw.h>  
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
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype) return body;  
        }  
        return NULL_TAG;  
    }  
    static void change_view_in_layout(char * view)  
    {  
        UF_CALL(uc6464("", "", view));  
    }  
    static void batch_shade(char * fspec)  
    {  
        UF_CALL(UF_DISP_batch_shade(fspec, 512, 512, UF_DISP_gouraud));  
    }  
    static void turn_off_drawing_display()  
    {  
        UF_CALL(UF_DRAW_set_display_state(1));  
    }  
    static void fit_view(char * viewname)  
    {  
        UF_CALL(uc6432(viewname,1));  
    }  
    static void build_similar_filespec(char *fspec, int ftype)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
        UF_CALL(uc4578(part_name, 2, part_name));  
        UF_CALL(uc4575(dspec, ftype, part_name, fspec));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            body = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            view[MAX_ENTITY_NAME_SIZE+1] = "TOP",  
            fspec[UF_CFI_MAX_PATH_NAME_SIZE] = "";  
        build_similar_filespec(fspec, 4);  
        strcat(fspec, "_top.jpg");  
        if ((body = ask_next_body(part, body)) != NULL_TAG)  
        {  
            turn_off_drawing_display();  
            change_view_in_layout(view);  
            fit_view(view);  
            batch_shade(fspec);  
        }  
        else  
            ECHO("%s\n", "    NO SOLIDS FOUND to shade");  
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
            UF_CALL(UF_PART_free_load_status(status));  
        }  
    }  
    /*  To run in external native mode NX:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external NX Manager mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( void )  
    {  
        int  
            cnt = 0,  
            resp;  
        tag_t  
            part;  
        char  
            dirspec[MAX_FSPEC_SIZE+1] = { "" },  
            fname[MAX_FSPEC_SIZE+1],  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (UF_CALL(UF_initialize())) return 1;  
        while (printf("Enter directory path:\n") && gets(dirspec) &&  
            strlen(dirspec))  
        {  
            uc4508(dirspec, (1<<13)|(1<<11), 0, "*.prt");  
            while ((resp = uc4518()) == 0)  
            {  
                UF_CALL(uc4600(fname));  
                UF_CALL(uc4575(dirspec, 2, fname, part_name));  
                ECHO( "%d.  %s\n", ++cnt, part_name);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) report_load_status(&status);  
                if (!status.failed)  
                {  
                    do_it();  
                    UF_PART_close_all();  
                }  
            }  
            UF_CALL(uc4548());  
            ECHO("\nProcessed %d parts.\n", cnt);  
        }  
    }

```

#### 代码解析

> 根据代码内容，这是一段用于NX（Siemens NX）CAD系统的二次开发代码，主要功能包括：
>
> 1. 初始化和关闭NX环境。
> 2. 循环提示用户输入目录路径，并遍历目录下所有prt文件。
> 3. 对于每个prt文件，打开并显示在NX中。
> 4. 如果prt文件中包含实体，则切换到TOP视图，缩放视图，然后批量渲染当前视图并保存为JPEG图片。
> 5. 在批处理过程中，关闭NX的图形显示以提高效率。
> 6. 使用ECHO函数在NX日志窗口输出信息。
> 7. 使用UF_CALL宏封装NX API调用，并检查返回码，如果失败则输出错误信息。
> 8. 定义了ask_next_body等辅助函数。
> 9. 主函数main实现了整个批处理流程。
> 10. ufusr是NX的二次开发标准入口函数，可以不修改直接编译运行。
>
> 总体来说，这段代码实现了通过批处理的方式，对目录下的所有prt文件进行打开、渲染和保存图片的功能。
>
