### 【2774】set manual weight of workpart 设置工作部件的手动重量

#### 代码

```cpp
    /* Include files 里海译:Include files翻译为“包含文件”。 */  
    #include <stdarg.h>  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_assem.h>  
    #include <uf_ui_types.h>  
    #include <uf_ui.h>  
    #include <uf_weight.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_BUFSIZE];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), _TRUNCATE, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error(char *file, int line, char *call, int irc)  
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
    void do_it()  
    {  
        tag_t workPart = UF_ASSEM_ask_work_part();  
        if (workPart != NULL_TAG)  
        {  
            UF_WEIGHT_properties_t propsOld, propsNew;  
            UF_CALL(UF_WEIGHT_ask_props(workPart, UF_WEIGHT_units_km, &propsOld));  
            ECHO("Old Weight: %f\n", propsOld.mass);  
            ECHO("Accuracy: %.2f\n", propsOld.accuracy);  
            propsNew.cache_state = propsNew.mass_state = UF_WEIGHT_asserted;  
            propsNew.area_state = propsNew.cofm_state = propsNew.mofi_state = propsNew.density_state = propsNew.volume_state = UF_WEIGHT_unknown;  
            propsNew.accuracy = 1.0;  
            propsNew.units = UF_WEIGHT_units_gm;  
            propsNew.mass = 1234.56;  
            UF_CALL(UF_WEIGHT_assert_part_props(workPart, &propsNew));  
            UF_CALL(UF_WEIGHT_ask_props(workPart, UF_WEIGHT_units_km, &propsOld));  
            ECHO("New Weight: %f\n", propsOld.mass);  
        }  
    }  
    /*****************************************************************************  
    **  Activation Methods  
    *****************************************************************************/  
    /*  Explicit Activation  
    **      This entry point is used to activate the application explicitly, as in  
    **      "File->Execute UG/Open->User Function..." */  
    extern DllExport void ufusr(char *parm, int *returnCode, int rlen)  
    {  
        /* Initialize the API environment 里海译:初始化API环境 */  
        if (UF_CALL(UF_initialize()))  
        {  
            /* Failed to initialize 里海译:Failed to initialize 翻译为：初始化失败。 */  
            return;  
        }  
        do_it();  
        /* Terminate the API environment 里海译:终止API环境 */  
        UF_CALL(UF_terminate());  
    }  
    /*****************************************************************************  
    **  Utilities  
    *****************************************************************************/  
    /* Unload Handler  
    **     This function specifies when to unload your application from Unigraphics.  
    **     If your application registers a callback (from a MenuScript item or a  
    **     User Defined Object for example), this function MUST return  
    **     "UF_UNLOAD_UG_TERMINATE". */  
    extern int ufusr_ask_unload(void)  
    {  
        return(UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是NX Open的二次开发代码，主要用于设置和查询零件的重量属性。代码的主要功能包括：
>
> 1. 包含必要的NX Open头文件，以使用NX提供的API。
> 2. 定义了一个ECHO宏，用于在列表窗口打印信息，同时也会打印到系统日志中。
> 3. 定义了一个UF_CALL宏，用于调用NX API函数，并捕获错误代码。
> 4. 定义了一个report_error函数，用于打印错误信息。
> 5. 定义了一个do_it函数，用于查询和设置当前工作零件的重量属性。
> 6. 定义了一个ufusr函数，作为程序的入口点。在函数中，先初始化NX环境，然后调用do_it函数，最后终止NX环境。
> 7. 定义了一个ufusr_ask_unload函数，用于指定何时卸载应用程序。在这个例子中，应用程序被设置为立即卸载。
>
> 总的来说，这段代码展示了如何使用NX Open API来操作NX的零件重量属性，包括查询、设置和错误处理等，为二次开发提供了基础示例。
>
