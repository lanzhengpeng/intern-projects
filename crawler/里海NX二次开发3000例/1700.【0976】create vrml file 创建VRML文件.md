### 【0976】create vrml file 创建VRML文件

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_std.h>  
    #include <uf_part.h>  
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
        char  
            tfn[UF_CFI_MAX_PATH_NAME_SIZE];  
        double  
            tol = 1.0;  
        build_similar_filespec(tfn, 0);  
        strcat(tfn, ".wrl");  
        if( prompt_for_text("Enter VRML file name:", tfn) )  
            UF_STD_create_vrml_file(tfn, tol, (UF_STD_OUTPUT_LIGHTS |   
            UF_STD_OUTPUT_MATERIALS | UF_STD_OUTPUT_TEXTURES));  
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

> 这是段NX二次开发代码，主要功能是导出当前打开的NX零件为VRML格式文件。以下是主要功能模块的简要介绍：
>
> 1. ECHO宏：用于打印信息到日志窗口和系统日志。
> 2. UF_CALL宏：用于封装NX API调用，并打印错误信息。
> 3. build_similar_filespec函数：构建与当前打开零件相似的文件路径。
> 4. prompt_for_text函数：弹窗提示用户输入VRML文件名。
> 5. do_it函数：构建文件路径，提示用户输入文件名，并调用NX API导出零件为VRML文件。
> 6. ufusr函数：NX二次开发主入口函数。初始化NX环境，执行do_it函数，然后结束NX环境。
> 7. ufusr_ask_unload函数：用于卸载二次开发应用程序。
>
> 整体而言，这段代码实现了在NX中导出当前打开零件为VRML格式的功能，其中运用了NX提供的API来实现文件操作和交互提示。
>
