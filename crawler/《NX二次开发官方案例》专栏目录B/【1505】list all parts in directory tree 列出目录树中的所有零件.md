### 【1505】list all parts in directory tree 列出目录树中的所有零件

#### 代码

```cpp
    /*HEAD LIST_ALL_PARTS_IN_DIRECTORY_TREE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    int main( void )  
    {  
        int  
            cnt,  
            type,  
            resp;  
        char  
            dirspec[MAX_FSPEC_SIZE+1] = { "" },  
            fspec[MAX_FSPEC_SIZE+1];  
        if (UF_CALL(UF_initialize())) return 1;  
        while (printf("Enter directory path:\n") && gets(dirspec) &&  
            strlen(dirspec))  
        {  
            cnt = 0;  
            if ((resp = uc4508(dirspec, (1<<10)|(1<<11)|(1<<13), 0, "*.prt")) > 0)  
            {  
                while (((resp = uc4518()) < 2) && (resp >= 0))  
                {  
                    switch (resp)  
                    {  
                        case 0:  
                            type = uc4601();  
                            if (type == 100)  
                                UF_CALL(uc4509());  
                            else  
                            {  
                                UF_CALL(uc4519(fspec));  
                                printf("%d.  %s\n", ++cnt, fspec);  
                            }  
                            break;  
                        case 1:  
                            UF_CALL(uc4549());  
                            break;  
                        default:  
                            UF_CALL(resp);  
                            break;  
                    }  
                }  
                UF_CALL(uc4548());  
                printf("\nFound %d parts.\n", cnt);  
            }  
            else  
                UF_CALL(resp);  
        }  
        UF_CALL(UF_terminate());  
        return 0;  
    }

```

#### 代码解析

> 这段代码的主要功能是递归地遍历指定目录树，并打印出所有的prt文件路径。主要步骤如下：
>
> 1. 包含NX二次开发所需的各种头文件。
> 2. 定义了宏ECHO和UF_CALL，用于打印错误信息和调用NX API。
> 3. 定义了report_error函数，用于打印函数调用失败的错误信息。
> 4. main函数首先初始化NX环境。
> 5. 在一个循环中，提示用户输入目录路径，并调用NX API uc4508来递归遍历目录树，查找所有后缀为.prt的文件。
> 6. 在一个循环中，调用NX API uc4518获取文件列表，并根据返回值进行相应的处理：如果返回0，表示是目录，则调用uc4509进入子目录。如果返回1，表示是文件，则调用uc4519获取文件名，并打印出来。如果返回其他值，则调用report_error打印错误信息。
> 7. 如果返回0，表示是目录，则调用uc4509进入子目录。
> 8. 如果返回1，表示是文件，则调用uc4519获取文件名，并打印出来。
> 9. 如果返回其他值，则调用report_error打印错误信息。
> 10. 每找到一个文件，计数器cnt加1。
> 11. 循环结束后，调用uc4548清理环境。
> 12. 最后，打印找到的文件总数，并终止NX环境。
> 13. 整个程序通过NX提供的API实现了目录树的递归遍历，并能够打印出所有.prt文件的路径。
>
