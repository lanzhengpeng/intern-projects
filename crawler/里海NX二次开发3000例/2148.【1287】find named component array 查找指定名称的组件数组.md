### 【1287】find named component array 查找指定名称的组件数组

#### 代码

```cpp
    /*HEAD FIND_NAMED_COMPONENT_ARRAY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #define ECHO(X)    (printf("\t%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的函数，用于打印系统日志消息。具体来说，这个函数会在发送系统日志消息时被调用，并在日志文件中打印出该消息。因此，UF_print_syslog是V18版本新增的用于打印系统日志消息的函数。 */  
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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static tag_t ask_named_component_array(char *name)  
    {  
        int  
            ii,  
            n,  
            type;  
        tag_t  
            *arrays,  
            *comps,  
            found = NULL_TAG;  
        UF_ASSEM_iset_array_data_t  
            iset_data;  
        UF_ASSEM_mc_array_data_t  
            mc_data;  
        UF_ASSEM_ask_arrays_in_part(UF_PART_ask_display_part(), &n, &arrays);  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_ASSEM_ask_type_of_array(arrays[ii], &type));  
            if (type == UF_ASSEM_ISET_ARRAY)  
            {  
                UF_CALL(UF_ASSEM_ask_iset_array_data(arrays[ii],&iset_data,&comps));  
                if (!strcmp(iset_data.array_name, name)) break;  
            }  
            else  
            {  
                UF_CALL(UF_ASSEM_ask_mc_array_data(arrays[ii], &mc_data, &comps));  
                if (!strcmp(mc_data.array_name, name)) break;  
            }  
            UF_free(comps);  
        }  
        if (ii < n) found = arrays[ii];  
        UF_free(arrays);  
        return found;  
    }  
    static void do_it(void)  
    {  
        char  
            name[MAX_ENTITY_NAME_SIZE+1];  
        while (prompt_for_text("Enter component array name", name))  
            ECHO(ask_named_component_array(name));  
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

> 这段代码是NX Open C++ API编写的NX二次开发代码，主要功能是查找并输出用户指定的组件数组名。下面是该代码的主要组成部分：
>
> 1. 包含头文件：包括NX Open C++ API的头文件，如uf.h、uf_ui.h、uf_assem.h等。
> 2. 定义宏ECHO和UF_CALL：ECHO用于打印变量名和值，UF_CALL用于调用NX API函数，并在出错时打印错误信息。
> 3. 定义report_error函数：用于报告API调用错误，并打印错误信息。
> 4. 定义prompt_for_text函数：用于弹出一个对话框，让用户输入组件数组名。
> 5. 定义ask_named_component_array函数：用于在当前部件中查找用户输入的组件数组名，并返回数组标签。
> 6. 定义do_it函数：循环调用prompt_for_text和ask_named_component_array，直到用户取消。
> 7. ufusr函数：程序的入口函数，初始化NX环境，调用do_it，然后终止NX环境。
> 8. ufusr_ask_unload函数：用于卸载二次开发程序，返回立即卸载。
>
> 总体而言，这段代码通过NX Open C++ API实现了与用户交互，并在NX部件中查找指定的组件数组的功能。
>
