### 【1451】import specified jt file 导入指定的JT文件

#### 代码

```cpp
    /*HEAD IMPORT_SPECIFIED_JT_FILE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_csys.h>  
    #include <uf_assem.h>  
    #include <uf_cfi.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n*** ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 的新功能。 */  
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
    static void ask_wcs_info(double origin[3], double axes[9])  
    {  
        tag_t  
            wcs,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, axes));  
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
    static logical prompt_for_jt_file_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[MAX_FSPEC_SIZE+1],  
            *p_dir;  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_PART_DIR, &p_dir));  
        combine_directory_and_wildcard(p_dir, "*.jt", filter);  
        UF_free(p_dir);  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "JT File Name", filter, "",  
            fspec, &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            grp,  
            disp_part = UF_PART_ask_display_part(),  
            work_part = UF_ASSEM_ask_work_part();  
        char  
            fspec[UF_CFI_MAX_PATH_NAME_SIZE] = { "" };  
        double  
            dest_csys[9],  
            dest_pnt[3];  
        UF_import_part_modes_t  
            modes = { 0, 1, 0, 0, 0, FALSE, 0 };  
        ask_wcs_info(dest_pnt, dest_csys);  
        if (work_part != disp_part)  
            UF_CALL(UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, dest_pnt,  
            UF_CSYS_WORK_COORDS, dest_pnt));  
        if (prompt_for_jt_file_name("Import", fspec))  
            UF_CALL(UF_PART_import(fspec, &modes, dest_csys, dest_pnt, 1.0, &grp));  
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

> 这是段NX二次开发代码，主要实现以下功能：
>
> 1. 错误报告机制：定义了一个错误报告函数report_error，用于在函数调用失败时记录错误信息。
> 2. 获取工作坐标系信息：定义了一个获取当前工作坐标系信息的函数ask_wcs_info，包括原点和坐标系矩阵。
> 3. 组合目录和通配符：定义了一个函数combine_directory_and_wildcard，用于组合目录和文件名通配符。
> 4. 提示用户输入JT文件：定义了一个函数prompt_for_jt_file_name，用于提示用户输入JT文件名。
> 5. 执行导入操作：定义了一个函数do_it，用于获取当前工作坐标系信息，提示用户输入JT文件，并调用UF_PART_import函数进行导入。
> 6. 主函数：定义了主函数ufusr，用于初始化NX，调用do_it函数，然后终止NX。
> 7. 卸载函数：定义了一个卸载函数ufusr_ask_unload，用于设置卸载方式。
>
> 总体来说，该代码实现了在NX中导入JT文件的功能，包括错误处理、坐标系转换、文件选择提示等。
>
