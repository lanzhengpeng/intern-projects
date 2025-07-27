### 【0401】batch shade 批处理阴影

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
    #include <uf_disp.h>  
    #include <uf_ugmgr.h>  
    #include <uf_view.h>  
    #include <uf_csys.h>  
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
    static tag_t set_wcs_to_current_view(void)  
    {  
        tag_t  
            csys,                       /* Csys object  译:翻译Csys object，即坐标系对象，是指定义和存储坐标系信息的对象。在计算机图形学和CAD软件中，坐标系对象用于描述图形元素的相对位置和方向，提供一种标准化的方法来表示图形对象在三维空间中的位置和方向。 */  
            matobj,                     /* Matrix object 译:矩阵对象 */  
            orig_wcs;  
        double  
            origin[3],                  /* View's center 译:View的中心 */  
            matrix[9],                  /* View's matrix 译:View的矩阵 */  
            scale;  
        UF_CALL(UF_CSYS_ask_wcs(&orig_wcs));  
        UF_CALL(uc6430("", origin, &scale));  
        UF_CALL(uc6433("", matrix));  
        if (!UF_CALL(UF_CSYS_create_matrix(matrix, &matobj)) &&  
            !UF_CALL(UF_CSYS_create_temp_csys(origin, matobj, &csys)))  
            UF_CALL(UF_CSYS_set_wcs(csys));  
        return orig_wcs;  
    }  
    static void do_it(int x_size, int y_size)  
    {  
        char  
            fspec[MAX_FSPEC_SIZE+1];  
        build_similar_filespec(fspec, 0);  // no extension  
        sprintf(fspec, "%s_%dX%d.jpg", fspec, x_size, y_size);  
        UF_CALL(uc6432("", 2));  
        set_wcs_to_current_view();  
        ECHO("\tcreating %s...\n", fspec);  
        UF_CALL(UF_DISP_batch_shade(fspec, x_size, y_size, UF_DISP_gouraud));  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it(500,500);  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            ECHO("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                ECHO("  ");  
                ECHO(status->file_names[ii]);  
                ECHO(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                ECHO(msg);  
                ECHO("\n");  
            }  
            ECHO("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    /*  To run in external native mode NX:  
            program.exe x_size y_size /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external NX Manager mode:  
            program.exe -pim=yes x_size y_size @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0,  
            x_size,  
            y_size;  
        tag_t  
            part;  
        char  
            xpixels[MAX_FSPEC_SIZE+1],  
            ypixels[MAX_FSPEC_SIZE+1],  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            if ((uc4621(xpixels) == 1) && (uc4621(ypixels) == 1))  
            {  
                sscanf(xpixels, "%d", &x_size);  
                sscanf(ypixels, "%d", &y_size);  
                while (uc4621(part_name) == 1)  
                {  
                    ECHO("%d.  %s\n", ++cnt, part_name);  
                    UF_CALL(UF_PART_open(part_name, &part, &status));  
                    if (status.n_parts > 0) report_load_status(&status);  
                    if (!status.failed)  
                    {  
                        do_it(x_size, y_size);  
                        UF_CALL(UF_PART_close_all());  
                    }  
                }  
                ECHO("\nProcessed %d parts.\n", cnt);  
            }  
            else  
            {  
                ECHO("usage: %s <x_pixel_cnt> <y_pixel_cnt> <part1> <part2> ...", argv[0]);  
                ECHO("The batch view has an x/y ratio of 1.0706 so for best results use a width and height with similar ratio.");  
            }  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++程序，主要实现了以下功能：
>
> 1. 初始化和清理：在main函数中，使用UF_initialize()和UF_terminate()来初始化和清理NX Open环境。
> 2. 参数解析：通过命令行参数解析图像尺寸和零件路径，并调用do_it()函数进行渲染。
> 3. 零件加载：使用UF_PART_open()函数加载零件，并记录加载状态。
> 4. 设置视图：调用set_wcs_to_current_view()函数设置坐标系到当前视图。
> 5. 图像渲染：使用UF_DISP_batch_shade()函数批量渲染零件图像，并保存为JPEG格式。
> 6. 错误报告：定义了report_error()函数用于输出错误信息。
> 7. 相似文件名生成：定义了build_similar_filespec()和build_similar_temp_filespec()函数用于生成相似的文件名。
> 8. 用户函数入口：定义了ufusr()函数作为用户函数入口。
> 9. 卸载请求：定义了ufusr_ask_unload()函数用于立即卸载用户函数。
> 10. 加载状态报告：定义了report_load_status()函数用于报告零件加载状态。
>
> 综上所述，这个程序实现了通过命令行参数加载零件、设置视图、批量渲染图像并保存为JPEG文件的功能，适用于自动化渲染NX零件图像的场景。
>
