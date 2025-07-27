### 【0704】create framed image 创建带边框的图像

#### 代码

```cpp
    /*HEAD CREATE_FRAMED_IMAGE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp_ugopenint.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本中的新功能，用于打印系统日志。 */  
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
    static logical prompt_for_four_integers(char *prompt, char *item1, char *item2,  
        char *item3, char *item4, int *number1, int *number2, int *number3,   
            int *number4)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[4][16];  
        int  
            da[4];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        strncpy(&menu[2][0], item3, 15);  
        strncpy(&menu[3][0], item4, 15);  
        menu[0][15] = '\0';  
        menu[1][15] = '\0';  
        menu[2][15] = '\0';  
        menu[3][15] = '\0';  
        da[0] = *number1;  
        da[1] = *number2;  
        da[2] = *number3;  
        da[3] = *number4;  
        resp = uc1607(prompt, menu, 4, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            *number3 = da[2];  
            *number3 = da[3];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int   
            width = 640,  
            height = 480,  
            xval = 0,  
            yval = 480,  
            ul[2];  
     while (prompt_for_four_integers("Enter pixel values for image",  
                "Width pixels", "Height", "Upper left X", "Upper Left Y",   
                    &width, &height, &xval, &yval))  
            {  
            /* PR 5693564 shows "upper left" settings not working in nx4 译:翻译PR 5693564显示nx4中的"upper left"设置无法工作。 */  
                ul[0] = xval;  
                ul[1] = yval;  
                UF_CALL(UF_DISP_create_framed_image( "framed.jpg", UF_DISP_JPEG,   
                    UF_DISP_WHITE, ul, width, height));  
            }  
    }  
    /* 里海 */  
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

> 这段代码是一个NX二次开发示例，用于创建一个带边框的图像。
>
> 主要功能包括：
>
> 1. 包含必要的NX Open头文件，以使用NX提供的API。
> 2. 定义了一个错误报告函数report_error，用于在发生错误时打印错误代码和消息。
> 3. 定义了一个提示用户输入4个整数的函数prompt_for_four_integers，用于获取图像的尺寸和位置信息。
> 4. 定义了一个do_it函数，用于循环提示用户输入，并根据用户输入创建带边框的图像。
> 5. 在ufusr函数中，首先初始化NX，然后调用do_it函数，最后终止NX。
> 6. ufusr_ask_unload函数用于设置立即卸载二次开发程序。
>
> 整体来看，这段代码实现了在NX中创建带边框图像的功能，通过用户输入获取图像参数，并调用NX API创建图像。这段代码为NX二次开发提供了一个很好的示例。
>
