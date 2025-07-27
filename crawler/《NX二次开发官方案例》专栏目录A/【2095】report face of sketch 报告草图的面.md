### 【2095】report face of sketch 报告草图的面

#### 代码

```cpp
    /*HEAD REPORT_FACE_OF_SKETCH CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中的新功能，它用于打印系统日志信息。 */  
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
    static tag_t select_a_sketch(char *prompt)  
    {  
        int  
            resp;  
        tag_t  
            sketch;  
        if (!UF_CALL(UF_UI_select_sketch(prompt, NULL, &sketch, &resp))  
        && (resp == UF_UI_OK))  
           return sketch;  
        else  
            return NULL_TAG;  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        logical  
            is_horiz;  
        tag_t  
            attach_face,  
            dir_ref,  
            sketch,  
            sket_feat,  
            tool;  
        while ((sketch = select_a_sketch("Select sketch")) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_object_feat(sketch, &sket_feat));  
            UF_CALL(UF_MODL_ask_link_faces(sket_feat, &attach_face, &tool, &dir_ref,  
                &is_horiz));  
            WRITE_D(attach_face);  
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

> 这段代码是用于NX的二次开发代码，主要功能是选择草图并获取草图所依附的面。具体来说：
>
> 1. 首先，代码包含了必要的NX API头文件，并定义了一个错误报告函数report_error，用于在调用NX API时发生错误时输出错误信息。
> 2. 定义了一个选择草图的函数select_a_sketch，它通过UF_UI_select_sketch函数让用户选择一个草图，并返回草图的tag。
> 3. 定义了一个写整数的函数write_integer_to_listing_window，用于在列表窗口输出整数值。
> 4. 在主函数do_it中，通过一个循环调用select_a_sketch选择草图，然后调用UF_MODL_ask_object_feat获取草图特征，再调用UF_MODL_ask_link_faces获取草图依附的面，并通过write_integer_to_listing_window输出。
> 5. ufusr是NX二次开发程序的入口函数，在函数中初始化NX环境，调用do_it执行主要功能，然后终止NX环境。
> 6. ufusr_ask_unload函数返回一个立即卸载的标志。
>
> 总的来说，这段代码的主要功能是选择草图并获取草图所依附的面，通过NX API实现，并通过列表窗口输出结果。
>
