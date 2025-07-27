### 【2689】select point collection 选择点集

#### 代码

```cpp
    /*HEAD SELECT_POINT_COLLECTION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中新增的函数，用于打印系统日志。 */  
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
    static logical ask_true_or_false(char *prompt, logical *deflt)  
    {  
        int  
            resp;  
        char  
            options[2][38] = { "True", "False" };  
        resp = 2 - *deflt;  
        resp = uc1603(prompt, resp, options, 2);  
        if (resp > 4)  
        {  
            *deflt = 6 - resp;  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        logical  
            flag = TRUE;  
        int  
            ii,  
            n,  
            resp;  
        UF_UI_chained_points_t  
            *points;  
        while (ask_true_or_false("Coincident points", &flag) &&  
            !UF_CALL(UF_UI_select_point_collection("Select points", flag, &points,  
                &n, &resp)) && (resp == UF_UI_OK))  
        {  
            for (ii = 0; ii < n; ii++)  
                printf("%d.  %d @ %f,%f,%f\n", ii + 1, points[ii].object,  
                    points[ii].pt[0], points[ii].pt[1], points[ii].pt[2]);  
            UF_free(points);  
        }  
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

> 这段代码是一个NX Open的UF程序，用于选择NX中的点集。主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于在调用UF函数出错时报告错误信息。
> 2. 定义了一个询问函数ask_true_or_false，用于询问用户是否选择共线点集。
> 3. do_it函数是主函数，用于循环调用UF_UI_select_point_collection函数选择点集，并打印点集信息。
> 4. ufusr是程序的入口函数，用于初始化UF环境，调用do_it函数，然后终止UF环境。
> 5. ufusr_ask_unload用于设置程序的卸载方式。
> 6. 使用了NX的UI库函数来交互式选择点集，并打印了所选点集的信息。
> 7. 通过UF环境初始化和终止来管理UF环境。
> 8. 使用了错误处理机制来处理UF函数调用错误。
> 9. 整体实现了选择点集并打印信息的简单功能。
>
> 希望这段代码的介绍对您有所帮助，如果您有其他问题，欢迎随时提出。
>
