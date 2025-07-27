### 【1356】group selected objects 创建一个非唯一成员资格组，高亮显示组内的对象

#### 代码

```cpp
    /*HEAD GROUP_SELECTED_OBJECTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_group.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
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
    static int select_objects(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select Objects", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &cnt, objects));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*objects)[ii], 0));  
        return cnt;  
    }  
    static void set_highlight_group(tag_t group, int flag)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *objects;  
        UF_CALL(UF_GROUP_ask_group_data(group, &objects, &n));  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
        if (n > 0) UF_free(objects);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_objects;  
        tag_t  
            group,  
            *objects,  
            junk;  
        double  
            zero[3] = { 0,0,0 };  
        while ((n_objects = select_objects("Create Non-Unique Membership Group",  
            &objects)) > 0)  
        {  
        /*  There is no available method to create a non-unique membership  
            group, so create a group using a dummy object first, then set  
            the group to be non-unique membership, then add the selected  
            objects which might already be members of other groups.  
        */  
            UF_CALL(UF_CURVE_create_point(zero, &junk));  
            UF_CALL(UF_GROUP_create_group(&junk, 1, &group));  
            UF_CALL(UF_GROUP_set_non_unique_membership(group));  
            for (ii = 0; ii < n_objects; ii++)  
            {  
                UF_CALL(UF_GROUP_add_member_to_group(objects[ii], group));  
            }  
            UF_CALL(UF_OBJ_delete_object(junk));  
            set_highlight_group(group, TRUE);  
            uc1601("Group members highlighted", TRUE);  
            set_highlight_group(group, FALSE);  
            UF_free(objects);  
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

```

#### 代码解析

> 这段代码是一个NX的二次开发程序，其主要功能是创建一个非唯一成员组，并将用户选择的对象添加到该组中。
>
> 关键要点如下：
>
> 1. 引入了必要的NX Open API头文件。
> 2. 定义了两个宏，用于打印错误信息。
> 3. 定义了一个错误报告函数report_error，用于打印函数调用失败时的错误信息。
> 4. 定义了一个选择对象的函数select_objects，用于通过对话框让用户选择对象，并获取选中的对象数组。
> 5. 定义了一个设置组对象高亮显示的函数set_highlight_group。
> 6. 定义了主函数do_it，用于创建非唯一成员组，并添加用户选择的对象。由于NX没有直接创建非唯一成员组的接口，所以先创建一个组，然后将其设置为非唯一成员组，最后添加对象。
> 7. 定义了ufusr函数，这是程序的入口函数，用于初始化NX Open API，调用do_it函数，然后终止NX Open API。
> 8. 定义了ufusr_ask_unload函数，用于立即卸载程序。
>
> 这段代码实现了NX中创建非唯一成员组的功能，并通过对话框让用户选择要添加到组中的对象。
>
