### 【2458】return list of selected locations to kf 返回选择的地点列表给KF

#### 代码

```cpp
    /*HEAD RETURN_LIST_OF_SELECTED_LOCATIONS_TO_KF CCC UFUN */  
    /*  This program demonstrates using Open C API to prompt the user for  
        multiple locations and return a list of Points to Knowledge Fusion.  
        The corresponding dfa file defining the KF function would look like  
        this:  
        #! UGNX/KF 2.0  
        #+  
            This function will return a list of user specified Point locations.  
            Input : the prompt for the Point Constructor dialog.  
            Return: list of selected points  
        #-  
        Defun: prompt_forLocations (string $prompt,  
                       String ($libname, "/path/to/this.sl_so_dll"),  
                       String ($funcname, "prompt_forLocations") )  
        @{ Cfunc("UF_KF_invoke_user_function_dialog"; "libufun"); } list ;  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_kf.h>  
    #include <uf_error_bases.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据提供的信息，UF_print_syslog是在V18版本中新增的函数。以下是关于该函数的翻译：

注意：UF_print_syslog是V18版本新增的函数，请仅提供翻译，不要添加任何废话。

如果您需要进一步了解该函数的详细信息，请提供更多上下文，以便我能提供更准确的翻译。 */  
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
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
        else return FALSE;  
    }  
    /* qq3123197280 */  
    extern DllExport void prompt_forLocations(  
        UF_KF_value_t *data,  
        int num,  
        UF_KF_value_t *rv)  
    {  
        double  
            pos[3];  
        char  
            *prompt;  
        UF_KF_value_p_t  
            value;  
        UF_KF_list_p_t  
            results = NULL;  
        *rv = NULL;  
        if (UF_CALL(UF_initialize())) return;  
        if (!UF_CALL(UF_KF_ask_string(data[0], &prompt)))  
            while (specify_point(prompt, pos))  
            {  
                UF_CALL(UF_KF_make_point(pos, &value));  
                UF_CALL(UF_KF_list_push(results, value, &results));  
            }  
        UF_CALL(UF_KF_make_list(results, rv));  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，其主要功能是提示用户输入多个位置，并将这些位置作为一个点列表返回给知识融合。
>
> 主要步骤包括：
>
> 1. 定义了错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了specify_point函数，用于提示用户指定一个点位置，并返回指定结果。
> 3. 定义了prompt_forLocations函数，作为知识融合的函数接口。该函数会提示用户输入位置，然后调用specify_point获取每个位置，并将位置转换为点对象，最后将这些点对象组成一个列表返回。
>
> 通过知识融合调用prompt_forLocations函数，可以获取用户输入的多个点位置，实现位置输入的功能。
>
