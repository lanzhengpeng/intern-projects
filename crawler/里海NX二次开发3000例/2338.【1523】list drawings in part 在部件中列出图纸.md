### 【1523】list drawings in part 在部件中列出图纸

#### 代码

```cpp
    /*HEAD LIST_DRAWINGS_IN_PART CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 的新功能。 */  
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
            fspec[MAX_FSPEC_SIZE + 1],  
            dname[UF_OBJ_NAME_LEN+1];  
        UF_PART_ask_part_name(part, fspec);  
        printf("Drawings found in %s:\n", fspec);  
        while ((drawing = ask_next_drawing(part, drawing)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_name(drawing, dname));  
            printf("\t%s\n", dname);  
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

> 这是NX二次开发代码，其主要功能是在NX部件中遍历所有图纸，并打印出图纸的名称。
>
> 代码的要点如下：
>
> 1. 定义了UF_CALL宏，用于调用NX API函数，并捕获错误。如果发生错误，将错误信息打印到系统日志和对话框。
> 2. 定义了ask_next_drawing函数，用于在部件中遍历图纸。它调用UF_OBJ_cycle_objs_in_part函数，传入部件标签和图纸类型，获取下一个图纸标签。
> 3. do_it函数负责遍历部件中的所有图纸，并打印图纸名称。它首先获取当前显示的部件，然后循环调用ask_next_drawing获取图纸标签，并调用UF_OBJ_ask_name获取图纸名称，最后打印出来。
> 4. ufusr是用户入口函数，用于初始化NX环境，调用do_it执行功能，然后终止NX环境。
> 5. ufusr_ask_unload用于卸载用户函数，返回立即卸载。
>
> 总的来说，这段代码通过遍历部件中的图纸对象，获取图纸名称并打印，实现了查询部件中所有图纸名称的功能。
>
