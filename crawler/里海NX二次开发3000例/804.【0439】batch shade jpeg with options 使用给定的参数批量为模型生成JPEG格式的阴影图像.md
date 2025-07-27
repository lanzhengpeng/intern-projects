### 【0439】batch shade jpeg with options 使用给定的参数批量为模型生成JPEG格式的阴影图像

#### 代码

```cpp
    /*HEAD BATCH_SHADE_JPEG_WITH_OPTIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档描述，UF_print_syslog是V18版本中的新功能。

以下是该功能的简单翻译：

UF_print_syslog是V18版本新增的函数，用于打印系统日志信息。 */  
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
    static logical prompt_for_two_integers(char *prompt, char *item1, char *item2,  
        int *number1, int *number2)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[2][16];  
        int  
            da[2];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        menu[0][15] = '\0';  
        menu[1][15] = '\0';  
        da[0] = *number1;  
        da[1] = *number2;  
        resp = uc1607(prompt, menu, 2, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {    
        int  
            x_sz = 640,  
            y_sz = 480;  
        UF_DISP_shade_options_t  
            shade_opts;  
        char  
            fspec[UF_CFI_MAX_PATH_NAME_SIZE] = "";  
        shade_opts.resolution = 180;  
        shade_opts.plot_quality = UF_DISP_PLOT_FINE;  
        shade_opts.generate_shadows = TRUE;  
        shade_opts.facet_quality = 1.0;  
        shade_opts.transparent_shadows = FALSE;  
        shade_opts.fixed_camera_viewing = FALSE;      
        shade_opts.super_sample = 3;                 /* Super sample  
                                             1 -> 5 */  
        shade_opts.subdivision_depth = 3;            /* Subdivision depth  
                                             0 -> 6 */  
        shade_opts.raytrace_memory = 128;              /* Memory in Mb for ray tracing,  
                                             4, 8, 16, 32, 64, 128 */  
        shade_opts.radiosity_quality = 8;            /*  Radiosity quality  
                                             0 -> 15 */                                       
        shade_opts.distribute_excess_light = FALSE;  /* Distribute excess light or not? 译:分散过多光线还是不分散？ */  
        shade_opts.use_midpoint_sampling = TRUE;    /* Use midpoint sampling or not? 译:是否使用中点采样 */  
        build_similar_filespec(fspec, 4);  
        strcat(fspec, ".jpg");  
        if(prompt_for_two_integers("Enter X-width and Y-height", "X", "Y",  
            &x_sz, &y_sz))  
        {  
            UF_CALL(UF_DISP_batch_shade_options(fspec, x_sz, y_sz,  
                UF_DISP_raytrace, &shade_opts));  
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

> 这段代码是一个NX二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在函数执行出错时打印错误信息。
> 2. 定义了一个生成临时文件名的函数build_similar_temp_filespec。
> 3. 定义了一个根据当前显示部件生成相似文件名的函数build_similar_filespec。
> 4. 定义了一个提示输入两个整数的函数prompt_for_two_integers。
> 5. 定义了一个主要函数do_it，用于设置渲染参数，调用UF_DISP_batch_shade_options进行批处理渲染，并将渲染结果保存为JPG图片。
> 6. 实现了ufusr和ufusr_ask_unload两个函数，供NX调用，用于初始化、执行渲染并退出。
>
> 该代码通过NX提供的UF函数，实现了交互式设置渲染参数、批处理渲染并保存图片的功能。用户可以输入渲染图片的尺寸，选择是否开启阴影、采样质量等高级渲染选项，然后渲染当前打开的NX部件并保存为JPG图片。
>
