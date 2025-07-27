### 【1218】export jpeg of current shaded display 将当前阴影显示导出为JPEG图片

#### 代码

```cpp
    /*HEAD EXPORT_JPEG_OF_CURRENT_SHADED_DISPLAY CCC UFUN */  
    /*  
        The methods used here will include any shading which is being shown in  
        the current display, however it is not a requirement that the current  
        display be shaded for the program to work.  The method used here works  
        in internal mode only.  If shading is not needed and external/batch mode  
        is preferred use UF_CGM_export_cgm and UF_PLOT_convert_file instead.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_ugmgr.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据上下文，UF_print_syslog 在 V18 中是一个新增的函数。 */  
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
    static void build_similar_filespec(char *fspec, int ftype)  
    {  
        logical  
            is_ugmgr;  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1];  
        UF_CALL(UF_is_ugmanager_active(&is_ugmgr));  
        UF_PART_ask_part_name(part, part_fspec);  
        if (!is_ugmgr)  
        {  
            UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
            UF_CALL(uc4578(part_name, 2, part_name));  
            UF_CALL(uc4575(dspec, ftype, part_name, fspec));  
        }  
        else  
        {  
            UF_CALL(UF_UGMGR_decode_part_filename(part_fspec, part_number,  
                part_revision, part_file_type, part_file_name));  
            build_similar_temp_filespec(part_number, ftype, fspec);  
        }  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static void do_it(void)  
    {  
        char  
            jpg_spec[MAX_FSPEC_SIZE+1];  
        build_similar_filespec(jpg_spec, 0);  
        strcat(jpg_spec, ".jpg");  
        UF_CALL(UF_DISP_create_image(jpg_spec, UF_DISP_JPEG, UF_DISP_ORIGINAL));  
        WRITE("JPEG file created ");  
        WRITE(jpg_spec);  
        WRITE("\n");  
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

> 这段NX二次开发代码实现了导出当前显示的图形为JPEG图片的功能。具体来说，主要功能包括：
>
> 1. 定义了错误报告函数report_error，用于打印出函数调用失败时的错误信息。
> 2. 定义了生成临时文件名的函数build_similar_temp_filespec，以及生成类似文件名的函数build_similar_filespec，用于根据当前显示的部件名称生成JPEG图片的文件名。
> 3. 定义了主函数do_it，用于创建当前显示的JPEG图片并保存。
> 4. 定义了ufusr函数，初始化NX后调用do_it函数，并在完成后终止NX。
> 5. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示NX会话结束后立即卸载二次开发代码。
>
> 总体来说，这段代码实现了在NX中导出当前显示的图形为JPEG图片的功能，并具有错误报告和文件名生成等辅助功能。
>
