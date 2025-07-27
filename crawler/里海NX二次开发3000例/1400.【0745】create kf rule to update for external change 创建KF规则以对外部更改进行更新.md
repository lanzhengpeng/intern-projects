### 【0745】create kf rule to update for external change 创建KF规则以对外部更改进行更新

#### 代码

```cpp
    /*****************************************************************************  
    **  
    ** create_kf_rule_to_update_for_external_change.c  
    **  
    ** Description:  
    **     Contains Unigraphics entry points for the application.  
    **  
    *****************************************************************************/  
    /* Include files 译:翻译Include files为：包含文件 */  
    #include <stdarg.h>  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_cfi.h>  
    #include <uf_kf.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_ui_types.h>  
    #include <uf_ui.h>  
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
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            msg[64] = { "" },  
            *value,  
            ruleName[UF_MAX_UNIQUE_FILE_NAME_BUFSIZE],  
            ruleChain[133];  
        uc4577(ruleName);  
        sprintf(ruleChain, "root:%s:", ruleName);  
        UF_CALL(UF_KF_init_part(part));  
        UF_CALL(UF_KF_create_rule("root:", ruleName, "Any", "%ug_updateForExternalChange(false)", ""));  
        UF_CALL(UF_MODL_update());  
        if (!UF_CALL(UF_KF_evaluate_rule_to_string(ruleChain, &value)))  
        {  
            sprintf(msg, "Value of Rule <%s> is: %s", ruleChain, value);  
            uc1601(msg, TRUE);  
            UF_free(value);  
        }  
        UF_CALL(UF_KF_delete_instance_rule("root:", ruleName));  
        UF_CALL(UF_MODL_update());  
    }  
    /*****************************************************************************  
    **  Activation Methods  
    *****************************************************************************/  
    /*  Explicit Activation  
    **      This entry point is used to activate the application explicitly, as in  
    **      "File->Execute UG/Open->User Function..." */  
    extern DllExport void ufusr( char *parm, int *returnCode, int rlen )  
    {  
        /* Initialize the API environment 译:初始化API环境 */  
        if (!UF_CALL(UF_initialize()))  
        {  
            /* do the tasks 译:执行任务 */  
            do_it();  
            /* Terminate the API environment 译:终止API环境 */  
            UF_CALL(UF_terminate());  
        }  
    }  
    /*****************************************************************************  
    **  Utilities  
    *****************************************************************************/  
    /* Unload Handler  
    **     This function specifies when to unload your application from Unigraphics.  
    **     If your application registers a callback (from a MenuScript item or a  
    **     User Defined Object for example), this function MUST return  
    **     "UF_UNLOAD_UG_TERMINATE". */  
    extern int ufusr_ask_unload( void )  
    {  
        return( UF_UNLOAD_IMMEDIATELY );  
    }

```

#### 代码解析

> 这段代码是一个NX二次开发示例，主要实现了以下功能：
>
> 1. 包含了必要的NX API头文件，如uf.h, uf_kf.h等。
> 2. 定义了ECHO和report_error两个辅助函数，用于在窗口和日志中输出信息，以及在发生错误时报告错误。
> 3. do_it函数实现了主要功能：
>
> 1. ufusr函数是程序的入口点，用于初始化NX API环境，调用do_it函数，然后终止NX API环境。
> 2. ufusr_ask_unload函数指定了何时卸载应用程序，在这个示例中，我们选择立即卸载。
>
> 总的来说，这段代码通过Knowledge Fusion规则，实现了在NX部件发生外部更改时，自动更新其更新状态的功能，为后续操作提供了基础。
>
