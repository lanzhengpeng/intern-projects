### 【1231】export tiff in external mode 在UG外部模式下导出TIFF图像

#### 代码

```cpp
    /*HEAD EXPORT_TIFF_IN_EXTERNAL_MODE CCC UFUN */  
    /*  Here is a one line "tiff.htt" which will generate a standard TIFF  
        image in the same directory and having the same name as the part.  
        This will work for JPEG and GIF too, just change the file extension  
        to .jpg or .gif:  
    <@UGSHADE FILE="<@UGVAR FORMAT="$HSPEC">.tif">  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_web.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中内容，UF_print_syslog 是 V18 版本新增的功能。 */  
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
    static void build_similar_filespec(char *fspec, int ftype)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
        UF_CALL(uc4578(part_name, 2, part_name));  
        UF_CALL(uc4575(dspec, ftype, part_name, fspec));  
    }  
    static void do_it(void)  
    {  
        char  
            fspec[MAX_FSPEC_SIZE+1];  
        build_similar_filespec(fspec, 0);  
        UF_CALL(UF_WEB_author_html(fspec, "/full/path/to/tiff.htt", FALSE));  
    /*  Delete empty html file 里海译:删除空白的HTML文件 */  
        UF_CALL(uc4561(fspec, 0));  
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

> 这段NX二次开发代码主要包括以下功能：
>
> 1. 错误报告函数：用于在发生错误时输出错误信息，包括文件名、行号、错误代码和错误信息。
> 2. 生成类似文件名函数：根据当前打开的部件名称，生成一个类似的文件名，用于后续保存图像。
> 3. 保存图像函数：使用NX提供的UF_WEB_author_html函数，根据指定的HTML模板，生成一个标准的TIFF图像文件，并将图像保存到与当前部件同一目录下，文件名与部件名类似。
> 4. 删除空白HTML文件：如果生成的HTML文件为空，则删除该文件。
> 5. UFusr函数：NX二次开发的标准入口函数，用于初始化和调用保存图像函数，最后终止NX。
> 6. 检查加载状态函数：用于输出加载部件时的状态信息。
> 7. 主函数：处理命令行参数，打开指定部件，调用保存图像函数，并输出处理部件数量。
>
> 总体来说，这段代码实现了根据NX部件生成标准TIFF图像文件的功能，并提供了命令行接口。
>
