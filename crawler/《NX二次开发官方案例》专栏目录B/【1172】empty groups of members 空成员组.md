### 【1172】empty groups of members 空成员组

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_draw.h>  
    #include <uf_draw_types.h>  
    #include <uf_drf.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 中新增的函数，用于打印系统日志。在 V18 之前的版本中，打印系统日志需要使用 uf_print 方法，而 V18 引入了专门用于打印系统日志的 UF_print_syslog 函数。 */  
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
        int  
            member_count = 0,  
            ii = 0;  
        tag_t  
            *member_list = NULL_TAG,  
            part = UF_PART_ask_display_part(),  
            object = NULL_TAG;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_group_type, &object)) &&  
             (object != NULL_TAG))  
        {  
            UF_CALL(UF_GROUP_ask_group_data(object, &member_list, &member_count));  
            for (ii = 0; ii < member_count; ++ii)  
            {  
                UF_GROUP_del_member_from_group(member_list[ii], NULL_TAG);  
            }  
            if (member_count > 0) UF_free(member_list);  
            printf("Items from Group of tag: %d are now independent objects\n", object);  
        }  
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
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(status->file_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    /*  To run in external native mode UG:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external UG/Manager or IMAN mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                printf("%d.  %s\n", ++cnt, part_name);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                report_load_status(&status);  
                if (!status.failed)  
                {  
                    do_it();  
                    if (!UF_CALL(UF_PART_save()))  
                        printf("\t\'%s\' Saved After Ungrouping\n", part_name);  
                    UF_CALL(UF_PART_close_all());  
                }  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个用于NX二次开发的示例，主要功能是遍历零件中的所有组，并将组中的成员移除，使它们成为独立的对象。
>
> 主要步骤如下：
>
> 1. 包含必要的NX API头文件，并定义了一个错误报告函数report_error，用于在调用NX API出错时打印错误信息。
> 2. 定义了一个do_it函数，用于遍历零件中的所有组，并删除组中的成员。
> 3. 定义了ufusr函数作为NX的入口函数，初始化NX环境后调用do_it函数，最后终止NX环境。
> 4. 定义了ufusr_ask_unload函数，返回立即卸载标志。
> 5. 定义了report_load_status函数，用于报告零件加载的状态信息。
> 6. 在main函数中，首先初始化NX环境，然后调用uc4624函数处理命令行参数，并遍历所有零件名称，逐个打开零件，调用report_load_status报告加载状态，如果加载成功则调用do_it函数，删除所有组，然后保存并关闭零件。
> 7. 最后打印处理了多少个零件，然后终止NX环境。
>
> 总体来说，这段代码通过遍历零件中的所有组，将组中的成员移除，实现了解组的功能，为NX的二次开发提供了参考。
>
