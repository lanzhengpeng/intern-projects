### 【0764】create menu from text file 从文本文件创建菜单

#### 代码

```cpp
    /*HEAD CREATE_MENU_FROM_TEXT_FILE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_csys.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本中新增的功能。 */  
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
            /*  strip off the carriage return 译:去掉回车 */  
                if (ii < (n_lines - 1)) a_line[strlen(a_line) - 1] = '\0';  
                UF_CALL(allocate_memory(strlen(a_line)+1, (void **)&(*data)[ii]));  
                strcpy((*data)[ii], a_line);  
            }  
            fclose(the_file);  
        }  
        return n_lines;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_lines,  
            choice,  
            resp;  
        char  
            msg[133],  
            options[14][38],  
            **txtlns,  
            tfn[MAX_FSPEC_SIZE+1];  
        if ((prompt_for_text_file_name("Menu list", tfn)) &&  
            (n_lines = ask_data_from_text_file(tfn, &txtlns)) > 0)  
        {  
            for (ii = 0; ii < n_lines; ii++)  
            {  
                strcpy(options[ii], txtlns[ii]);  
            }  
            resp = uc1603("SELECT DESIRED OPTION",0,options ,n_lines );  
            choice = resp -4;  
            sprintf(msg, "Item %d selected, Value: %s", choice, options[choice - 1]);  
            uc1601(msg, TRUE);  
            UF_free_string_array(n_lines, txtlns);  
        }  
    }  
    /* 里海 */  
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

> 这段代码是一个NX Open Menu的二次开发示例，其主要功能是从文本文件中读取菜单选项，并显示一个列表对话框让用户选择。
>
> 代码的主要流程包括：
>
> 1. 定义错误报告函数report_error，用于打印错误信息。
> 2. 定义函数prompt_for_text_file_name，用于提示用户输入文本文件名。
> 3. 定义函数allocate_memory，用于分配内存。
> 4. 定义函数ask_data_from_text_file，用于从文本文件中读取数据并保存到指针数组中。
> 5. 定义函数do_it，是主函数，首先提示用户输入文本文件名，然后读取文件内容，最后显示列表对话框让用户选择。
> 6. 定义ufusr函数，这是NX Open Menu的入口函数，调用do_it函数。
> 7. 定义ufusr_ask_unload函数，用于卸载动态库。
>
> 这段代码展示了如何利用NX Open Menu进行二次开发，实现了从文本文件读取菜单数据并显示选择的功能。
>
