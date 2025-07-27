### 【2984】transform imported geometry 将导入的几何体变换

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_trns.h>  
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
            *choice = resp - 4;  /* returns 1 or 2 里海译:翻译为“返回1或2”。 */  
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
    #define WRITENZ(X) if (X != 0) ECHO("%s = %d\n", #X, X);  
    static void do_it(void)  
    {  
        int  
            one = 1,  
            resp,  
            two = 2,  
            zero = 0;  
        tag_t  
            group;  
        double  
            abs[6] = { 1,0,0, 0,1,0 },  
            deltas[3] = { 5, 0, 0 },  
            mx[12],  
            org[3] = { 0,0,0 };  
        char  
            fspec[MAX_FSPEC_SIZE+1];  
        UF_import_part_modes_t  
            modes = { 1, 1, 0, 0, 0, 0, 0 };  
        while (prompt_for_any_existing_part_name("Import", fspec))  
        {  
            if (!UF_CALL(UF_PART_import(fspec, &modes, abs, org, 1.0, &group)))  
            {  
                FTN(uf5943)(deltas, mx);  
                FTN(uf5947)(mx, &group, &one, &one, &zero, &two, NULL, NULL, &resp);  
                WRITENZ(resp);  
            }  
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

> 这段NX二次开发代码主要实现以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了一个提示选择函数pick_one_of_two_choices，用于提示用户在两个选项之间进行选择。
> 3. 定义了一个组合目录和通配符的函数combine_directory_and_wildcard。
> 4. 定义了一个提示输入部件名称的函数prompt_for_part_name。
> 5. 定义了一个提示输入现有部件名称的函数prompt_for_any_existing_part_name。
> 6. 定义了一个执行导入和移动部件的操作的函数do_it。
> 7. 在ufusr函数中，初始化NX环境，调用do_it函数执行操作，然后终止NX环境。
> 8. 定义了一个卸载函数ufusr_ask_unload。
>
> 整体来看，这段代码实现了导入现有部件，将其移动一定距离的功能。同时，提供了良好的用户交互，使用户能够选择导入的部件类型，并提供了相应的错误处理和提示功能。
>
