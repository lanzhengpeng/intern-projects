### 【0743】create jpeg of current view and import to teamcenter 创建当前视图的JPEG并导入Teamcenter

#### 代码

```cpp
    /*  
        For the server side code, see the Teamcenter-> api article:  
            [ Sample ITK program : import jpeg from NX as new dataset ](http://solutions.industrysoftware.automation.siemens.com/view.php?si=tc_api326)  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_ugmgr.h>  
    #include <uf_disp.h>  
    #include <uf_view.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
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
    static void build_similar_temp_filespec(char *fspec, int ftype, char *new_spec)  
    {  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            temp_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            *tmp_dir;  
    // Note: when working in 4-tier mode use a different environment variable  
    // which points to a folder that both the client and server machine can access  
    // using the same path.  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4576(fspec, 0, dspec, temp_name));  
        if (strrchr(temp_name, '.')) strcpy(strrchr(temp_name, '.'), "");  
        UF_CALL(uc4575(tmp_dir, ftype, temp_name, new_spec));  
    }  
    static void do_it(void)  
    {  
        logical  
            is_ugmgr;  
        int  
            out_code = 0;  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            in_string[MAX_LINE_SIZE+1],  
            jpg_spec[MAX_FSPEC_SIZE+1],  
            *out_string = NULL,  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1],  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_is_ugmanager_active(&is_ugmgr));  
        UF_PART_ask_part_name(part, part_fspec);  
        uc6448(view_name);  
        if (!is_ugmgr)  
        {  
            UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
            UF_CALL(uc4578(part_name, 2, part_name));  
            UF_CALL(uc4575(dspec, 0, part_name, jpg_spec));  
        }  
        else  
        {  
            UF_CALL(UF_UGMGR_decode_part_filename(part_fspec, part_number,  
                part_revision, part_file_type, part_file_name));  
            build_similar_temp_filespec(part_number, 0, jpg_spec);  
        }  
        strcat(jpg_spec, "-");  
        strcat(jpg_spec, view_name);  
        strcat(jpg_spec, ".jpg");  
        UF_CALL(UF_DISP_create_image(jpg_spec, UF_DISP_JPEG, UF_DISP_ORIGINAL));  
        ECHO("JPEG file created: %s\n", jpg_spec);  
        if (is_ugmgr)  
        {  
            sprintf(in_string, "%s~%s~%s~%s", part_number, part_revision,  
                view_name, jpg_spec);  
            UF_CALL(UF_UGMGR_invoke_pdm_server(6582954, in_string, &out_code,  
                &out_string));  
            ECHO("imported to Teamcenter out_code = %d\n", out_code);  
            if (out_code == 0) uc4561(jpg_spec, 0); // it worked! delete OS file.  
            if (out_string != NULL) UF_free(out_string);  
        }  
    }  
    /* 里海 */  
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

> 这段代码是NX的二次开发代码，主要功能是从NX中导出当前显示的零件为JPEG图片，并将其导入到Teamcenter系统中。
>
> 关键步骤包括：
>
> 1. 检查当前是否在UG Manager模式下，以确定获取零件文件名的不同方式。
> 2. 构建JPEG图片的文件名，并将其保存到临时目录下。
> 3. 使用UF_DISP_create_image API将当前显示的零件导出为JPEG图片。
> 4. 如果在UG Manager模式下，调用UF_UGMGR_invoke_pdm_server API将JPEG图片导入到Teamcenter系统中。
> 5. 如果导入成功，删除本地生成的JPEG图片文件。
> 6. 在代码中使用了UF_CALL宏来简化错误处理，并使用ECHO宏来打印日志。
> 7. ufusr函数是NX二次开发的入口函数，在这里调用do_it函数执行主体逻辑。
> 8. ufusr_ask_unload函数返回立即卸载，表示该二次开发模块不需要常驻内存。
>
> 总体来说，这段代码实现了NX和Teamcenter之间的数据交互，展示了NX二次开发的基本流程。
>
