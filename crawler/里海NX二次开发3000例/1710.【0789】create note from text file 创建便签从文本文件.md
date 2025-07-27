### 【0789】create note from text file 创建便签从文本文件

#### 代码

```cpp
    /*HEAD CREATE_NOTE_FROM_TEXT_FILE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog是在V18版本中新增的函数。它的主要作用是打印系统日志信息。在翻译时，只需要简洁地表达这个信息即可，不需要添加任何其他无关的内容。 */  
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
    static logical specify_position(char *prompt, double pos[3])  
    {  
        int  
            resp = uc1615(prompt, pos);  
        if (resp == 5) return TRUE;  
        else return FALSE;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int read_data_from_text_file(char *fspec, char ***data)  
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
                a_line[strlen(a_line) - 1] = '\0'; /* take off carriage return 译:去掉回车换行 */  
                UF_CALL(allocate_memory(strlen(a_line) + 1, (void **)&(*data)[ii]));  
                strcpy((*data)[ii], a_line);  
            }  
            fclose(the_file);  
        }  
        return n_lines;  
    }  
    static void do_it(void)  
    {  
        int  
            n_lines;  
        tag_t  
            note;  
        double  
            origin[3];  
        char  
            **note_text,  
            tfn[MAX_FSPEC_SIZE+1] = { "" };  
        while (prompt_for_text_file_name("Create note", tfn) &&  
            specify_position("Note Origin", origin))  
        {  
            n_lines = read_data_from_text_file(tfn, &note_text);  
            if (n_lines == 0) continue;  
            UF_CALL(UF_DRF_create_note(n_lines, note_text, origin, 0, &note));  
            UF_free_string_array(n_lines, note_text);  
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

> 这段代码是用于NX二次开发的，其主要功能是读取文本文件并创建注释。以下是该代码的主要功能介绍：
>
> 1. 头文件：包含了必要的头文件，如stdio.h、string.h、uf.h等。
> 2. 错误报告函数：定义了report_error函数，用于报告UF函数调用的错误信息。
> 3. 提示用户输入文本文件名：定义了prompt_for_text_file_name函数，用于提示用户输入文本文件名。
> 4. 提示用户输入注释位置：定义了specify_position函数，用于提示用户输入注释的位置。
> 5. 内存分配函数：定义了allocate_memory函数，用于分配内存。
> 6. 读取文本文件数据：定义了read_data_from_text_file函数，用于读取文本文件的数据，并保存为二维字符数组。
> 7. 执行主函数：定义了do_it函数，用于循环提示用户输入文本文件和注释位置，并调用UF函数创建注释。
> 8. NX启动函数：定义了ufusr函数，在NX启动时调用，执行do_it函数。
> 9. 卸载函数：定义了ufusr_ask_unload函数，用于卸载时立即卸载。
>
> 总体来说，这段代码通过交互式地提示用户输入，读取文本文件，并调用UF函数在指定位置创建注释。
>
