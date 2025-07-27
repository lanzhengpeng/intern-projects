### 【1050】demo creating a foreign surface 创建一个外部表面示例，用于证明一个观点

#### 代码

```cpp
    /*  
       Example user function program to create a simple foreign surface  
       using the uf_fs_setup.c foreign surface manager.  
    */  
    #include <stdarg.h>  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_forgeo.h>  
    #include <uf_modl.h>  
    #include "uf_brep_privileged.h"  
    /*=============================================================================  
    Function description:  
      Create a very simple foreign surface to prove a point.  
    =============================================================================*/  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    static void create_fs(void)  
    {  
        int j,k,int_array = 0 ;  
        char *char_array = "not used" ;  
        double double_array[] = {1.0, 2.0, 3.0,    /* origin 里海译:origin */  
                                 3.0, 2.0, 3.0,    /* x-axis 里海译:x-axis */  
                                 1.0, 6.0, 3.0} ;  /* y-axis 里海译:y轴 */  
        UF_FORGEO_forsurf_t forsurf_data ;  
        tag_t sheet_tag = NULL_TAG ;  
    /*  
       Load the foreign surface data structure.  
    */  
        forsurf_data.char_data = char_array ;  
        forsurf_data.nspace = 11 ;  
        forsurf_data.num_ints = 0 ;  
        forsurf_data.int_array = &int_array ;  
        forsurf_data.num_doubles = 9 ;  
        forsurf_data.double_array = double_array ;  
        for (j = 0; j < 4; j++)  
        {  
            for (k = 0; k < 4; k++)  
            {  
                if (j == k)  
                    forsurf_data.transform[j][k] = 1.0 ;  
                else  
                    forsurf_data.transform[j][k] = 0.0 ;  
            }  
        }  
    /*  
       Create the sheet body.  
    */  
        if (!UF_CALL(UF_BREP_create_sheet(&forsurf_data,&sheet_tag) )  
            && (sheet_tag != NULL_TAG))  
        {  
            UF_CALL(UF_DISP_add_item_to_display(sheet_tag));  
            UF_CALL(UF_MODL_update());  
        }  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
    /*  
       Create a foreign surface.  
    */  
            create_fs() ;  
            UF_terminate();  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  // This one is OK to unload  
    }

```

#### 代码解析

> 这段代码是一个简单的NX二次开发示例，用于创建一个外部表面。
>
> 主要功能和步骤如下：
>
> 1. 定义了一个ECHO函数，用于在列表窗口和系统日志中打印信息。
> 2. 定义了一个报告错误的函数report_error，用于打印出错误信息。
> 3. 定义了创建外部表面的函数create_fs。在这个函数中：定义了外部表面数据结构，包括字符数组、整数数组和双精度数组，并设置了坐标变换矩阵。调用UF_BREP_create_sheet函数创建一个片体。如果创建成功，则添加到显示列表并更新模型。
> 4. 定义了外部表面数据结构，包括字符数组、整数数组和双精度数组，并设置了坐标变换矩阵。
> 5. 调用UF_BREP_create_sheet函数创建一个片体。
> 6. 如果创建成功，则添加到显示列表并更新模型。
> 7. 定义了ufusr函数，这是用户函数的入口点。在这个函数中：初始化NX Open API。调用create_fs函数创建外部表面。终止NX Open API。
> 8. 初始化NX Open API。
> 9. 调用create_fs函数创建外部表面。
> 10. 终止NX Open API。
> 11. 定义了ufusr_ask_unload函数，用于卸载用户函数。
>
> 总体来说，这是一个简单的示例，演示了如何使用NX Open API创建一个外部表面，并添加到NX模型中显示。
>
