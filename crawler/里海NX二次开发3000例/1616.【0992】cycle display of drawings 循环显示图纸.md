### 【0992】cycle display of drawings 循环显示图纸

#### 代码

```cpp
    /*HEAD CYCLE_DISPLAY_OF_DRAWINGS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是在V18版本中新增的函数。翻译如下：

注意：UF_print_syslog是V18版本新增的函数。 */  
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
    static tag_t ask_next_drawing(tag_t part, tag_t drawing)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drawing_type, &drawing));  
        return (drawing);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            drawing = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            dname[MAX_ENTITY_NAME_SIZE+1],  
            msg[133];  
        UF_CALL(UF_DRAW_set_display_state(UF_DRAW_DRAWING_VIEW));  
        while ((drawing = ask_next_drawing(part, drawing)) != NULL_TAG)  
        {  
            UF_CALL(UF_DRAW_open_drawing(drawing));  
            UF_CALL(UF_OBJ_ask_name(drawing, dname));  
            sprintf(msg, "Drawing %s is displayed", dname);  
            uc1601(msg, TRUE);  
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

> 这段代码是用于NX CAD系统的二次开发示例，主要功能是循环显示当前活动部件中的所有图纸。以下是代码的主要功能：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于在发生UF函数调用错误时，打印错误信息到系统日志窗口和日志文件中。
> 2. 获取下一个图纸：定义了一个函数ask_next_drawing，用于在部件中循环获取下一个图纸对象，直到没有图纸为止。
> 3. 主函数：主函数do_it首先获取当前活动的部件对象，然后循环调用ask_next_drawing获取每个图纸对象，并打开显示它，同时打印图纸名称到日志窗口。
> 4. NX启动函数：ufusr函数是NX启动时调用的主函数，初始化NX环境，调用do_it执行功能，然后终止NX环境。
> 5. 卸载函数：ufusr_ask_unload函数用于定义当用户卸载该程序时的行为，这里定义为立即卸载。
>
> 总的来说，这段代码提供了一个简单的示例，用于循环显示当前活动部件中的所有图纸，并包含错误处理和日志打印功能。
>
