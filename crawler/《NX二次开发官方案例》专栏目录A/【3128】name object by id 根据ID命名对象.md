### 【3128】name object by id 根据ID命名对象

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_assem.h>  
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
    static logical prompt_for_an_integer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        int  
            da[1];  
        strncpy(&menu[0][0], item, 15);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static tag_t find_object_by_id(int id)  
    {  
        unsigned int  
            sub_id,  
            ver;  
        tag_t  
            obj = NULL_TAG,  
            part = UF_ASSEM_ask_work_part();  
        char  
            *file_data,  
            *handle;  
        do  
        {  
            obj = UF_OBJ_cycle_all(part, obj);  
            if (obj == NULL_TAG) return NULL_TAG;  
            handle = UF_TAG_ask_handle_of_tag(obj);  
            UF_CALL(UF_TAG_decompose_handle(handle, &file_data, &sub_id, &ver));  
            UF_free(handle);  
            UF_free(file_data);  
            if (sub_id == id) return obj;  
        } while (obj != NULL_TAG);  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            id = 0;  
        tag_t  
            obj;  
        char  
            name[MAX_ENTITY_NAME_SIZE+1];  
        while (prompt_for_an_integer("Name object", "ID", &id))  
        {  
            obj = find_object_by_id(id);  
            if (obj != NULL_TAG)  
            {  
                sprintf(name, "%d", id);  
                UF_CALL(UF_OBJ_set_name(obj, name));  
            }  
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

> 这段代码是一个NX Open的二次开发示例，主要实现了以下功能：
>
> 1. 使用UF_initialize()初始化NX Open，使用UF_terminate()结束NX Open。
> 2. 定义了一个错误报告函数report_error()，用于输出错误信息到日志窗口。
> 3. 定义了一个提示输入整数函数prompt_for_an_integer()，用于获取用户输入的整数。
> 4. 定义了一个根据ID查找对象的函数find_object_by_id()，用于在当前部件中循环查找指定ID的对象。
> 5. 定义了一个执行主体函数do_it()，循环提示用户输入对象ID，然后查找对应对象并设置名称。
> 6. ufusr()是NX Open的入口函数，调用do_it()执行主体功能。
> 7. ufusr_ask_unload()定义了卸载函数，返回立即卸载。
>
> 总体来说，这是一个简单的二次开发示例，实现了根据ID查找对象并设置名称的功能。代码结构清晰，错误处理完善，具有一定的参考价值。
>
