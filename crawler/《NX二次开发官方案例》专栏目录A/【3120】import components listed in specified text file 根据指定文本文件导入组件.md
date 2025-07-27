### 【3120】import components listed in specified text file 根据指定文本文件导入组件

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_csys.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
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
    static logical prompt_for_text_file_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[MAX_FSPEC_SIZE+1] = { "*.txt" };  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "Text File Name", filter, "",  
            fspec, &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int ask_data_from_text_file(char *fspec, char ***data)  
    {  
        int  
            ii = 0,  
            n_lines = 0;  
        char  
            a_line[MAX_LINE_SIZE+1];  
        FILE  
            *the_file;  
        if ((the_file = fopen(fspec, "r")) != NULL)  
        {  
            while ((fgets(a_line, MAX_LINE_SIZE, the_file)) != NULL) n_lines++;  
            rewind(the_file);  
            UF_CALL(allocate_memory(n_lines * sizeof(char *), (void **)data));  
            for (ii = 0; ii < n_lines; ii++)  
            {  
                fgets(a_line, MAX_LINE_SIZE, the_file);  
            /*  strip off the carriage return 里海译:去掉回车换行 */  
                a_line[strlen(a_line) - 1] = '\0';  
                UF_CALL(allocate_memory((unsigned int)(strlen(a_line) + 1),  
                    (void **)&(*data)[ii]));  
                strcpy((*data)[ii], a_line);  
            }  
            fclose(the_file);  
        }  
        return n_lines;  
    }  
    static void ask_wcs_info(double origin[3], double axes[9])  
    {  
        tag_t  
            wcs,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, axes));  
    }  
    static void report_load_status(UF_PART_load_status_t *status)  
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
        if (status->n_parts > 0)  
        {  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static void add_component(char *fspec)  
    {  
        tag_t  
            instance,  
            part = UF_PART_ask_display_part();  
        double  
            origin[3],  
            matrix[9];  
        UF_PART_load_status_t  
            error_status;  
        ask_wcs_info(origin, matrix);  
        ECHO("Adding %s ...\n", fspec);  
        UF_CALL(UF_ASSEM_add_part_to_assembly(part, fspec, NULL, NULL,  
            origin, matrix, 0, &instance, &error_status));  
        if (error_status.n_parts > 0) report_load_status(&error_status);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_comps;  
        char  
            **cspecs,  
            tfn[MAX_FSPEC_SIZE+1];  
        if ((prompt_for_text_file_name("Component list", tfn)) &&  
            (n_comps = ask_data_from_text_file(tfn, &cspecs)) > 0)  
        {  
            for (ii = 0; ii < n_comps; ii++) add_component(cspecs[ii]);  
            UF_free_string_array(n_comps, cspecs);  
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

```

#### 代码解析

> 这段NX二次开发代码实现了以下功能：
>
> 1. ECHO函数：用于格式化打印日志信息。
> 2. report_error函数：用于报告函数调用错误，并打印错误码和错误信息。
> 3. prompt_for_text_file_name函数：用于弹出文件选择框，让用户选择文本文件。
> 4. allocate_memory函数：用于分配NX内存。
> 5. ask_data_from_text_file函数：用于读取文本文件中的数据行，并保存到指针数组中。
> 6. ask_wcs_info函数：用于获取当前工作坐标系的信息。
> 7. report_load_status函数：用于打印装配错误状态信息。
> 8. add_component函数：用于向装配体中添加一个组件。
> 9. do_it函数：用于执行主功能，读取组件列表文件，并逐个添加到装配体中。
> 10. ufusr函数：是NX二次开发程序的入口函数，在这里初始化NX系统，调用do_it函数执行主功能，然后终止NX系统。
> 11. ufusr_ask_unload函数：用于卸载NX二次开发程序。
>
> 通过这些函数，这段代码实现了读取组件列表文件，逐个添加到NX装配体的功能。
>
