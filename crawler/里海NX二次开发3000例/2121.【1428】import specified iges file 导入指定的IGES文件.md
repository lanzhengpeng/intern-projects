### 【1428】import specified iges file 导入指定的IGES文件

#### 代码

```cpp
    /*HEAD IMPORT_SPECIFIED_IGES_FILE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_disp.h>  
    #include <uf_cfi.h>  
    #include <uf_assem.h>  
    #include <uf_csys.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中的新功能。 */  
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
    static void build_similar_temp_filespec(char *fspec, int ftype, char *new_spec)  
    {  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            temp_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            *tmp_dir;  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4576(fspec, 0, dspec, temp_name));  
        if (strrchr(temp_name, '.')) strcpy(strrchr(temp_name, '.'), "");  
        UF_CALL(uc4575(tmp_dir, ftype, temp_name, new_spec));  
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
    static void do_it(void)  
    {  
        int  
            resp;  
        tag_t  
            grp,  
            disp_part = UF_PART_ask_display_part(),  
            work_part = UF_ASSEM_ask_work_part();  
        double  
            dest_csys[9],  
            dest_pnt[3];  
        char  
            cmd[UF_UI_MAX_STRING_LEN+1],  
            *pfn,  
            filter[MAX_FSPEC_SIZE+1] = { "*.igs" },  
            lfn[MAX_FSPEC_SIZE+1],  
            ifn[MAX_FSPEC_SIZE+1],  
            tfn[MAX_FSPEC_SIZE+1];  
        UF_import_part_modes_t  
            modes = { 0, 1, 0, 0, 0, FALSE, 0 };  
    #if defined(_WIN32)  
        pfn = "%UGII_BASE_DIR%\\IGES\\iges.cmd";  
    #else  
        pfn = "$UGII_BASE_DIR/iges/iges";  
    #endif  
        while (!UF_CALL(UF_UI_create_filebox("Specify IGES file to import",  
                "IGES file", filter, "", ifn, &resp)) && (resp != UF_UI_CANCEL))  
        {  
            build_similar_temp_filespec(ifn, 47, lfn);  
            build_similar_temp_filespec(ifn, 2, tfn);  
            sprintf(cmd, "%s o=%s d=igesimport.def l=%s %s", pfn, tfn, lfn, ifn);  
            system(cmd);  
            ask_wcs_info(dest_pnt, dest_csys);  
            if (work_part != disp_part)  
                UF_CALL(UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, dest_pnt,  
                    UF_CSYS_WORK_COORDS, dest_pnt));  
            UF_CALL(UF_PART_import(tfn, &modes, dest_csys, dest_pnt, 1.0, &grp));  
            UF_CALL(uc4561(tfn, 2));  
            UF_CALL(uc4561(lfn, 47));  
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

> 这是段NX二次开发代码，主要实现从IGES文件导入零件的功能。主要步骤包括：
>
> 1. 定义错误报告函数report_error，用于打印错误信息。
> 2. 定义构建临时文件路径的函数build_similar_temp_filespec。
> 3. 定义获取当前工作坐标系信息的函数ask_wcs_info。
> 4. 主函数do_it()中，通过对话框让用户指定要导入的IGES文件。
> 5. 调用构建临时文件路径的函数，生成临时文件路径。
> 6. 调用iges外部命令，将IGES文件转换为NX可识别的临时文件。
> 7. 获取当前工作坐标系信息。
> 8. 如果显示坐标系和工作坐标系不一致，进行坐标系转换。
> 9. 调用UF_PART_import()函数，以临时文件路径为参数，导入零件。
> 10. 删除临时文件。
> 11. ufusr()函数中初始化NX环境，调用do_it()函数，然后终止NX环境。
> 12. ufusr_ask_unload()函数返回立即卸载。
>
> 总体来说，这段代码通过交互式对话框让用户指定IGES文件，然后调用外部命令和NX API实现零件的导入，并进行了错误处理和坐标系转换。
>
