### 【1522】list directory tree 列出目录树

#### 代码

```cpp
    /*HEAD LIST_DIRECTORY_TREE CCC UFUN */  
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
            if ((resp = uc4508(dirspec, (1<<10)|(1<<13), 0, NULL)) >= 0)  
            {  
                while (((resp = uc4518()) < 2) && (resp >= 0))  
                {  
                    switch (resp)  
                    {  
                        case 0:  
                            type = uc4601();  
                            if (type == 100)  
                            {  
                                UF_CALL(uc4519(fspec));  
                                printf("%d.  %s\n", ++cnt, fspec);  
                                UF_CALL(uc4509());  
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
                printf("\nFound %d directories.\n", cnt);  
            }  
            else  
                UF_CALL(resp);  
        }  
        UF_CALL(UF_terminate());  
        return 0;  
    }

```

#### 代码解析

> 这是NX二次开发代码，其主要功能是遍历目录树并列出所有子目录。
>
> 主要步骤如下：
>
> 1. 包含NX Open相关的头文件，定义错误报告宏。
> 2. main函数中初始化NX系统。
> 3. 循环提示用户输入目录路径，然后调用NX提供的函数uc4508来遍历目录树。
> 4. 在遍历过程中，根据返回值判断是目录还是文件，如果是目录，就调用uc4519获取目录名并打印，同时统计目录数量。
> 5. 遍历结束后，调用uc4548来关闭目录树，打印目录数量。
> 6. 最后终止NX系统。
>
> 该代码通过NX提供的目录树遍历函数，实现了遍历指定目录树并打印出所有子目录的功能。
>
