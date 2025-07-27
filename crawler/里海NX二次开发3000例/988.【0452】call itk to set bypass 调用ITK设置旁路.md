### 【0452】call itk to set bypass 调用ITK设置旁路

#### 代码

```cpp
    /*  
     This should be used with the related ITK program:  
      Sample ITK program : set bypass from open c  
     See the comments in that file for more complete information.  
     This method has been obsoleted by the NXOpen::PDM::SessionSettings class.  For example to turn on bypass from a C/C++ application:  
      PDM::SessionSettings *sst = theSession->NewDatabaseSessionOptions();  
      sst->SetAdministrationBypass(true);  
      sst->Apply();  
      delete sst;  // Necessary for all classes inheriting NXOpen::TransientObject  
    */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ugmgr.h>  
    #define SET_BYPASS_ON    1  
    #define SET_BYPASS_OFF   2  
    #define REPORT_BYPASS    3  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
                      UF_UI_write_listing_window(X); \  
                      UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    int main(int argc, char *argv[])  
    {  
        int  
            output_code;  
        char  
            *input_string = { "" },  
            *output_string = NULL;  
        UF_CALL(UF_UGMGR_initialize(argc,(const char**)argv));  
        UF_CALL(UF_UGMGR_invoke_pdm_server(SET_BYPASS_ON, input_string,  
                                           &output_code, &output_string));  
        UF_CALL(UF_UGMGR_invoke_pdm_server(REPORT_BYPASS, input_string,  
                                           &output_code, &output_string));  
        if (output_string != NULL) UF_free(output_string);  
        return 0;  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是用于NX的二次开发，其主要功能包括：
>
> 1. 包含必要的头文件，如UF.h和UF_UI.h。
> 2. 定义了SET_BYPASS_ON、SET_BYPASS_OFF和REPORT_BYPASS三个宏，用于设置和查询PDM的bypass状态。
> 3. 定义了ECHO和UF_CALL两个宏，用于在日志窗口打印信息和调用UF函数。
> 4. 定义了report_error函数，用于在UF函数调用失败时打印错误信息。
> 5. 在main函数中，初始化UF_UGMGR模块，并调用UF_UGMGR_invoke_pdm_server函数设置PDM的bypass状态为开启，并查询当前状态。
> 6. 在ufusr_ask_unload函数中，返回UF_UNLOAD_IMMEDIATELY，表示立即卸载NX Open库。
>
> 总体来说，这段代码主要用于设置和查询NX的PDM bypass状态，以便进行二次开发时跳过PDM检查。
>
