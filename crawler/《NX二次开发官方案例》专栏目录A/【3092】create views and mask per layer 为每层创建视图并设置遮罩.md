### 【3092】create views and mask per layer 为每层创建视图并设置遮罩

#### 代码

```cpp
    /*HEAD CREATE_VIEWS_AND_MASK_PER_LAYER CCC UFUN */  
    /* This creates a view per layer and masks it   
            to show only that layer visible in view */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_draw.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中新增的功能。文档要求只提供译文，无需添加其他内容。因此，UF_print_syslog在V18版本中是一个新增的功能。 */  
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
    static void do_it(void)  
    {  
        int  
            jj = 0,  
            ii = 0,  
            junk = 0,  
            n = 9, /* number of views and layers 里海译:number of views and layers 翻译为：视图数和层数 */  
            vis_mask[256];  
        char  
            messg[300],  
            vname[MAX_ENTITY_NAME_SIZE + 1];  
        for (jj = 1; jj <= n; ++jj)    
        {  
            sprintf(vname,"view_of_layer_%d", jj);  
            /* save the view 里海译:保存视图 */   
            uc6450("", vname, FALSE, FALSE);  
            /* set the view mask 里海译:设置视图遮罩 */   
            for (ii = 0; ii < 255; ii++)  
            {  
                vis_mask[ii] = 0;  
            }  
            vis_mask[jj-1] = 1; /*layer to make visible */  
            /* apply the view mask 里海译:apply the view mask */  
            uc6447(vname, 2, vis_mask);  
            /* save the view so layer - visible-in-view settings remain 里海译:保存视图，以便图层的可见性设置保持不变。 */   
            uc6450("", "", junk, junk); /* last two don't matter   
                when saving the current view */  
        }  
        sprintf(messg,"%d Masked Views Created", jj);  
        uc1601(messg, TRUE);  
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

> 这是段NX二次开发代码，其主要功能是创建多个视图，并设置每个视图只显示一个图层。
>
> 具体步骤如下：
>
> 1. 包含必要的NX二次开发头文件。
> 2. 定义一个错误报告函数report_error，用于在出错时输出错误信息。
> 3. 定义一个do_it函数，用于实现创建视图和设置视图遮罩的逻辑。
> 4. 在do_it函数中，遍历视图和图层的数量，为每个视图创建一个名称，然后保存当前视图为该名称。
> 5. 初始化一个视图遮罩数组，只将当前遍历到的图层设置为可见，其他图层设置为不可见。
> 6. 应用该视图遮罩到当前视图。
> 7. 保存当前视图，以保持图层可见性的设置。
> 8. 循环结束后，输出创建的视图数量。
> 9. 在ufusr函数中初始化NX环境，调用do_it函数，然后终止NX环境。
> 10. 定义一个卸载函数，用于在插件卸载时立即清理资源。
>
> 综上所述，该代码通过创建多个视图并设置视图遮罩，实现了按图层查看NX模型的功能。
>
