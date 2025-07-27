### 【0483】choose multiple items from text file list 从文本文件列表中选择多个项目

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
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
    static logical choose_items(char *prompt, char **choices, int n_choices,  
        int *chosen)  
    {  
        logical  
            more;  
        int  
            a = 0,  
            ii,  
            resp,  
            selected[14],  
            z;  
        char  
            options[14][38];  
        do  
        {  
            z = (n_choices - a < 14) ? n_choices - a : 14;  
            for (ii = 0; ii < z; ii++)  
            {  
                strncpy(&options[ii][0], choices[a+ii], 37);  
                options[ii][37] = '\0';  
            }  
            if ((z == 14) && ((a + z) < n_choices))  
            {  
                more = true;  
                strcpy(&options[13][0], "More...");  
            }  
            else  
                more = false;  
            switch (resp = uc1605(prompt, 0, options, z, selected))  
            {  
                case 1:                     /* Back 译:Back的翻译是“背部”。 */  
                    if (a == 0) return false;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 译:Cancel的中文翻译是“取消”。 */  
                    return false;  
                    break;  
                default:                    /* OK 译:翻译OK，只回答译文，不要废话。 */  
                    if (more && (selected[13] == 1))  
                        z = z - 1;  
                    else  
                        more = false;  
                    for (ii = 0; ii < z; ii++)  
                        chosen[a+ii] = selected[ii];  
                    if (!more)  
                        return true;  
                    a = a + 13;  
                    break;  
            }  
        } while (TRUE);  
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
                UF_CALL(allocate_memory((int)(strlen(a_line) + 1),  
                    (void **)&(*data)[ii]));  
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
            n_opts,  
            *sel_opts;  
        char  
            **opts;  
        n_opts = read_data_from_text_file("Z:\\IR\\67\\6736984\\test.txt", &opts);  
        if (n_opts == 0) return;  
        UF_CALL(allocate_memory(n_opts * sizeof(int), (void **)&sel_opts));  
        for (ii = 0; ii < n_opts; ii++) sel_opts[ii] = 0;  
        while (choose_items("Choose Items", opts, n_opts, sel_opts))  
        {  
            ECHO("Picked these:\n");  
            for (ii = 0; ii < n_opts; ii++)  
                if (sel_opts[ii] == 1)  
                {  
                    ECHO("%d.  %s\n", ii, opts[ii]);  
                // act on this choice here  
                    sel_opts[ii] = 0;  
                }  
        }  
        UF_free(sel_opts);  
        UF_free(opts);  
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

> 根据代码内容，这是一段基于NX Open C API的NX二次开发代码，其主要功能如下：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于在调用NX API函数出错时输出错误信息。
> 2. 内存分配函数：定义了一个内存分配函数allocate_memory，用于调用NX API函数UF_allocate_memory来分配内存。
> 3. 文本文件读取函数：定义了一个文本文件读取函数read_data_from_text_file，用于读取文本文件中的每一行数据，并保存到指针数组中。
> 4. 交互式选择函数：定义了一个交互式选择函数choose_items，用于在NX界面上显示选项供用户选择，并返回用户的选择结果。
> 5. 主要逻辑函数：定义了一个主要逻辑函数do_it，用于读取文本文件中的选项，然后在NX界面上显示这些选项供用户选择，并处理用户的选择。
> 6. NX Open初始化函数：定义了UFusr函数，作为NX Open的初始化函数，在该函数中调用主要逻辑函数do_it来执行主要功能。
> 7. NX Open卸载函数：定义了ufusr_ask_unload函数，作为NX Open的卸载函数，在该函数中返回立即卸载标志。
>
> 综上所述，这段代码的主要功能是读取文本文件中的选项，然后在NX界面上显示这些选项供用户选择，并根据用户的选择进行相应的处理。
>
