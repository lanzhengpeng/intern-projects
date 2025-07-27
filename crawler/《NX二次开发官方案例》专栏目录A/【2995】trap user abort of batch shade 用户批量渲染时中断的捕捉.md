### 【2995】trap user abort of batch shade 用户批量渲染时中断的捕捉

#### 代码

```cpp
    /*HEAD TRAP_USER_ABORT_OF_BATCH_SHADE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_disp.h>  
    #include <uf_ugmgr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的函数。该函数用于打印系统日志。 */  
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
    static void build_unique_temp_name(char *fspec, int ftype)  
    {  
        char  
            *tmp_dir,  
            unique[UF_CFI_MAX_FILE_NAME_SIZE];  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4577(unique));  
        UF_CALL(uc4575(tmp_dir, ftype, unique, fspec));  
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
    /*  UF_ABORT_ask_flag_status does not work with UF_DISP_batch_shade_options  
        See PR 5766077.  This function demonstrates a work around.  
        Return:  
            TRUE = Batch shade file was created successfully  
            FALSE = Process failed or was aborted by the user  
    */  
    static logical create_batch_shade(char* filename, int x_size, int y_size,  
        UF_DISP_shade_method_t method,  UF_DISP_shade_options_p_t options)  
    {  
        int  
            missing;  
        char  
            file_ext[5],  
            temp_spec[UF_CFI_MAX_PATH_NAME_SIZE],  
            uniq_spec[UF_CFI_MAX_PATH_NAME_SIZE];  
        strcpy(file_ext, strrchr(filename, '.'));  
        build_unique_temp_name(uniq_spec, 0);  
        strcat(uniq_spec, file_ext);  
        UF_CALL(UF_CFI_ask_file_exist(uniq_spec, &missing));  
        if (!missing) uc4561(uniq_spec, 0);  
        if (UF_CALL(UF_DISP_batch_shade_options(uniq_spec, x_size, y_size,  
            method, options))) return FALSE;  
        build_similar_temp_filespec(filename, 0, temp_spec);  
        strcat(temp_spec, file_ext);  
        uc4567(uniq_spec, temp_spec, UF_CFI_MOVE_ALWAYS_REPLACE, 0, 0);  
        UF_CALL(UF_CFI_ask_file_exist(uniq_spec, &missing));  
        if (missing)  
        {  
            uc4567(temp_spec, filename, UF_CFI_MOVE_ALWAYS_REPLACE, 0, 0);  
            return TRUE;  
        }  
        else  
        {  
            uc4561(temp_spec, 0);  
            return FALSE;  
        }  
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
        UF_DISP_shade_options_t  
            shade_opts = {  
                UF_DISP_FORMAT_RASTER,      /* format 里海译:翻译：格式 */  
                UF_DISP_DISPLAY_MONOCHROME, /* display 里海译:展示 */  
                400,                        /* resolution 里海译:分辨率 */  
                UF_DISP_PLOT_FINE,          /* plot_quality 里海译:绘图质量 */  
                FALSE,                      /* generate_shadows 里海译:根据提供的上下文，"generate_shadows"翻译为"生成阴影"。 */  
                2.0,                        /* facet_quality 里海译:Facet Quality */  
                FALSE,                      /* transparent_shadows 里海译:透明阴影 */  
                TRUE,                       /* disable_raytracing 里海译:禁用光线追踪 */  
                TRUE,                       /* fixed_camera_viewing 里海译:固定相机视角 */  
                5,                          /* super_sample 里海译:super_sample 的翻译是“超采样”。 */  
                6,                          /* subdivision_depth 里海译:细分深度 */  
                128,                        /* raytrace_memory 里海译:raytrace_memory的翻译是：光线追踪内存。 */  
                15,                         /* radiosity_quality 里海译:辐射质量 */  
                FALSE,                      /* distribute_excess_light 里海译:将多余的光分布。 */  
                TRUE };                     /* use_midpoint_sampling 里海译:根据上下文，use_midpoint_sampling 的翻译为“使用中点采样”。 */  
        char  
            fspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        build_similar_filespec(fspec, 0);  
        strcat(fspec, ".jpg");  
        if (create_batch_shade(fspec, 640, 480, UF_DISP_high_quality, &shade_opts))  
        {  
            WRITE("Created ");  
            WRITE_S(fspec);  
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

> 这段代码是NX的二次开发代码，主要实现了批量渲染的功能。具体介绍如下：
>
> 1. 定义了错误报告函数report_error，用于打印UF函数调用的错误信息。
> 2. 定义了生成临时文件名的函数build_unique_temp_name和build_similar_temp_filespec，用于渲染时生成临时文件名。
> 3. 定义了创建批量渲染文件的函数create_batch_shade，调用UF_DISP_batch_shade_options函数生成渲染文件，并处理渲染过程中的用户中断。
> 4. 定义了生成类似文件名的函数build_similar_filespec，用于生成渲染文件的文件名。
> 5. 定义了写入列表窗口的函数WRITE和WRITE_S，用于在NX的列表窗口中输出信息。
> 6. 主函数do_it中设置了渲染选项，并调用create_batch_shade生成渲染文件。
> 7. ufusr是NX二次开发的入口函数，在这里调用do_it执行渲染。
> 8. ufusr_ask_unload函数设置了立即卸载二次开发模块。
>
> 总的来说，这段代码实现了NX的批量渲染功能，包括渲染选项设置、临时文件名生成、渲染过程中的用户中断处理等。
>
