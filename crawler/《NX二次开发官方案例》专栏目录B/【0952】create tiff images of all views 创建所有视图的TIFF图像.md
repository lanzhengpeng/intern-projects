### 【0952】create tiff images of all views 创建所有视图的TIFF图像

#### 代码

```cpp
    /*HEAD CREATE_TIFF_IMAGES_OF_ALL_VIEWS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_view.h>  
    #include <uf_layout.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 版本中新增的函数。

翻译：UF_print_syslog 是在 V18 版本中新增的函数。 */  
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
    static tag_t ask_next_view(tag_t part, tag_t view)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_view_type, &view));  
        return (view);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part(),  
            view = NULL_TAG;  
        char  
            name[36];  
        uc6468("L1", 1, 1.0);  
        while ((view = ask_next_view(part, view)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_name(view, name));  
            if (strstr(name, "@")) continue;  
            uc6464("", "", name);  
            UF_CALL(UF_VIEW_fit_view(NULL_TAG, 1.0));  
            strcat(name, ".tif");  
            UF_CALL(UF_DISP_create_image(name, UF_DISP_TIFF, UF_DISP_ORIGINAL));  
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

> 这段代码是NX的二次开发示例，主要功能是创建当前活动部件的所有视图的TIFF格式图片。以下是代码的主要组成部分：
>
> 1. 包含必要的NX头文件，如uf.h、uf_ui.h、uf_obj.h等。
> 2. 定义了UF_CALL宏，用于调用NX函数，并检查错误，如果出错则报告错误。
> 3. 定义了ask_next_view函数，用于遍历活动部件的所有视图，返回下一个视图的tag。
> 4. 定义了do_it函数，用于遍历所有视图，如果视图名称中包含@符号，则跳过该视图。然后调整视图，使其适应屏幕，并调用UF_DISP_create_image函数将视图保存为TIFF格式的图片。
> 5. ufusr函数是NX二次开发程序的入口函数。初始化NX环境，调用do_it函数执行任务，然后终止NX环境。
> 6. ufusr_ask_unload函数用于设置程序卸载方式，这里设置为立即卸载。
>
> 总体来说，这段代码实现了遍历活动部件的所有视图，并将视图保存为TIFF图片的功能。它包含了NX二次开发中常见的错误处理、视图遍历和图像保存等操作。
>
