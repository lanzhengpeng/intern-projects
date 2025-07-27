### 【2708】select sketch dimensions 选择草图尺寸

#### 代码

```cpp
    /*HEAD SELECT_SKETCH_DIMENSIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的函数，用于打印系统日志消息。 */  
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
    static int select_sketch_dimensions(char *prompt, tag_t sketch, tag_t **dims)  
    {  
        int  
            n,  
            resp;  
        if (!UF_CALL(UF_UI_select_sketch_dimensions(prompt,sketch,&n,dims,&resp))  
        && (resp == UF_UI_OK))  
            return n;  
        else  
            return 0;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *exps,  
            sketch;  
        while ((sketch = select_a_sketch("Select sketch")) != NULL_TAG)  
        {  
            n = select_sketch_dimensions("Select dimensions", sketch, &exps);  
            for (ii = 0; ii < n; ii++)  
            {  
                ECHO(ii);  
                ECHO(exps[ii]);  
            }  
            if (n > 0) UF_free(exps);  
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

> 这段代码是用于NX软件的二次开发代码，主要功能是选择草图并获取草图中的尺寸。具体来说：
>
> 1. 定义了错误处理函数report_error，用于打印错误代码和错误信息。
> 2. 定义了选择草图的函数select_a_sketch，使用UF_UI_select_sketch函数让用户选择草图，并返回草图tag。
> 3. 定义了选择草图尺寸的函数select_sketch_dimensions，使用UF_UI_select_sketch_dimensions函数让用户选择草图中的尺寸，并返回尺寸tag数组。
> 4. 在do_it函数中，循环调用select_a_sketch函数，直到用户没有选择草图为止。对于每个选择的草图，调用select_sketch_dimensions函数获取尺寸tag数组，然后遍历打印每个尺寸的tag。
> 5. 定义了ufusr函数，初始化后调用do_it函数，然后终止。
> 6. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 通过以上步骤，该代码实现了选择草图并获取草图尺寸的功能，可用于NX软件的二次开发。
>
