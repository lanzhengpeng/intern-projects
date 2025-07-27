### 【1425】import part 导入零件

#### 代码

```cpp
    /*HEAD IMPORT_PART CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_csys.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog在V18中是新增的功能。 */  
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
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            grp;  
        char  
            fspec[MAX_FSPEC_SIZE+1] = { "" };  
        double  
            dest_csys[9],  
            dest_pnt[3];  
        UF_import_part_modes_t  
            modes = { 1, 1, 0, 3, 1, FALSE, 0 };  
        ask_wcs_info(dest_pnt, dest_csys);  
        while (prompt_for_part_name("Import part",  fspec) &&  
            specify_point("Destination point for import", dest_pnt))  
        {  
            UF_CALL(UF_PART_import(fspec, &modes, dest_csys, dest_pnt, 1.0, &grp));  
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

> 这段NX二次开发代码的主要功能是实现零件的导入。以下是代码的主要内容和功能：
>
> 1. 头文件引入：引入了必要的NX Open API头文件，包括uf.h、uf_ui.h、uf_part.h等。
> 2. 错误报告函数：定义了一个错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 3. 获取当前工作坐标系信息：定义了ask_wcs_info函数，用于获取当前工作坐标系的原点和方向矩阵。
> 4. 组合目录和通配符：定义了combine_directory_and_wildcard函数，用于组合目录和通配符，以构建文件路径。
> 5. 提示输入零件名称：定义了prompt_for_part_name函数，用于提示用户输入零件名称。
> 6. 指定点坐标：定义了specify_point函数，用于提示用户指定一个点的坐标。
> 7. 执行导入：定义了do_it函数，用于循环提示输入零件名称和指定点坐标，并调用UF_PART_import函数导入零件。
> 8. UFusr函数：这是NX二次开发的标准入口函数，用于初始化NX Open API，调用do_it函数执行导入操作，然后终止NX Open API。
> 9. 卸载提示：定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总体来说，这段代码实现了零件的批量导入，用户可以循环输入零件名称和指定导入点，直到取消操作。代码结构清晰，注释详细，便于理解和维护。
>
