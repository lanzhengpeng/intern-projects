### 【2745】set crosshatch type 设置十字线类型

#### 代码

```cpp
    /*HEAD SET_CROSSHATCH_TYPE CCC UFUN */  
    #include <uf_drf.h>  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

请注意，UF_print_syslog 是在 V18 中新增的。 */  
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
    static tag_t select_anything(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t   hatch;  
        int     mpi_array[100];  
        double  mpr_array[70];  
        char    radius[7], diam[7];  
        hatch = select_anything("Select x-hatch to edit");  
        UF_CALL( UF_DRF_ask_object_preferences( hatch, mpi_array,  
                 mpr_array, radius, diam));  
        mpi_array[31] = 7;  
        UF_CALL( UF_DRF_set_object_preferences( hatch, mpi_array,  
                 mpr_array, radius, diam));  
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

> 根据代码内容，这是段NX的二次开发代码，主要功能是设置所选交叉线的类型。具体步骤如下：
>
> 1. 包含了必要的NX开发头文件。
> 2. 定义了UF_CALL宏，用于调用NX函数并检查返回码，如果出错则打印错误信息。
> 3. 定义了select_anything函数，用于让用户选择一个交叉线，并返回交叉线的tag。
> 4. 定义了do_it函数，首先调用select_anything函数获取用户选择的交叉线，然后获取交叉线的参数，修改交叉线类型参数，最后设置交叉线的参数。
> 5. 定义了ufusr函数，这是NX二次开发程序的入口函数，在函数中初始化NX环境，调用do_it函数执行主要功能，然后终止NX环境。
> 6. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示二次开发程序可以立即卸载。
>
> 总体来说，这段代码的主要功能是让用户选择一个交叉线，然后修改并设置该交叉线的类型参数。通过这段代码，用户可以方便地调整交叉线的类型。
>
