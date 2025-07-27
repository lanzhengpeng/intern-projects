### 【0130】ask box of assy 问装配体箱体

#### 代码

```cpp
    /*HEAD ASK_BOX_OF_ASSY CCC UFUN */  
    #include <stdio.h>  
    #include <uf_fltr.h>  
    #include <uf_assem.h>  
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
    static void do_it(void)  
    {  
        tag_t     
            comp = NULL_TAG;  
        double    
            centroid[3] = { 0., 0., 0. },   
            corner[3] = { 0., 0., 0. },   
            orientation[9] = { 0., 0., 0.,  0., 0., 0.,  0., 0., 0. };  
        char  
            msg[300] = { "" };  
        comp = UF_ASSEM_ask_root_part_occ( UF_ASSEM_ask_work_part());  
        if (comp != NULL_TAG)  
        {  
            UF_CALL( UF_FLTR_ask_box_of_assy( comp, centroid,  
                                              corner, orientation));  
            sprintf(msg, "\nCentroid coordinates: %f, %f, %f\n", centroid[0],  
                                          centroid[1], centroid[2]);  
            WRITE( msg );  
            sprintf(msg, "Corner coordinates: %f, %f, %f\n", corner[0],  
                                           corner[1], corner[2]);  
            WRITE( msg );  
        }  
        else  
            uc1601("work part is not an assembly", TRUE);  
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

> 这段代码是用于NX的二次开发代码，主要实现了以下功能：
>
> 1. 包含了必要的头文件，如stdio.h、uf_fltr.h、uf_assem.h等，用于调用NX的API函数。
> 2. 定义了一个错误报告函数report_error，用于在调用NX API函数出错时输出错误信息。
> 3. 定义了一个宏WRITE，用于向NX日志窗口打印信息。
> 4. 定义了一个函数do_it，用于获取当前工作部件的根装配组件，然后调用UF_FLTR_ask_box_of_assy函数获取该组件的包围盒信息（质心坐标、顶点坐标、旋转矩阵），并将这些信息打印到日志窗口。
> 5. 定义了ufusr函数，作为NX二次开发的入口函数。在ufusr中初始化NX，调用do_it函数，然后终止NX。
> 6. 定义了ufusr_ask_unload函数，返回立即卸载，表示二次开发应用程序在退出时不需要执行清理工作。
> 7. 整体来说，这段代码的功能是获取当前工作部件的根装配组件的包围盒信息，并打印输出。这可以用于二次开发中需要获取组件位置信息的应用场景。
>
