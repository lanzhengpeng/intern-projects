### 【2741】set clipping planes 设置裁剪平面

#### 代码

```cpp
    /*HEAD SET_CLIPPING_PLANES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_view.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    /* qq3123197280 */  
    static logical prompt_for_two_numbers(char *prompt, char *item1, char *item2,  
        double *number1, double *number2)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[2][16];  
        double  
            da[2];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        menu[0][15] = '\0';  
        menu[1][15] = '\0';  
        da[0] = *number1;  
        da[1] = *number2;  
        resp = uc1609(prompt, menu, 2, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    /* qq3123197280 */  
    static void process_uc6437_return_code( int retcode )  
    {  
        switch (retcode)  
        {  
             case 1:  
                     WRITE("View Does Not Exist");  
                     break;  
             case 2:  
                     WRITE("Invalid View Name");  
                     break;  
             case 3:  
                     WRITE("View Has 3D Vectors - Can Not Remove Clipping Planes");  
                     break;  
             case 9:  
                     WRITE("View Not Active");  
                     break;  
             case 31:  
                     WRITE("Front Plane Behind Back Plane");  
                     break;  
             case 33:  
                     WRITE("Perspective View - Can Not Remove Front Plane");  
                     break;  
             case 34:  
                     WRITE("Perspective View - Front Plane Moved To Eye Point");  
        }     
    }  
    static void do_it(void)  
    {  
        int  
            ip2[2] = { 2, 2 },  
            retcode = 0;  
        tag_t  
            part = UF_PART_ask_display_part();  
        double  
            clipping[2] = { 0., 0. };  
        if( NULL_TAG == part ) return;  
        while( prompt_for_two_numbers("Clipping Planes:","Front","Back",   
                &clipping[0], &clipping[1]))  
        {          
            retcode = uc6437("",ip2,clipping );  
            if( retcode ) process_uc6437_return_code( retcode );  
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

> 这段NX二次开发代码的主要功能是设置和修改NX视图的裁剪平面。以下是代码的主要组成部分：
>
> 1. 包含的头文件：代码包含了一些必要的NX API头文件，例如uf.h、uf_view.h、uf_ui.h等，这些头文件提供了代码中使用的函数的原型和数据结构。
> 2. 错误报告函数：report_error函数用于打印出NX API调用失败的错误信息，包括文件名、行号、失败的函数调用、错误码和错误消息。
> 3. 提示输入两个数字的函数：prompt_for_two_numbers用于提示用户输入两个数字，作为裁剪平面的距离。用户可以通过NX的交互式对话框输入这两个数字。
> 4. 处理返回码的函数：process_uc6437_return_code用于处理设置裁剪平面失败的返回码，并给出相应的错误提示。
> 5. 执行主体函数：do_it函数首先获取当前显示的零件，然后无限循环提示用户输入裁剪平面距离，直到用户取消。用户输入后，调用NX API函数uc6437设置裁剪平面，并根据返回码处理可能出现的错误。
> 6. ufusr函数：这是NX二次开发的入口函数。它首先调用UF_initialize初始化NX环境，然后调用do_it执行主体功能，最后调用UF_terminate结束NX环境。
> 7. 卸载函数：ufusr_ask_unload函数用于指示NX在卸载该二次开发应用程序时立即卸载。
>
> 综上，该代码实现了设置NX视图裁剪平面的交互式功能，用户可以通过输入对话框修改裁剪平面距离，以控制视图的显示范围。
>
