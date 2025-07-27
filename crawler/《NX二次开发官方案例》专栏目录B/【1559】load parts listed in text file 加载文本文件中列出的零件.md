### 【1559】load parts listed in text file 加载文本文件中列出的零件

#### 代码

```cpp
    /*HEAD LOAD_PARTS_LISTED_IN_TEXT_FILE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_ugmgr.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n*** ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog在V18中是新增的功能。 */  
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
            *the_file = NULL;  
        if ((the_file = fopen(fspec, "r")) != NULL)  
        {  
            while ((fgets(a_line, MAX_LINE_SIZE, the_file)) != NULL) n_lines++;  
            rewind(the_file);  
            UF_CALL(allocate_memory(n_lines * sizeof(char *), (void **)data));  
            for (ii = 0; ii < n_lines; ii++)  
            {  
                fgets(a_line, MAX_LINE_SIZE, the_file);  
            /*  strip off the carriage return 里海译:去掉回车 */  
                if (ii < (n_lines - 1)) a_line[strlen(a_line) - 1] = '\0';  
                UF_CALL(allocate_memory(strlen(a_line) + 1, (void **)&(*data)[ii]));  
                strcpy((*data)[ii], a_line);  
            }  
            fclose(the_file);  
        }  
        return n_lines;  
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
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(status->file_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static void do_it(void)  
    {  
        logical  
            nxman;  
        int  
            ii,  
            n;  
        tag_t  
            part;  
        char  
            **part_names,  
            prompts[2][31] = { "List of part path names", "List of cli names" },  
            tfn[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_PART_load_status_t  
            status;  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (!prompt_for_text_file_name(prompts[nxman], tfn)) return;  
        n = ask_data_from_text_file(tfn, &part_names);  
        for (ii = 0; ii < n; ii++)  
        {  
            WRITE(part_names[ii]);  
            if (!UF_PART_is_loaded(part_names[ii]))  
            {  
                WRITE(": Loading...\n");  
                UF_CALL(UF_PART_open(part_names[ii], &part, &status));  
                if (status.n_parts > 0) report_load_status(&status);  
            }  
            else  
            {  
                WRITE(": Already Loaded.\n");  
            }  
        }  
        UF_free_string_array(n, part_names);  
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

> 这段代码是一个NX Open C++应用程序，其主要功能是从文本文件中读取部件名称，并尝试加载这些部件。
>
> 代码的关键部分如下：
>
> 1. 头文件包含：包含了NX Open C++库的必要头文件，如uf.h、uf_ui.h等。
> 2. 错误报告函数：定义了一个错误报告函数report_error，用于在调用NX函数出错时输出错误信息。
> 3. 内存分配函数：定义了一个内存分配函数allocate_memory，用于调用NX的内存分配函数UF_allocate_memory。
> 4. 从文本文件读取数据：定义了一个函数ask_data_from_text_file，用于从指定文件读取字符串数组。
> 5. 提示用户输入文本文件名：定义了一个函数prompt_for_text_file_name，用于提示用户输入文本文件名。
> 6. 报告部件加载状态：定义了一个函数report_load_status，用于报告部件的加载状态。
> 7. 主函数do_it：定义了主函数do_it，用于从文本文件读取部件名称，并尝试加载这些部件。
> 8. ufusr函数：定义了ufusr函数，作为程序的入口点，调用UF_initialize初始化NX，调用do_it执行主要功能，最后调用UF_terminate终止NX。
> 9. 卸载提示：定义了ufusr_ask_unload函数，返回立即卸载的标识。
>
> 总的来说，这段代码通过读取文本文件中的部件名称，调用NX的部件加载函数，实现了批量加载部件的功能。
>
