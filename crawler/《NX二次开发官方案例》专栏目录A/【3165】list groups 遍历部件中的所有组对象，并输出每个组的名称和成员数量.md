### 【3165】list groups 遍历部件中的所有组对象，并输出每个组的名称和成员数量

#### 代码

```cpp
    /*HEAD LIST_GROUPS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_group.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中的新功能。 */  
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
    static void report_group_info(tag_t object)  
    {  
        int  
            n;  
        char  
            obj_name[MAX_ENTITY_NAME_SIZE + 1];  
        tag_t  
            *members;  
        printf("group %d", object);  
        if (!UF_OBJ_ask_name(object, obj_name))  
            printf(", named %s, ", obj_name);  
        UF_CALL(UF_GROUP_ask_group_data(object, &members, &n));  
        printf(" has %d group members\n", n);  
        if (n > 0) UF_free(members);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            group = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_group_type, &group)) &&  
            (group != NULL_TAG))  
            report_group_info(group);  
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

> 这段代码是NX的二次开发代码，主要功能是遍历零件中的所有组，并打印出每个组的信息，包括组ID、名称和成员数量。
>
> 主要特点如下：
>
> 1. 包含了NX二次开发所需的头文件。
> 2. 定义了一个错误报告函数report_error，用于在函数执行出错时打印错误信息。
> 3. 定义了一个报告组信息的函数report_group_info，用于打印出组的ID、名称和成员数量。
> 4. 定义了一个主函数do_it，用于遍历零件中的所有组，并调用report_group_info函数报告每个组的信息。
> 5. 定义了ufusr函数作为程序的入口，初始化NX环境，调用do_it函数，然后终止NX环境。
> 6. 定义了ufusr_ask_unload函数，返回立即卸载，表示程序不需要在NX中保留状态。
> 7. 代码使用了NX提供的UF_CALL宏来检查函数返回码，并在出错时调用report_error函数。
> 8. 使用了NX的UF_OBJ_cycle_objs_in_part函数来遍历零件中的所有组。
> 9. 使用了NX的UF_GROUP_ask_group_data函数来获取组的成员信息。
> 10. 整体代码结构清晰，功能明确，是一个典型的NX二次开发示例。
>
