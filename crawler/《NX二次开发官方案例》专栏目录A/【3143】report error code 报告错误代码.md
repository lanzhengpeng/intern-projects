### 【3143】report error code 报告错误代码

#### 代码

```cpp
    /*****************************************************************************  
    **  
    ** report_error_code.c  
    **  
    ** Description:  
    **    Main file for the application.  
    **  
    *****************************************************************************/  
    /* Include files 里海译:Include files翻译为"包含文件"。 */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>    
    #include <uf.h>  
    void replace_char_from_string(char from, char to, char *str)   
    {  
        int i = 0;  
        int len = (int)strlen(str)+1;  
        for(i=0; i<len; i++)  
        {  
            if(str[i] == from)  
            {  
                str[i] = to;  
            }  
        }   
    }    
    static int report_error( int irc)  
    {  
        int ret;  
        if (irc)  
        {  
            char err[1000];  
            ret = (UF_get_fail_message(irc, err));  
            if(ret == 0)  
            {  
                // long form  
                // printf("*** ERROR code %d:\n", irc);  
                // printf("+++ %s\n\n", err);  
                // short single line without LF/CR   
                replace_char_from_string('\n',' ',err);  
                replace_char_from_string('\r',' ',err);  
                printf("%d | %s\n", irc, err);  
            }  
        }  
        return(irc);  
    }  
    /* Main routine 里海译:主程序 */  
    extern void main( char argc, char *argv[] )  
    {  
        int min=0, max=0, ii;  
        /* Initialize the API environment 里海译:初始化API环境 */  
        if( UF_initialize() )   
        {  
            /* Failed to initialize 里海译:失败初始化 */  
            return;  
        }  
        /* TODO: Add your application code here 里海译:翻译TODO: 在这里添加你的应用程序代码。 */  
        if( argc < 2 )  
        {  
            printf("Usage: %s MIN_error_code [MAX_error_code]\n", argv[0]);  
            return;  
        }  
        min = atoi(argv[1]);  
        if(argc == 3)  
        {  
            max = atoi(argv[2]);  
            if(max != 0 && min >= max)  
            {  
                printf("MIN_error_code cannot be equal or greater than MAX_error_code.\n");  
                return;  
            }  
            for(ii=min; ii<=max; ii++) report_error( ii );  
        }  
        else   
        {  
            report_error( min );  
        }  
        /* Terminate the API environment 里海译:终止API环境 */  
        UF_terminate();  
    }

```

#### 代码解析

> 这段代码是一个用于报告NX Open API错误代码的程序。其功能如下：
>
> 1. 包含了必要的头文件，包括标准库头文件以及NX Open API的头文件。
> 2. 定义了一个replace_char_from_string函数，用于将字符串中指定的字符替换成另一个字符。
> 3. 定义了一个report_error函数，用于获取错误代码对应的错误信息，并进行打印。错误信息中的换行符会被替换成空格。
> 4. 主函数首先初始化NX Open API环境，然后解析命令行参数，获取要报告的错误代码范围。对于每个错误代码，调用report_error函数进行打印。最后终止NX Open API环境。
> 5. 如果命令行参数不足，程序会打印使用说明并退出。
> 6. 程序支持两种命令行参数格式：只指定一个错误代码，或者指定一个最小和最大错误代码范围。
> 7. 在报告错误时，可以选择打印详细错误信息，也可以选择单行简洁信息。
> 8. 程序使用了NX Open API的错误代码获取函数UF_get_fail_message。
>
> 总体来说，这段代码提供了一个简单的工具，用于开发者查询和学习NX Open API的错误代码及其对应的信息。
>
