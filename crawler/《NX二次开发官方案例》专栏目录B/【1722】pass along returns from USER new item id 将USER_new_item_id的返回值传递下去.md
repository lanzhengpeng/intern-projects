### 【1722】pass along returns from USER new item id 将USER_new_item_id的返回值传递下去

#### 代码

```cpp
    /*  When using the Assign button in NX Manager mode, if the ITK function  
        registered as USER_new_item_id returns a NULL for the Number, NX  
        will raise an error "Input for part number was invalid."  See PR  
        6332971.  
        This code demonstrates a work around.  When it acts as the "middle  
        man" and returns the values from USER_new_item_id, the errro does  
        not occur.  
    */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_exit.h>  
    #include <uf_ugmgr.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
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
    DllExport int MY_assign(UF_UGMGR_new_part_no_p_t assign_data)  
    {  
        logical  
            modifiable;  
        char  
            part_no[UF_UGMGR_PARTNO_SIZE+1];  
        UF_CALL(UF_UGMGR_assign_part_number(assign_data->old_item,  
            assign_data->item_type, part_no, &modifiable));  
        assign_data->part_number_modifiable = modifiable;  
        strcpy(assign_data->new_id, part_no);  
        assign_data->part_name_modifiable = TRUE;  
        strcpy(assign_data->new_name, "");  
        assign_data->part_description_modifiable = TRUE;  
        strcpy(assign_data->new_description, "");  
        assign_data->ifail = 0;  
        return 0;  
    }  
    static void do_it(void)  
    {  
        ECHO("Registering MY_assign\n", FALSE);  
        UF_CALL(UF_UGMGR_reg_new_part_no(MY_assign));  
    }  
    /* qq3123197280 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_SEL_DIALOG);  
    }

```

#### 代码解析

> 根据代码，可以总结如下：
>
> 1. 功能介绍：该代码实现了NX Manager模式下的部件编号分配功能。通过注册回调函数MY_assign，在NX尝试分配新部件编号时，该函数会被调用，从而实现自定义的分配逻辑。
> 2. 主要函数：其中，MY_assign函数是核心函数，实现了自定义的部件编号分配逻辑，包括获取旧的部件编号、设置新编号是否可修改、设置新名称和描述等。
> 3. 错误处理：通过定义UF_CALL宏，实现了错误码的获取和打印，便于调试。
> 4. NX初始化和终止：在ufsta和ufusr函数中实现了NX的初始化和终止。
> 5. 卸载提示：ufusr_ask_unload函数用于提示是否卸载NX。
> 6. 自定义分配逻辑：通过自定义分配函数MY_assign，避免了直接调用NX的分配函数可能出现的错误，从而实现了自定义的分配逻辑。
> 7. 调试信息输出：使用ECHO函数，通过UF_listing_window和UF_print_syslog输出调试信息，方便调试。
> 8. NX二次开发标准流程：整个代码遵循NX二次开发的标准流程，包括初始化、实现功能、输出信息、终止等。
>
> 综上所述，该代码通过自定义分配函数，实现了NX Manager模式下部件编号的分配，并遵循NX二次开发的标准流程。
>
