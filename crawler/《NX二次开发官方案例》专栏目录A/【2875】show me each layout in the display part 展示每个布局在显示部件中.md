### 【2875】show me each layout in the display part 展示每个布局在显示部件中

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_draw.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_layout.h>  
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
        tag_t  
            currentDwg = NULL_TAG,  
            dispPart = UF_PART_ask_display_part();  
        char  
            layoutName[ UF_OBJ_NAME_BUFSIZE ] = { "" };  
        if(NULL_TAG == dispPart) return;  
        UF_DRAW_ask_current_drawing(&currentDwg); // Don't UF_CALL this.  
        if(NULL_TAG != currentDwg)  
        {  
           ECHO("This won't work if you have a drawing active.\n\n");  
           ECHO("Please switch to Modeling, and try again.\n\n");  
           return;  
        }  
        do  
        {  
            const int  
                option = 2;      // Fit all views  
            const double  
                ignoreThis = 1;  // Dummy parameter  
           int  
               returnCode = uc6471(layoutName);    
           if((int) strlen(layoutName) < 1) break;  // Found all the layouts  
           ECHO("Layout name: %s\n", layoutName);  
           if(returnCode == 11)  
           {  
               ECHO("Layout does not exist\n");  
               return;  
           }  
           if(returnCode == 12)  
           {  
               ECHO("Invalid Layout name\n");  
               return;  
           }  
           returnCode = uc6468(layoutName, option, ignoreThis);  // Open the layout  
           if(returnCode == 11)  
           {  
               ECHO("Layout does not exist\n");  
               return;  
           }  
           if(returnCode == 12)  
           {  
               ECHO("Invalid Layout name\n");  
               return;  
           }  
           if(returnCode == 13)  
           {  
               ECHO("Layout Has No Views\n");  
               return;  
           }  
           if(returnCode == 31)  
           {  
               ECHO("Invalid Scale\n");  
               return;  
           }  
           if(returnCode == 32)  
           {  
               ECHO("Invalid Scale Option\n");  
               return;  
           }  
           // this error could occur if you were in Drafting,   
           // and you expanded a view.  No current drawing would be found,  
           // so it would pass the initial test, but this would fail.  
           if(returnCode == 35)  
           {  
               ECHO("Operation Not Allowed In Drafting\n");  
               return;  
           }  
           uc1601(layoutName, 1);  
        } while((int) strlen(layoutName) > 1);  
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

> 这段代码是NX的二次开发代码，其主要功能是遍历所有布局，并尝试打开每个布局。具体步骤如下：
>
> 1. 包含必要的NX API头文件。
> 2. 定义了ECHO宏，用于打印信息。
> 3. 定义了report_error函数，用于打印错误信息。
> 4. 定义了do_it函数，是主要逻辑：a. 获取当前显示的部件，如果没有部件则返回。b. 获取当前绘图，如果当前有绘图则返回，提示需要在建模模式下运行。c. 循环遍历所有布局：i. 获取下一个布局名称。ii. 打印布局名称。iii. 尝试打开布局，处理打开时的错误。iv. 如果布局打开成功，则关闭布局。v. 循环直到所有布局遍历完毕。
> 5. 定义了ufusr函数，是NX的回调函数。在该函数中初始化NX，调用do_it函数，然后终止NX。
> 6. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 综上，该代码的主要目的是遍历所有布局，并尝试打开每个布局。
>
