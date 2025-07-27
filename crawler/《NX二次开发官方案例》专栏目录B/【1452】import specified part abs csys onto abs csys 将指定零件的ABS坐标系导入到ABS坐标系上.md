### 【1452】import specified part abs csys onto abs csys 将指定零件的ABS坐标系导入到ABS坐标系上

#### 代码

```cpp
    /*  
    UF_PART_import aligns the WCS in the imported part to the specified destination.    
    This example demonstrates importing the part so that its absolute coordinate system comes in at and aligned  
    with the work part's absolute coordinate system regardless of where its WCS was when it was saved.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_csys.h>  
    #include <uf_obj.h>  
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
            *choice = resp - 4;  /* returns 1 or 2 里海译:翻译为：返回1或2。 */  
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
    static tag_t ask_parts_wcs(tag_t part)  
    {  
        int  
            this_type,  
            this_subtype;  
        tag_t  
            csys = NULL_TAG;  
        while ((csys = UF_OBJ_cycle_all(part, csys)) && (csys != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(csys, &this_type, &this_subtype));  
            if ((this_type == UF_coordinate_system_type) &&  
                (this_subtype == UF_csys_wcs_subtype)) return csys;  
        }  
        return NULL_TAG;  //can't happen  
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
    static void do_it(void)  
    {  
        tag_t  
            grp,  
            matrix,  
            other_wcs = NULL_TAG,  
            part = NULL_TAG;  
        char  
            fspec[MAX_FSPEC_SIZE+1] = { "" };  
        double  
            dest_csys[9],  
            dest_pnt[3];  
        UF_import_part_modes_t  
            modes = { 1, 1, 0, 3, 1, FALSE, 0 };  
        UF_PART_load_status_t  
            error_status;  
        if (prompt_for_any_existing_part_name("Import part ABS - ABS",  fspec))  
        {  
            UF_CALL(UF_PART_open_quiet(fspec, &part, &error_status));  
            if (error_status.n_parts > 0) report_load_status(&error_status);  
            if (part == NULL_TAG) return;  
            other_wcs = ask_parts_wcs(part);  
            UF_CALL(UF_CSYS_ask_csys_info(other_wcs, &matrix, dest_pnt));  
            UF_CALL(UF_CSYS_ask_matrix_values(matrix, dest_csys));  
            UF_CALL(UF_PART_import(fspec, &modes, dest_csys, dest_pnt, 1.0, &grp));  
            UF_CALL(UF_PART_close(part, 0, 1));  
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

> 根据代码注释和内容，这是一段用于NX的二次开发代码，主要用于导入一个部件，并将该部件的绝对坐标系对齐到工作部件的绝对坐标系。主要步骤如下：
>
> 1. 定义了一个名为ECHO的函数，用于输出信息。
> 2. 定义了一个名为report_error的函数，用于输出错误信息。
> 3. 定义了一个名为pick_one_of_two_choices的函数，用于让用户从两个选项中选择一个。
> 4. 定义了一个名为combine_directory_and_wildcard的函数，用于组合目录和通配符。
> 5. 定义了一个名为prompt_for_part_name的函数，用于提示用户输入部件名称。
> 6. 定义了一个名为prompt_for_any_existing_part_name的函数，用于提示用户选择本地或TcEng中的部件。
> 7. 定义了一个名为ask_parts_wcs的函数，用于获取部件的WCS坐标系。
> 8. 定义了一个名为report_load_status的函数，用于输出加载状态。
> 9. 定义了一个名为do_it的函数，用于执行导入部件并坐标系对齐。
> 10. 定义了UFusr函数，作为程序入口。
> 11. 定义了ufusr_ask_unload函数，用于卸载。
>
> 总的来说，这段代码实现了NX部件的导入和对齐，包含用户交互、坐标系转换、错误处理等功能。
>
