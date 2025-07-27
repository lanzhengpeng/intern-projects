### 【1243】extract face from grip 从GRIP提取面

#### 代码

```cpp
    /*HEAD EXTRACT_FACE_FROM_GRIP CCC UFUN */  
    /*  This is a generic UG/Open API (Ufunc) program which can be called whenever  
        it is desired to extract a sheet from an occurrence face in GRIP.  The GRIP  
        command BSURF/SURFC does not work on occurrences.  (See PR 585332)  
        Here is sample GRIP code demonstrating how to use the program:  
            entity/ face  
            grargs/ face  
            mask/71  
    a10:    ident/ 'Select face', face, resp  
            ifthen/ resp > 2  
                bsurf/surfc, face, iferr, a20:  
                    jump/ a10:  
    a20:        messg/temp, 'grip didn''t work, trying ufun...'  
                xspawn/ufun,'/full/path/to/internal/program.sl'  
                jump/ a10:  
            endif  
            halt  
    */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog是V18版本中新增的一个功能。它可以将日志信息输出到系统日志中。 */  
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
        tag_t  
            face,  
            sheet;  
        UF_args_t  
            arg;  
        arg.type = UF_TYPE_TAG_T;  
        arg.length = 0;  
        arg.address = &face;  
        UF_CALL(UF_ask_grip_args(1, &arg));  
        UF_CALL(UF_MODL_extract_face(face, 2, &sheet));  
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

> 这是NX的二次开发代码，其主要功能是从GRIP中选择一个面，并提取该面作为一个薄片实体。代码采用了UFUNC的架构，主要包含以下几个部分：
>
> 1. 错误报告函数：定义了report_error函数，用于报告UF函数调用错误，并将错误信息输出到系统日志和用户界面。
> 2. 提取面操作函数：定义了do_it函数，从GRIP参数中获取面的tag，并调用UF_MODL_extract_face函数提取面作为一个薄片实体。
> 3. UFUNC入口函数：定义了ufusr函数，用于初始化和执行do_it函数。
> 4. 卸载函数：定义了ufusr_ask_unload函数，用于设置UFUNC立即卸载。
> 5. 代码头部注释：详细介绍了代码的功能和使用方法。
>
> 代码通过在GRIP中调用UFUNC，实现了从GRIP中提取面为薄片的功能，从而解决了GRIP自身不支持提取occurrence面的问题。该代码结构清晰，逻辑简单，是一个典型的NX二次开发Ufunc示例。
>
