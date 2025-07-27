### 【1995】report AsSaved paths of components of specified assembly part 报告指定装配零件组件的AsSaved路径

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
    static logical prompt_for_existing_part_name(char *prompt, char *fspec)  
    {  
        logical  
            unused;  
        int  
            resp;  
        UF_CALL(UF_UI_set_prompt(prompt));  
        if (UF_CALL(UF_UI_ask_open_part_filename(fspec, &unused, &resp))  
            || (resp == UF_UI_CANCEL)) return FALSE;  
        return TRUE;  
    }  
    static void build_unique_temp_name(char *fspec, int ftype)  
    {  
        char  
            *tmp_dir,  
            unique[UF_CFI_MAX_FILE_NAME_SIZE];  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4577(unique));  
        UF_CALL(uc4575(tmp_dir, ftype, unique, fspec));  
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
            /*  strip off the carriage return 里海译:脱掉回车 */  
                a_line[strlen(a_line) - 1] = '\0';  
                UF_CALL(allocate_memory((unsigned int)(strlen(a_line) + 1),  
                    (void **)&(*data)[ii]));  
                strcpy((*data)[ii], a_line);  
            }  
            fclose(the_file);  
        }  
        return n_lines;  
    }  
    static void do_it(void)  
    {  
        logical nxman;  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (nxman)  
        {  
            uc1601("This only works in Native mode NX.", 1);  
            return;  
        }  
        char fspec[MAX_FSPEC_BUFSIZE] = { "" };  
        prompt_for_existing_part_name("Select an assembly", fspec);  
        char temp[MAX_FSPEC_BUFSIZE] = { "" };  
        build_unique_temp_name(temp, 4); // 4 = text  
        // ECHO("%s\n", temp);  
        char cmd[UF_UI_MAX_STRING_LEN + 1];  
        sprintf(cmd, "ug_edit_part_names -list %s > %s", fspec, temp);  
        system(cmd);  
        char **data = NULL;  
        int n = ask_data_from_text_file(temp, &data);  
        for (int ii = 0; ii < n; ii++)  
        {  
            char leaf[MAX_FSPEC_BUFSIZE] = { "" };  
            char folder[MAX_FSPEC_BUFSIZE] = { "" };  
            sscanf(data[ii], "%s (%s", leaf, folder);  
            folder[strlen(folder) - 1] = '\0'; /* strip off the trailing ')' 里海译:剥离尾随的')' */  
            ECHO("%s\\%s\n", folder, leaf);  
        }  
        UF_free_string_array(n, data);  
        _unlink(temp);  
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

> 这段代码是NX二次开发代码，主要实现了以下功能：
>
> 1. 定义了一个宏UF_CALL，用于调用NX API函数，并打印错误信息。
> 2. 实现了prompt_for_existing_part_name函数，用于提示用户选择一个已存在的NX部件文件。
> 3. 实现了build_unique_temp_name函数，用于生成一个唯一的临时文件名。
> 4. 实现了allocate_memory函数，用于申请内存。
> 5. 实现了ask_data_from_text_file函数，用于从文本文件中读取数据，并保存到字符串数组中。
> 6. 实现了do_it函数，用于主流程：检查是否处于NX Manager模式下，如果是则提示错误并返回。提示用户选择一个装配文件。生成临时文件名。使用系统调用执行NX命令，将装配文件中的部件名列表输出到临时文件中。读取临时文件中的数据，并解析出部件的名称和所在文件夹。打印出每个部件的名称和所在文件夹。释放内存，删除临时文件。
> 7. 检查是否处于NX Manager模式下，如果是则提示错误并返回。
> 8. 提示用户选择一个装配文件。
> 9. 生成临时文件名。
> 10. 使用系统调用执行NX命令，将装配文件中的部件名列表输出到临时文件中。
> 11. 读取临时文件中的数据，并解析出部件的名称和所在文件夹。
> 12. 打印出每个部件的名称和所在文件夹。
> 13. 释放内存，删除临时文件。
> 14. 实现了ufusr函数，用于NX启动时调用，进行初始化，执行主流程，然后进行清理。
> 15. 实现了ufusr_ask_unload函数，用于卸载时立即返回。
>
> 综上所述，这段代码主要用于读取NX装配文件中的部件列表，并打印出每个部件的名称和所在文件夹，以便进行进一步处理。
>
