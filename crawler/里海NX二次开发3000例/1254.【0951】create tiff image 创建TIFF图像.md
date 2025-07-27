### 【0951】create tiff image 创建TIFF图像

#### 代码

```cpp
    /*HEAD CREATE_TIFF_IMAGE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 版本中的新功能，用于打印系统日志。 */  
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
            tfn[UF_CFI_MAX_PATH_NAME_SIZE];  
        build_similar_filespec(tfn, 36);  
        UF_CALL(UF_DISP_create_image(tfn, UF_DISP_TIFF, UF_DISP_ORIGINAL));  
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

> 这段代码是一个NX二次开发示例，用于创建当前显示部件的TIFF图像文件。
>
> 主要功能包括：
>
> 1. 初始化和终止NX环境。
> 2. 获取当前显示部件的名称，并构建一个类似的文件名。
> 3. 使用NX的UF_DISP_create_image函数，创建当前显示部件的TIFF图像文件。
> 4. 在函数内部处理UF函数的错误，并打印错误信息。
> 5. 提供一个ufusr函数作为二次开发的入口点。
> 6. 提供一个ufusr_ask_unload函数，表示二次开发模块不需要卸载。
>
> 通过这段代码，用户可以快速生成当前显示部件的图像文件，便于保存或分享。代码结构清晰，注释详细，是一个很好的NX二次开发入门示例。
>
