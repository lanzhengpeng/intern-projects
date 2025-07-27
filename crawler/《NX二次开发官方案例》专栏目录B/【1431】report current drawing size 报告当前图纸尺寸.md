### 【1431】report current drawing size 报告当前图纸尺寸

#### 代码

```cpp
    /*HEAD REPORT_CURRENT_DRAWING_SIZE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的提示，UF_print_syslog 是 V18 版本中新增的函数。文档要求只提供翻译，不添加任何额外内容。

因此，对于 UF_print_syslog 的翻译是：UF_print_syslog 是 V18 版本新增的函数。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE2F(X)  (write_double_array_to_listing_window(#X, X, 1, 2))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            size_code,  
            units;  
        double  
            sizes[2];  
        WRITE_D(uc6479("", &units, &size_code, sizes));  
        WRITE_D(units);  
        WRITE_D(size_code);  
        WRITE2F(sizes);  
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

> 这段代码是用于NX的二次开发，主要功能是获取当前绘图尺寸的报告。代码流程如下：
>
> 1. 首先包含必要的头文件，定义错误报告函数report_error()。
> 2. 定义宏UF_CALL，用于调用NX API函数，并在出错时调用report_error()报告错误。
> 3. 定义宏WRITE_D，用于向列表窗口写入整数值。
> 4. 定义宏WRITENZ，用于向列表窗口写入非零的整数值。
> 5. 定义write_integer_to_listing_window()函数，用于向列表窗口写入整数值。
> 6. 定义宏WRITE2F，用于向列表窗口写入二维double数组。
> 7. 定义write_double_array_to_listing_window()函数，用于向列表窗口写入二维double数组。
> 8. 定义do_it()函数，用于获取当前绘图尺寸并写入列表窗口。
> 9. 定义ufusr()函数，是NX二次开发的入口函数，用于初始化NX、调用do_it()获取报告、终止NX。
> 10. 定义ufusr_ask_unload()函数，用于卸载二次开发时立即返回。
>
> 综上，该代码实现了获取当前绘图尺寸并报告的功能。
>
