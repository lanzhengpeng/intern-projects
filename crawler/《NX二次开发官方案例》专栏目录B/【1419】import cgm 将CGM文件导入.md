### 【1419】import cgm 将CGM文件导入

#### 代码

```cpp
    /*HEAD IMPORT_CGM CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_std.h>  
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
    static void do_it(void)  
    {  
        int  
            resp;  
        char  
            *dir_name,  
            err_txt[MAX_LINE_SIZE+1],  
            filter[MAX_FSPEC_SIZE+1],  
            fspec[MAX_FSPEC_SIZE+1] = { "" };  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_IMPORT_DIR, &dir_name));  
        if (!strcmp(dir_name, ""))  
        {  
            uc4565(1, fspec);  
            UF_CALL(UF_UI_set_dialog_directory(UF_UI_IMPORT_DIR, fspec));  
        }  
        else  
            strcpy(fspec, dir_name);  
        UF_free(dir_name);  
        combine_directory_and_wildcard(fspec, "*.cgm", filter);  
        while (!UF_UI_create_filebox("Choose CGM to import", "Import CGM",  
            filter, "", fspec, &resp) && (resp != UF_UI_CANCEL))  
            if (uc6411( fspec, err_txt )) uc1601( err_txt, TRUE );  
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

> 这段NX二次开发代码主要实现了以下功能：
>
> 1. 错误报告：定义了report_error函数，用于报告函数调用失败时的错误信息，并输出到系统日志和对话框中。
> 2. 文件路径组合：combine_directory_and_wildcard函数将目录和通配符组合成一个完整的文件路径。
> 3. CGM文件导入：do_it函数首先获取导入目录，然后组合目录和通配符，接着使用UF_UI_create_filebox创建一个文件选择对话框，让用户选择要导入的CGM文件。如果用户取消选择，则返回。如果选择成功，则将选择的文件路径返回给fspec。
> 4. UFusr函数：这是NX二次开发程序的入口函数。首先调用UF_initialize初始化NX，然后调用do_it函数执行CGM文件导入，最后调用UF_terminate终止NX。
> 5. 卸载函数：ufusr_ask_unload函数用于在程序卸载时返回立即卸载的标志。
>
> 总体而言，这段代码实现了在NX中导入CGM文件的功能，通过创建文件选择对话框，让用户选择要导入的CGM文件，并将文件路径返回。同时，使用了错误处理和日志输出，使得程序更加健壮。
>
