### 【3076】update part attribute before each save 每次保存前更新零件属性

#### 代码

```cpp
    /*HEAD UPDATE_PART_ATTRIBUTE_BEFORE_EACH_SAVE CCC UFUN */  
    /*  This program demonstrates how to do something to a part each time it is  
        saved, before it is saved.  
        Simply put these lines in a .men file in a startup directory and  
        put the program update_part_attribute_before_each_save (.sl, .dll,  
        or .so) in an application directory:  
        VERSION 130  
        EDIT UG_GATEWAY_MAIN_MENUBAR  
        MODIFY  
                BUTTON UG_FILE_SAVE_PART  
                ACTIONS/PRE update_part_attribute_before_each_save  
                BUTTON UG_FILE_SAVE_AS  
                ACTIONS/PRE update_part_attribute_before_each_save  
                BUTTON UG_FILE_SAVE_ALL  
                ACTIONS/PRE update_part_attribute_before_each_save  
        END_OF_MODIFY  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_attr.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数。 */  
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
            part = UF_ASSEM_ask_work_part();  
        int  
            now[2] = { -1, -1 };  
        char  
            dstr[21],  
            mstr[MAX_LINE_SIZE+1],  
            tstr[21],  
            user_name[30];  
        UF_ATTR_value_t  
            value = { UF_ATTR_string, NULL };  
        UF_CALL(uc4595(1, user_name));  
        UF_CALL(uc4582(now, 2, dstr, tstr));  
        sprintf(mstr,"%s saved on %s at %s\n", user_name, dstr, tstr);  
        UF_CALL(UF_UI_open_listing_window());  
        UF_CALL(UF_UI_write_listing_window(mstr));  
        value.value.string = mstr;  
        UF_CALL(UF_ATTR_assign(part, "LAST_SAVE", value));  
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
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这是NX二次开发的示例代码，主要实现了在每次保存零件之前自动更新零件属性的功能。以下是代码的主要功能和实现步骤：
>
> 1. 包含所需的头文件，包括UF、UI、CFI、ATTR和ASSEM模块。
> 2. 定义了report_error函数，用于在函数调用出错时报告错误信息。
> 3. 定义了do_it函数，用于获取当前用户、时间和日期，拼接成字符串，并写入零件的"LAST_SAVE"属性中。
> 4. ufusr函数是程序的入口，调用do_it函数来实现功能。
> 5. ufusr_ask_unload函数用于设置程序的卸载模式为立即卸载。
> 6. 在men文件中通过修改保存按钮的预动作，调用该程序，从而实现每次保存前自动更新属性的功能。
>
> 该程序通过修改men文件，实现了在NX中保存零件前自动记录保存信息的功能。通过UF_ATTR模块实现属性的获取和设置，通过UF_UI模块实现用户界面的交互。
>
