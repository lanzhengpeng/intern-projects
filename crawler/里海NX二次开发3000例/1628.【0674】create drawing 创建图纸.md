### 【0674】create drawing 创建图纸

#### 代码

```cpp
    /*HEAD CREATE_DRAWING CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：

注意：UF_print_syslog在V18中是新增的。 */  
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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            drawing,  
            part = UF_PART_ask_display_part();  
        char  
            dname[MAX_ENTITY_NAME_SIZE+1];  
        UF_DRAW_info_t  
            draw_info = { UF_DRAW_CUSTOM_SIZE, 0.0, 1.0, UF_PART_ENGLISH,  
                UF_DRAW_THIRD_ANGLE_PROJECTION };  
        draw_info.size.custom_size[0] = 10;  
        draw_info.size.custom_size[1] = 7.5;  
        while (prompt_for_text("Enter name for new drawing", dname))  
        {  
            drawing = NULL_TAG;  
            UF_CALL(UF_OBJ_cycle_by_name_and_type(part, dname, UF_drawing_type,  
                FALSE, &drawing));  
            if (drawing == NULL_TAG)  
            {  
                WRITE("Creating ");  
                WRITE(dname);  
                WRITE("...\n");  
                UF_CALL(UF_UI_set_status(dname));  
                UF_CALL(UF_DRAW_create_drawing(dname, &draw_info, &drawing));  
            }  
            else  
            {  
                WRITE("Opening existing drawing ");  
                WRITE(dname);  
                WRITE("...\n");  
                UF_CALL(UF_DRAW_open_drawing(drawing));  
            }  
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

> 这段代码是一个NX二次开发程序，主要实现了创建或打开绘图的功能。
>
> 主要步骤包括：
>
> 1. 包含必要的NX开发库头文件。
> 2. 定义宏UF_CALL，用于检测函数调用是否返回错误，并报告错误信息。
> 3. 定义宏WRITE，用于在列表窗口中写入信息。
> 4. 定义函数report_error，用于报告错误信息。
> 5. 定义函数prompt_for_text，用于提示用户输入文本。
> 6. 定义函数do_it，用于实现创建或打开绘图的功能。
> 7. 在do_it函数中，首先获取当前显示的零件。
> 8. 循环提示用户输入绘图名称，直到用户输入为空。
> 9. 检查输入的绘图名称是否已存在，若不存在则创建新绘图，否则打开现有绘图。
> 10. 使用UF_DRAW_create_drawing或UF_DRAW_open_drawing函数实现创建或打开绘图。
> 11. ufusr函数初始化NX开发环境，调用do_it函数，然后终止开发环境。
> 12. ufusr_ask_unload函数设置卸载模式。
>
> 整体上，这段代码实现了创建或打开NX绘图的功能，通过循环提示用户输入绘图名称，并检查名称是否存在，以实现创建或打开绘图的目的。
>
