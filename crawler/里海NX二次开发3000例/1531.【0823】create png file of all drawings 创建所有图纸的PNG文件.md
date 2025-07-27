### 【0823】create png file of all drawings 创建所有图纸的PNG文件

#### 代码

```cpp
    /*HEAD CREATE_PNG_FILE_OF_ALL_DRAWINGS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
    #include <uf_plot.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据您的要求，以下是关于 UF_print_syslog 的翻译：

UF_print_syslog 是 V18 中新增的函数。

仅此而已，没有其他废话。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void build_temp_filespec(char *fspec, int ftype)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            *tmp_dir;  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
        UF_CALL(uc4578(part_name, 2, part_name));  
        UF_CALL(uc4575(tmp_dir, ftype, part_name, fspec));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_drawings;  
        tag_t  
            *drawings;  
        char  
            cgm_spec[UF_CFI_MAX_PATH_NAME_SIZE],  
            draw_name[MAX_ENTITY_NAME_SIZE+1],  
            export_spec[UF_CFI_MAX_PATH_NAME_SIZE],  
            png_spec[UF_CFI_MAX_PATH_NAME_SIZE],  
            temp_spec[UF_CFI_MAX_PATH_NAME_SIZE];  
        build_temp_filespec(temp_spec, 0);  
        UF_CALL(UF_DRAW_ask_drawings(&n_drawings, &drawings));  
        for (ii = 0; ii < n_drawings; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_name(drawings[ii], draw_name));  
            sprintf(export_spec, "%s-%s", temp_spec, draw_name);  
            sprintf(cgm_spec, "%s.cgm", export_spec);  
            WRITE_S(cgm_spec);  
            UF_CALL(UF_PLOT_save_cgm(drawings[ii], NULL, NULL, NULL, cgm_spec));  
            sprintf(png_spec, "%s.png", export_spec);  
            WRITE_S(png_spec);  
            UF_CALL(UF_PLOT_convert_file(cgm_spec, UF_PLOT_PNG_FORMAT, png_spec));  
        }  
        if (n_drawings > 0) UF_free(drawings);  
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
    static void check_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        for (ii=0; ii<status->n_parts; ii++)  
        {  
            UF_get_fail_message(status->statuses[ii], msg);  
            printf("    %s - %s\n", status->file_names[ii], msg);  
        }  
        UF_free(status->statuses);  
        UF_free_string_array(status->n_parts, status->file_names);  
    }  
    /*  To run in external native mode UG:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external UG/Manager or IMAN mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                printf("%d.  %s\n", ++cnt, part_name);  
                if (UF_CALL(UF_PART_open(part_name, &part, &status))  
                    || status.n_parts > 0) check_load_status(&status);  
                else  
                {  
                    do_it();  
                    UF_PART_close_all();  
                }  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是：
>
> 1. 创建一个临时目录，用于存储生成的文件。
> 2. 查询当前打开的部件中的所有绘图，并获取绘图数量和绘图对象数组。
> 3. 遍历每个绘图对象，首先将其保存为CGM格式文件，然后使用UF_PLOT_convert_file函数将CGM文件转换为PNG格式的图片文件。
> 4. 在控制台中输出CGM和PNG文件的路径。
> 5. 如果是从NX外部启动的，可以处理多个部件文件，每个文件都会调用一次do_it函数。
>
> 该代码实现了将部件中的绘图批量导出为图片文件的功能。
>
