### 【1426】import parts list from specified template part 从指定模板零件导入零件清单

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_plist.h>  
    #include <uf_ugmgr.h>  
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
            *choice = resp - 4;  /* returns 1 or 2 里海译:返回1或2。 */  
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
            if (pick_one_of_two_choices(prompt, "in Native", "in Teamcenter",  
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
    static void do_it(void)  
    {  
        tag_t  
            plist;  
        double  
            origin[3];  
        char  
            fspec[MAX_FSPEC_SIZE+1] = {""};  
        if (prompt_for_any_existing_part_name("Parts List Template", fspec) &&  
            specify_position("Parts List Origin", origin))  
        {  
        //  NOTE: The encoded name format must used in TCIN mode  
        //        CLI name format will not work.  See PR 7651069  
            ECHO("importing template from %s\n", fspec);  
            UF_CALL(UF_PLIST_create_from_template(fspec, origin, &plist));  
            ECHO("plist tag = %d\n", plist);  
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

> 这段代码是用于NX的二次开发代码，主要包括以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在调用NX API失败时输出错误信息。
> 2. 定义了一个对话框函数pick_one_of_two_choices，用于弹出对话框让用户在两个选项中选择一个。
> 3. 定义了一个函数combine_directory_and_wildcard，用于拼接目录名和通配符，用于文件过滤。
> 4. 定义了一个函数prompt_for_part_name，用于弹出对话框让用户输入零件名称，并拼接完整的文件名。
> 5. 定义了一个函数prompt_for_any_existing_part_name，用于根据当前是否在Teamcenter环境中，弹出不同的对话框让用户选择或输入零件名称。
> 6. 定义了一个函数specify_position，用于弹出对话框让用户输入位置坐标。
> 7. 定义了一个函数do_it，用于执行以下操作：调用prompt_for_any_existing_part_name让用户输入零件模板文件名。调用specify_position让用户输入位置坐标。调用UF_PLIST_create_from_template根据模板文件和位置坐标创建零件列表。
> 8. 调用prompt_for_any_existing_part_name让用户输入零件模板文件名。
> 9. 调用specify_position让用户输入位置坐标。
> 10. 调用UF_PLIST_create_from_template根据模板文件和位置坐标创建零件列表。
> 11. 定义了ufusr函数，用于初始化NX，调用do_it函数执行操作，然后终止NX。
> 12. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 整体上，这段代码通过对话框获取用户输入，然后调用NX API创建零件列表。其中使用了一些辅助函数来简化对话框操作和错误处理。
>
