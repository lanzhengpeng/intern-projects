### 【1759】print existing plot layout to specified printer 打印现有绘图布局到指定打印机

#### 代码

```cpp
    /*HEAD PRINT_EXISTING_PLOT_LAYOUT_TO_SPECIFIED_PRINTER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_plot.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog 是 V18 中新增的。

 */  
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
                case 1:                     /* Back 里海译:"Back"翻译为“后面”。 */  
                    if (a == 0) return -1;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 里海译:Cancel */  
                    return -1;  
                    break;  
                case 18:                    /* More... 里海译:更多... */  
                    if ((a + z) < n_choices)  
                    {  
                        a = a + 13;  
                        break;  
                    }  
                default:                    /* Picked one 里海译:Picked one */  
                    return a + resp - 5;  
                    break;  
            }  
        } while (TRUE);  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_groups,  
            n_printers,  
            n_profiles,  
            printer_n,  
            profile_n;  
        char  
            *job_name,  
            **group_names,  
            **printers,  
            **profiles;  
        UF_PLOT_printer_group_t  
            *printer_groups;  
        UF_CALL(UF_PLOT_ask_printer_groups(&n_groups, &printer_groups));  
        if (n_groups > 1)  
        {  
            UF_CALL(allocate_string_array(n_groups, UF_PLOT_MAX_STRING_SIZE,  
                &group_names));  
            for (ii = 0; ii < n_groups; ii++)  
                strcpy(group_names[ii], printer_groups[ii].label);  
            ii = choose_one_item("Select Printer Group", group_names, n_groups);  
            if (ii !=  -1)  
            {  
                UF_CALL(UF_PLOT_set_printer_group(printer_groups[ii].group_dir,  
                    printer_groups[ii].jobs_dir,  
                    printer_groups[ii].home_dir));  
            }  
            UF_free_string_array(n_groups, group_names);  
        }  
        if (n_groups > 0) UF_free(printer_groups);  
        UF_CALL(UF_PLOT_ask_printer_names(&n_printers, &printers));  
        if (n_printers == 0)  
        {  
            uc1601("No printers found.", TRUE);  
            return;  
        }  
        if (n_printers > 1)  
            printer_n = choose_one_item("Select Printer", printers, n_printers);  
        else  
            printer_n = 0;  
        UF_CALL(UF_PLOT_ask_profile_names(printers[printer_n], &n_profiles,  
            &profiles));  
        if (n_profiles > 1)  
            profile_n = choose_one_item("Select Profile", profiles, n_profiles);  
        else  
            profile_n = 0;  
        UF_CALL(UF_PLOT_ask_default_job_name(NULL_TAG, &job_name));  
        WRITE_S(job_name);  
        WRITE_S(printers[printer_n]);  
        WRITE_S(profiles[profile_n]);  
    /*  This will return an error code 11 "Internal error: memory access violation"  
        when there are no plots already added to the plot layout.  
        See PR 1554359.  
    */  
        UF_CALL(UF_PLOT_print_plot_layout(job_name, NULL, printers[printer_n],  
            profiles[profile_n], 1));  
        UF_free_string_array(n_printers, printers);  
        UF_free_string_array(n_profiles, profiles);  
        UF_free(job_name);  
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

> 这段NX二次开发代码主要用于实现以下功能：
>
> 1. 获取当前NX中的所有打印机组，并让用户选择一个打印机组。
> 2. 获取当前选择的打印机组中的所有打印机，并让用户选择一个打印机。
> 3. 获取当前选择的打印机中的所有打印配置文件，并让用户选择一个配置文件。
> 4. 获取默认的打印作业名称。
> 5. 将当前NX中的绘图布局打印到用户选择的打印机上，并使用用户选择的配置文件。
>
> 主要步骤包括：
>
> 1. 使用UF_PLOT_ask_printer_groups()函数获取所有打印机组信息。
> 2. 使用UF_PLOT_ask_printer_names()函数获取当前打印机组中的所有打印机名称。
> 3. 使用UF_PLOT_ask_profile_names()函数获取当前打印机中的所有配置文件名称。
> 4. 使用UF_PLOT_ask_default_job_name()函数获取默认的打印作业名称。
> 5. 使用UF_PLOT_print_plot_layout()函数将绘图布局打印到选择的打印机上。
>
> 此外，代码还包含一些错误处理和内存分配释放的辅助函数。
>
> 总体而言，该代码实现了在NX中选择打印机组、打印机、配置文件，并将绘图布局打印到指定打印机的功能。
>
