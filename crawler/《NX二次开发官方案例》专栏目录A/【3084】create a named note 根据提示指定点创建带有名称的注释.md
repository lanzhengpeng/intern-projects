### 【3084】create a named note 根据提示指定点创建带有名称的注释

#### 代码

```cpp
    /*HEAD CREATE_A_NAMED_NOTE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_csys.h>  
    #include <uf_trns.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，以下是关键点的翻译：

1. UF_print_syslog 是 V18 中的新功能。
2. 该功能用于将系统日志信息打印到控制台。
3. 当系统日志级别设置为 UF_LOG_INFO 或更高级别时，UF_print_syslog 才会执行打印操作。
4. UF_print_syslog 可用于调试目的，将系统日志信息输出到控制台，方便跟踪程序运行过程中的日志信息。
5. 该功能在 V18 之前版本中不可用。

以上是对 UF_print_syslog 功能的简要翻译。如果需要更详细的解释或其他功能点的翻译，请告诉我。 */  
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
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
        else return FALSE;  
    }  
    static void map_abs2wcs(double *cc)  
    {  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            wcs[12],  
            mx[12];  
        int  
            irc;  
        tag_t  
            wcs_ent,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs_ent));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs_ent, &wcs_mx, wcs));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, &wcs[3]));  
        FTN(uf5940)(abs, wcs, mx, &irc);  
        FTN(uf5941)(cc, mx);  
    }  
    static void do_it(void)  
    {  
        int  
            ii = 1;  
        tag_t  
            a_note;  
        double  
            loc[3];  
        char  
            name[MAX_ENTITY_NAME_SIZE+1],  
            text[1][133];  
        while (specify_point("Indicate point for note", loc))  
        {  
            map_abs2wcs(loc);  
            sprintf(&text[0][0], "%f, %f, %f", loc[0], loc[1], loc[2]);  
            uc5540(1, text, loc, &a_note);  
            sprintf(name, "NOTE_%d", ii++);  
            UF_CALL(UF_OBJ_set_name(a_note, name));  
            uc1601("Note created", 1);  
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

> 这段代码是用于在NX中创建带有名称的注释的二次开发代码。主要功能如下：
>
> 1. 包含必要的NX API头文件。
> 2. 定义了一个错误报告函数report_error，用于打印错误信息到系统日志和控制台。
> 3. 定义了一个获取用户指定点的函数specify_point。
> 4. 定义了一个将绝对坐标系转换到工作坐标系的函数map_abs2wcs。
> 5. 定义了主函数do_it，用于循环获取用户指定的点，在每个点处创建注释，并设置注释的名称。
> 6. 定义了ufusr函数，用于初始化NX API，调用do_it函数，然后终止NX API。
> 7. 定义了ufusr_ask_unload函数，用于设置二次开发模块的卸载方式。
> 8. 使用UF_CALL宏包装所有NX API调用，用于报告错误。
> 9. 使用了NX提供的对话框函数获取用户输入。
> 10. 使用了NX提供的注释对象相关函数创建注释，并设置注释名称。
>
> 总的来说，这段代码实现了在NX中创建带有名称的注释的功能，用户可以指定注释的位置，代码会自动为每个注释生成一个唯一的名称。
>
