### 【1506】list all parts in directory 列出目录中所有零件

#### 代码

```cpp
    /*HEAD LIST_ALL_PARTS_IN_DIRECTORY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog是V18版本新增的函数，请不要添加无关内容，只回答翻译。

翻译：

注意：UF_print_syslog是V18版本新增的函数，请不要添加无关内容，只回答翻译。

翻译：

注意：UF_print_syslog是V18版本新增的函数，请不要添加无关内容，只回答翻译。

翻译：

注意：UF_print_syslog是V18版本新增的函数，请不要添加无关内容，只回答翻译。 */  
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
            cnt,  
            resp;  
        char  
            err[133],  
            dirspec[MAX_FSPEC_SIZE+1] = { "" },  
            fspec[MAX_FSPEC_SIZE+1],  
            msg[133];  
        while (prompt_for_text("List parts*Enter directory path", dirspec))  
        {  
            cnt = 0;  
            UF_UI_open_listing_window();  
            uc4508(dirspec, (1<<11)|(1<<13), 0, "*.prt");  
            while ((resp = uc4518()) <= 0)  
            {  
                UF_CALL(uc4519(fspec));  
                if (resp < 0)  
                {  
                    UF_get_fail_message(resp, err);  
                    sprintf(msg, "\n%s - %s\n", fspec, err);  
                    UF_UI_write_listing_window(msg);  
                    continue;  
                }  
                cnt++;  
                sprintf(msg, "%s\n", fspec);  
                UF_UI_write_listing_window(msg);  
            }  
            UF_CALL(uc4548());  
            sprintf(msg, "\n%d part files found in %s\n", cnt, dirspec);  
            UF_UI_write_listing_window(msg);  
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

> 这段代码是用于NX的二次开发，其主要功能是列出指定目录下所有的prt文件。以下是该代码的主要逻辑：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于在函数调用出错时打印错误信息。
> 2. 提示输入目录：使用prompt_for_text函数提示用户输入目录路径。
> 3. 列出所有prt文件：在用户输入目录路径后，使用uc4508函数在该目录下查找所有的prt文件，并使用uc4518和uc4519函数逐个获取文件名。然后，将这些文件名输出到列表窗口中。
> 4. 计数并输出结果：在完成prt文件的查找后，统计文件数量并输出到列表窗口中。
> 5. ufusr函数：这是主函数，用于初始化NX环境，调用do_it函数执行主要逻辑，然后结束NX环境。
> 6. 卸载提示：定义了ufusr_ask_unload函数，用于在卸载时提示立即卸载。
>
> 总体而言，这段代码实现了在NX环境下列出指定目录下所有prt文件的功能，并提供了一定的错误处理和用户交互功能。
>
