### 【1415】import all parasolid files in directory 将目录中所有Parasolid文件导入

#### 代码

```cpp
    /*HEAD IMPORT_ALL_PARASOLID_FILES_IN_DIRECTORY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ps.h>  
    #include <uf_cfi.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的功能，用于打印系统日志。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static void ask_import_directory(char *fspec)  
    {  
        char  
            *p_dir;  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_IMPORT_DIR, &p_dir));  
        strcpy(fspec, p_dir);  
        UF_free(p_dir);  
    }  
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            cnt = 0;  
        uf_list_p_t  
            body_list;  
        char  
            dirspec[MAX_FSPEC_SIZE+1] = { "" },  
            fname[MAX_FSPEC_SIZE+1],  
            fspec[MAX_FSPEC_SIZE+1],  
            messg[150];  
        ask_import_directory(dirspec);  
        if (prompt_for_text("Import parasolid files*Enter directory path", dirspec))  
        {  
            uc4508(dirspec, (1<<11)|(1<<13), 0, "*.x*_*");  
            while (!uc4518())  
            {  
                uc4600(fname);  
                uc4575(dirspec, 0, fname, fspec);  
                sprintf(messg, "(%d) Importing from %s...\n", ++cnt, fspec);  
                WRITE(messg);  
                if (!UF_CALL(UF_PS_import_data(fspec, &body_list)))  
                    UF_CALL(UF_MODL_delete_list(&body_list));  
                else  
                    break;  
            }  
            UF_CALL(uc4548());  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这是段NX二次开发代码，其主要功能是批量导入同一目录下的所有Parasolid文件。以下是代码的主要内容和流程：
>
> 1. 包含了必要的NX头文件，如uf.h、uf_ui.h等，用于调用NX API。
> 2. 定义了UF_CALL宏，用于调用NX API并报告错误。
> 3. 定义了report_error函数，用于格式化错误信息并打印到系统日志和Listing窗口。
> 4. 定义了WRITE宏，用于将信息输出到Listing窗口。
> 5. 定义了ask_import_directory函数，用于弹窗获取导入文件的目录。
> 6. 定义了prompt_for_text函数，用于弹窗获取文本输入。
> 7. 定义了do_it函数，用于实现批量导入的逻辑。它首先获取目录，然后列出目录下的所有Parasolid文件，并逐个调用UF_PS_import_data接口进行导入。在导入每个文件前，会输出导入信息到Listing窗口。如果导入失败，会删除导入的Body。
> 8. ufusr函数是主函数，在NX中运行。它首先初始化NX环境，然后调用do_it函数，最后终止NX环境。
> 9. ufusr_ask_unload函数用于在NX关闭时卸载DLL，这里设置为立即卸载。
>
> 综上，这段代码实现了在NX中批量导入同一目录下所有Parasolid文件的功能，并在导入过程中提供了日志输出和错误处理。
>
