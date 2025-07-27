### 【0928】create structures scenario part with user defined name 创建自定义名称的结构场景部分

#### 代码

```cpp
    /*HEAD CREATE_STRUCTURES_SCENARIO_PART_WITH_USER_DEFINED_NAME CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_vec.h>  
    #include <uf_trns.h>  
    #include <uf_view.h>  
    #include <uf_sf.h>  
    #include <uf_mech.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 版本新增的函数。 */  
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
    static void do_it(void)  
    {  
        char  
            scenario_name[25]={"scenario_1"};  
        tag_t  
            new_scenario_tag=NULL_TAG,  
            part = UF_PART_ask_display_part();  
        prompt_for_text("Enter name for new scenario part", scenario_name);  
        UF_CALL(UF_SF_create_scenario (scenario_name, &new_scenario_tag ));  
        UF_CALL(UF_PART_save());       
        UF_CALL(UF_PART_close(new_scenario_tag, 0, 1));  
        UF_CALL(UF_PART_set_display_part(part));  
        UF_CALL(UF_PART_save());  
        /*UF_CALL(UF_SF_open_scenario(scenario_name, part));*/ /*UF_SF_open_scenario   
                                                                - new function in NX 1*/  
        uc1601("Click on 'Application -> Structures'", TRUE);  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX的二次开发程序，主要功能是创建一个用户定义名称的结构场景部分。具体来说，代码的主要功能如下：
>
> 1. 引入NX提供的头文件，包含各种UF函数接口。
> 2. 定义错误报告函数report_error，用于在UF函数调用失败时输出错误信息。
> 3. 定义提示输入文本的函数prompt_for_text。
> 4. 定义执行主要功能的函数do_it，其中：获取当前显示的部件提示用户输入新场景部分的名称调用UF_SF_create_scenario创建新场景保存当前部件关闭新创建的场景恢复显示原来的部件保存当前部件
> 5. 获取当前显示的部件
> 6. 提示用户输入新场景部分的名称
> 7. 调用UF_SF_create_scenario创建新场景
> 8. 保存当前部件
> 9. 关闭新创建的场景
> 10. 恢复显示原来的部件
> 11. 保存当前部件
> 12. 定义主函数ufusr，初始化UF系统后执行do_it，然后终止UF系统。
> 13. 定义卸载函数ufusr_ask_unload，用于立即卸载当前程序。
>
> 通过以上函数，程序实现了在NX中创建一个用户指定名称的结构场景部分的功能。
>
