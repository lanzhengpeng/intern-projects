### 【2425】report view matrix 报告视图矩阵

#### 代码

```cpp
    /*HEAD REPORT_VIEW_MATRIX CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_view.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的函数。 */  
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
    #define WRITE9F(X)  (write_double_array_to_listing_window(#X, X, 3, 3))  
    /* qq3123197280 */  
    static void write_double_array_to_listing_window(char *name, double *array,  
                                                    int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            UF_CALL(UF_UI_write_listing_window(msg));  
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
            UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    /* qq3123197280 */  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* qq3123197280 */  
    static logical select_a_view( char *prompt, char *view_name)  
    {  
        int  
            ret_code,  
            view_of_cursor;  
        double  
            pos[3];  
        UF_CALL( UF_UI_ask_cursor_view( &view_of_cursor));  
        UF_CALL( UF_UI_set_cursor_view( 0));  
        ret_code = uc1615(prompt, pos);  
        UF_CALL( UF_UI_set_cursor_view( view_of_cursor));  
        if (ret_code == 5)  
        {  
            UF_CALL( uc1653( view_name ));  
            return TRUE;  
        }  
        else  
            return FALSE;  
    }  
    static void do_it(void)  
    {  
        logical  
            flag = FALSE;  
        double  
            mtx[9] = { 0., 0., 0., 0., 0., 0., 0., 0., 0. };  
        char  
            viewname[MAX_ENTITY_NAME_SIZE+1];         
        while( (flag = select_a_view( "Select a View:", viewname )) != FALSE )  
        {  
            UF_CALL(uc6433(viewname, mtx ));  
            WRITE_S( viewname );  
            WRITE9F( mtx );  
            UF_CALL(UF_UI_write_listing_window( "\n" ));  
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

> 这段代码是一个用于在NX中显示视图变换矩阵的二次开发代码，其主要功能如下：
>
> 1. 定义了错误处理宏UF_CALL，用于在调用NX API函数时检查返回码，并在出错时打印错误信息。
> 2. 定义了WRITE9F和WRITE_S宏，用于将数字数组和字符串输出到日志窗口。
> 3. 实现了select_a_view函数，用于提示用户选择一个视图，并返回视图名称。
> 4. 实现了do_it函数，用于循环提示用户选择视图，并调用uc6433 API获取每个视图的变换矩阵，然后输出到日志窗口。
> 5. 实现了ufusr主函数，用于初始化NX API，调用do_it函数，然后终止NX API。
> 6. 实现了ufusr_ask_unload函数，用于卸载用户函数。
>
> 综上所述，这段代码通过交互式提示用户选择视图，并调用NX API获取每个视图的变换矩阵，然后输出到日志窗口，以显示每个视图的变换信息。
>
