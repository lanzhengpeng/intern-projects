### 【1810】put a note on every drawing sheet 在每张图纸放置注释

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_drf.h>  
    #include <uf_draw.h>  
    #include <uf_obj.h>  
    #include <uf_ui.h>  
    #include <stdarg.h>  
    #pragma warning(disable:4996)  
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
    static void do_it(void)  
    {  
        int  
            inx = 0,  
            dwgCount = 0,  
            displayState = -1,  
            txtLinesCount  = 3,  
            txtOrientation = 0; // horizontal  
        tag_t  
            dwg = NULL_TAG,  
            *allDwgs = NULL,  
            theNote = NULL_TAG;  
        double  
            origin[3] = { 40., 75., 0. };  
        char *  
            text[] = {"This is a NOTE", "and it should appear", "on every drawing sheet." };  
        UF_CALL(UF_DRAW_ask_display_state(&displayState));  
        UF_CALL(UF_DRAW_ask_drawings(&dwgCount, &allDwgs));  
        for(inx = 0; inx < dwgCount; inx++)  
        {  
            UF_CALL(UF_DRAW_open_drawing(allDwgs[inx]));  
            if(displayState == 1)   
            {  
                UF_CALL(UF_DRAW_set_display_state(2));  
            }  
            UF_CALL(UF_DRF_create_note(txtLinesCount, text, origin, txtOrientation, &theNote));  
        }  
        if(dwgCount > 0) UF_free(allDwgs);  
    }  
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

> 这段代码是NX的二次开发代码，主要用于在NX图纸中添加注释。其主要功能包括：
>
> 1. 包含必要的NX二次开发头文件，如uf.h、uf_drf.h等。
> 2. 定义了ECHO宏，用于输出信息到日志窗口。
> 3. 定义了UF_CALL宏，用于调用NX API函数，并捕获错误码。
> 4. 定义了report_error函数，用于报告函数调用错误。
> 5. 定义了do_it函数，用于在所有打开的图纸中添加注释。该函数首先获取所有打开图纸的数量和数组，然后遍历图纸数组，打开每个图纸，并调用UF_DRF_create_note API创建注释。
> 6. 定义了ufusr函数，这是二次开发的入口函数。该函数首先初始化NX环境，然后调用do_it函数添加注释，最后终止NX环境。
> 7. 定义了ufusr_ask_unload函数，用于设置二次开发模块卸载策略。
>
> 总体而言，这段代码实现了在NX图纸中批量添加注释的功能，为NX二次开发提供了示例。
>
