### 【1420】import color table from another open part 从另一个打开的部件导入颜色表

#### 代码

```cpp
    /*HEAD IMPORT_COLOR_TABLE_FROM_ANOTHER_OPEN_PART CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_error_bases.h>  
    /*  
        This program can be used to copy a color table from one part to another.  
        UF_DISP_ask_color returns localized color names, so the copied color  
        names may be converted to the local language and not match the names in  
        the other part.  
    */  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    static int ask_all_parts(tag_t **parts)  
    {  
        int  
            err,  
            ii,  
            n = UF_PART_ask_num_parts();  
        if (n > 0)  
        {  
            *parts = UF_allocate_memory(n * sizeof(tag_t), &err);  
            if (UF_CALL(err)) return 0;  
            for (ii = 0; ii < n; ii++)  
            {  
                (*parts)[ii] = UF_PART_ask_nth_part(ii);  
            }  
        }  
        return n;  
    }  
    static int allocate_string_array(int n_elements, int n_chars, char ***where)  
    {  
        int  
            ii,  
            resp;  
        *where = UF_allocate_memory(n_elements * sizeof(char *), &resp);  
        for (ii = 0; ii < n_elements; ii++)  
        {  
            (*where)[ii] = UF_allocate_memory(n_chars * sizeof(char), &resp);  
            if (resp) return resp;  
        }  
        return 0;  
    }  
    static void ask_part_shortname(tag_t part, char *shortname)  
    {  
        logical  
            nxman;  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_PART_ask_part_name(part, partname));  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (nxman)  
            UF_CALL(UF_PART_name_for_display(partname, shortname));  
        else  
            UF_CALL(uc4574(partname, 2, shortname));  
    }  
    static int choose_one_item(char *prompt, char **choices, int n_choices)  
    {  
        int  
            a = 0,  
            ii,  
            resp,  
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
                strcpy(&options[13][0], "More...");  
            switch (resp = uc1603(prompt, 0, options, z))  
            {  
                case 1:                     /* Back 里海译:Back的翻译是“背部”。 */  
                    if (a == 0) return -1;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 里海译:"取消" */  
                    return -1;  
                    break;  
                case 18:                    /* More... 里海译:More... */  
                    if ((a + z) < n_choices)  
                    {  
                        a = a + 13;  
                        break;  
                    }  
                default:                    /* Picked one 里海译:The translation of "Picked one" is "选择一个". */  
                    return a + resp - 5;  
                    break;  
            }  
        } while (TRUE);  
    }  
    static tag_t choose_loaded_part(char *prompt)  
    {  
        int  
            ii,  
            n_parts,  
            which;  
        tag_t  
            the_part,  
            *parts;  
        char  
            **leaf_names;  
        n_parts = ask_all_parts(&parts);  
        UF_CALL(allocate_string_array(n_parts, UF_CFI_MAX_FILE_NAME_LEN,  
            &leaf_names));  
        for (ii = 0; ii < n_parts; ii++)  
            ask_part_shortname(parts[ii], leaf_names[ii]);  
        if ((which = choose_one_item(prompt, leaf_names, n_parts)) == -1)  
            the_part = NULL_TAG;  
        else  
            the_part = parts[which];  
        UF_free(parts);  
        UF_free_string_array(n_parts, leaf_names);  
        return the_part;  
    }  
    #include <uf_undo.h>  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void do_it(void)  
    {  
        int  
            cnt,  
            ii;  
        tag_t  
            from_part,  
            to_part = UF_PART_ask_display_part();  
        double  
            clr_values[3];  
        char  
            *color_name,  
            msg[MAX_LINE_SIZE+1];  
        if ((from_part = choose_loaded_part("Copy color table from")) != NULL_TAG)  
        {  
            set_undo_mark("Import color table");  
            UF_CALL(UF_DISP_set_display(UF_DISP_SUPPRESS_DISPLAY));  
            UF_CALL(UF_PART_set_display_part(from_part));  
            UF_CALL(UF_DISP_ask_color_count(&cnt));  
            for (ii = UF_OBJ_MIN_COLOR; ii < cnt; ii++)  
            {  
                UF_CALL(UF_DISP_ask_color(ii, UF_DISP_rgb_model, &color_name,  
                    clr_values));  
                if (strlen(color_name) > UF_DISP_MAX_NAME_SIZE)  
                {  
                    sprintf(msg, "color # %d name \"%s\" shortened to ",  
                        ii, color_name);  
                    strcpy(&color_name[UF_DISP_MAX_NAME_SIZE-1], ".");  
                    strcat(msg, color_name);  
                    strcat(msg, "\n");  
                    ECHO(msg);  
                }  
                UF_CALL(UF_PART_set_display_part(to_part));  
                UF_CALL(UF_DISP_set_color(ii, UF_DISP_rgb_model, color_name,  
                    clr_values));  
                UF_free(color_name);  
                if (ii < cnt-1) UF_CALL(UF_PART_set_display_part(from_part));  
            }  
            UF_CALL(UF_DISP_set_display(UF_DISP_UNSUPPRESS_DISPLAY));  
            UF_CALL(UF_DISP_regenerate_display());  
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

> 这段代码是NX二次开发的UFUN，主要功能是从一个NX部件复制颜色表到另一个NX部件。具体包括以下步骤：
>
> 1. 包含NX二次开发常用的头文件，定义了错误处理宏和日志打印宏。
> 2. 定义了获取所有加载部件的函数，并获取部件的简称。
> 3. 定义了一个函数用于在多个选项中选择一个，用于选择源部件和目标部件。
> 4. 定义了设置撤销标记的函数。
> 5. do_it函数是主功能实现，首先选择源部件，然后获取源部件的颜色表，将颜色表复制到目标部件。复制时会对颜色名称进行截断处理，并打印日志。
> 6. ufusr是UFUN的入口函数，调用do_it函数实现功能。
> 7. ufusr_ask_unload函数用于立即卸载UFUN。
>
> 总的来说，这段代码实现了NX部件之间颜色表的复制功能，通过选择源部件和目标部件，将源部件的颜色表复制到目标部件，并在复制过程中进行错误处理和日志打印。
>
