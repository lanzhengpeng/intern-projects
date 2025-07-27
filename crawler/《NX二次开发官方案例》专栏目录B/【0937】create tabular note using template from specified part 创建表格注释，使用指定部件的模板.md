### 【0937】create tabular note using template from specified part 创建表格注释，使用指定部件的模板

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_tabnot.h>  
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
    static logical pick_one_of_two_choices(char *prompt, char *option_one,  
        char *option_two, int *choice)  
    {  
        int  
            resp;  
        char  
            options[2][38];  
        strncpy(&options[0][0], option_one, 37);  
        strncpy(&options[1][0], option_two, 37);  
        options[0][37] = '\0';  
        options[1][37] = '\0';  
        resp = uc1603(prompt, *choice, options, 2);  
        if ((resp > 4) && (resp < 19))  
        {  
            *choice = resp - 4;  /* returns 1 or 2 里海译:Returns 1 or 2。 */  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void combine_directory_and_wildcard(char *dir, char *fltr, char *spec)  
    {  
        if (!strcmp(dir, ""))  
            strcpy(spec, fltr);  
        else  
        {  
            UF_CALL(uc4575(dir, 0, "junk.xxx", spec));  
            strcpy(strstr(spec, "junk.xxx"), fltr);  
        }  
    }  
    static logical prompt_for_part_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[MAX_FSPEC_SIZE+1],  
            *p_dir,  
            *p_ext;  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_PART_DIR, &p_dir));  
        UF_CALL(UF_UI_ask_dialog_filter(UF_UI_PART_OPEN_FLTR, &p_ext));  
        combine_directory_and_wildcard(p_dir, p_ext, filter);  
        UF_free(p_dir);  
        UF_free(p_ext);  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "Part Name", filter, "", fspec,  
            &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }  
    static logical prompt_for_any_existing_part_name(char *prompt, char *fspec)  
    {  
        logical  
            nxman,  
            unused;  
        int  
            mode = 1,  
            resp;  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (nxman)  
        {  
            if (pick_one_of_two_choices(prompt, "in Native", "in TcEng",  
                &mode))  
            {  
                if (mode == 1)  
                    return (prompt_for_part_name(prompt, fspec));  
                else  
                {  
                    UF_CALL(UF_UI_set_prompt(prompt));  
                    if (UF_CALL(UF_UI_ask_open_part_filename(fspec, &unused, &resp))  
                        || (resp == UF_UI_CANCEL)) return FALSE;  
                    return TRUE;  
                }  
            }  
            else return FALSE;  
        }  
        else  
        {  
            UF_CALL(UF_UI_set_prompt(prompt));  
            if (UF_CALL(UF_UI_ask_open_part_filename(fspec, &unused, &resp))  
                || (resp == UF_UI_CANCEL)) return FALSE;  
            return TRUE;  
        }  
    }  
    static logical specify_position(char *prompt, double pos[3])  
    {  
        int  
            resp = uc1615(prompt, pos);  
        if (resp == 5) return TRUE;  
        else return FALSE;  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1];  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (cols > 0)  
                if (ii == 0) ECHO("%s = ", name);  
                else ECHO("%s", blanks);  
            for (jj = 0; jj < cols; jj++)  
            {  
                ECHO("%.3f", array[kk++]);  
                if (jj < cols - 1) ECHO("%s", ", ");  
            }  
            if (cols > 0) ECHO("%s", "\n");  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            tab_note = NULL_TAG;  
        char  
            template[MAX_FSPEC_SIZE+1] = {""};  
        double  
            origin[3];  
        if (prompt_for_any_existing_part_name("Template", template) &&  
            specify_position("Tabular note origin", origin))  
        {  
            WRITE_S(template);  
            WRITE3F(origin);  
            UF_CALL(UF_TABNOT_create_from_template(template, origin, &tab_note));  
            WRITE_D(tab_note);  
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

> 这段代码是NX的二次开发代码，主要用于创建一个基于模板的表格注释。下面是主要功能的介绍：
>
> 1. 定义了一个宏 UF_CALL，用于调用NX API函数，并在调用失败时打印错误信息。
> 2. 定义了一个宏 WRITE_S，用于将字符串输出到列表窗口。
> 3. 定义了一个宏 WRITE_D，用于将整数输出到列表窗口。
> 4. 定义了一个宏 WRITE3F，用于将三维数组输出到列表窗口。
> 5. 定义了一个函数 ECHO，用于输出信息到列表窗口和系统日志。
> 6. 定义了一个函数 report_error，用于打印错误信息。
> 7. 定义了一个函数 pick_one_of_two_choices，用于弹出对话框让用户在两个选项之间选择。
> 8. 定义了一个函数 combine_directory_and_wildcard，用于组合目录和通配符。
> 9. 定义了一个函数 prompt_for_part_name，用于弹出对话框让用户选择一个部件文件。
> 10. 定义了一个函数 prompt_for_any_existing_part_name，用于弹出对话框让用户选择一个已存在的部件文件。
> 11. 定义了一个函数 specify_position，用于弹出对话框让用户指定一个位置。
> 12. 定义了一个函数 do_it，用于执行创建表格注释的逻辑：弹出对话框让用户选择一个模板部件文件。弹出对话框让用户指定表格注释的原点位置。调用 UF_TABNOT_create_from_template API 创建基于模板的表格注释。输出表格注释的 tag 值。
> 13. 弹出对话框让用户选择一个模板部件文件。
> 14. 弹出对话框让用户指定表格注释的原点位置。
> 15. 调用 UF_TABNOT_create_from_template API 创建基于模板的表格注释。
> 16. 输出表格注释的 tag 值。
> 17. 定义了 ufusr 函数，作为二次开发的入口函数，用于初始化和执行 do_it 函数，然后终止 NX。
> 18. 定义了 ufusr_ask_unload 函数，返回立即卸载标志。
>
> 总体来说，这段代码实现了创建基于模板的表格注释的功能，并且提供了友好的用户交互。
>
