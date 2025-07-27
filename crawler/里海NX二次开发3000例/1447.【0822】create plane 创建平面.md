### 【0822】create plane 创建平面

#### 代码

```cpp
    /*HEAD CREATE_PLANE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：

注意：UF_print_syslog 是 V18 中新增的，请只提供译文，不要添加任何评论。 */  
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
    static logical specify_vector(char *prompt, double vec[3], double point[3])  
    {  
        int  
            mode = UF_UI_NO_DEFAULT,  
            resp;  
        UF_CALL(UF_UI_specify_vector(prompt, &mode, UF_UI_DISP_TEMP_VECTOR,  
            vec, point, &resp));  
        if (resp == UF_UI_OK) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            plane;  
        double  
            org[3],  
            normal[3];  
        while (specify_vector("specify plane normal", normal, org))  
            UF_CALL(UF_MODL_create_plane(org, normal, &plane));  
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

> 这段代码是用于在NX环境中创建平面的用户函数。具体来说，主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于打印UF函数调用失败的错误代码和错误信息。
> 2. 定义了一个向量输入函数specify_vector，用于提示用户输入一个向量。
> 3. 定义了一个主函数do_it，在这个函数中，用户可以不断输入平面法向量，程序会根据输入的法向量和原点创建一个平面。
> 4. 定义了ufusr函数，这是NX二次开发的标准入口函数。在这个函数中，初始化了UF模块，并调用了do_it函数，然后终止了UF模块。
> 5. 定义了ufusr_ask_unload函数，这个函数用于设置用户函数卸载的方式，这里设置为立即卸载。
>
> 总的来说，这段代码实现了在NX中创建平面的交互式功能，用户可以不断输入法向量创建新的平面。代码结构清晰，功能明确，是一个典型的NX二次开发示例。
>
