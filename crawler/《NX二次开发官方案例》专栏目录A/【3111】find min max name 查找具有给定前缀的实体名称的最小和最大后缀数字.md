### 【3111】find min max name 查找具有给定前缀的实体名称的最小和最大后缀数字

#### 代码

```cpp
    /*HEAD FIND_MIN_MAX_NAME CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <ctype.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static tag_t cycle_by_name_prefix(char *prefix, tag_t part, tag_t object,  
        char *fullname)  
    {  
        int  
            ii;  
     /*  make sure the name is all in upper case 里海译:确保名称全部为大写 */  
        if (object == NULL_TAG)  
            for (ii = 0; ii < strlen(prefix); ii++)  
                prefix[ii] = toupper(prefix[ii]);  
        while ((object = UF_OBJ_cycle_all(part, object)) != NULL_TAG)  
            if ((!UF_OBJ_ask_name(object, fullname)) &&  
                (strstr(fullname, prefix) == fullname))  
                break;  
        return object;  
    }  
    static void ask_name_min_max_suffix(char *prefix, int *min, int *max)  
    {  
        int  
            n;  
        tag_t  
            object = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            fullname[MAX_ENTITY_NAME_SIZE + 1];  
        *min = 0;  
        *max = 0;  
        printf("looking for %s\n", prefix);  
        while ((object = cycle_by_name_prefix(prefix, part, object, fullname))  
            != NULL_TAG)  
        {  
            printf("found %s\n", fullname);  
            n = atoi(&fullname[strlen(prefix)]);  
            if ((*min == 0) || (n < *min)) *min = n;  
            if (n > *max) *max = n;  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            min,  
            max;  
        char  
            prefix[MAX_ENTITY_NAME_SIZE + 1] = { "item-" };  
        while (prompt_for_text("Enter name prefix", prefix))  
        {  
            ask_name_min_max_suffix(prefix, &min, &max);  
            printf("Found %s%d to %s%d\n", prefix, min, prefix, max);  
        }  
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

> 这段NX二次开发代码主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用失败时输出错误信息和调用栈。
> 2. 定义了一个提示用户输入文本的函数prompt_for_text，通过调用NX API获取用户输入。
> 3. 定义了一个遍历具有指定前缀名称的对象的函数cycle_by_name_prefix，通过NX API遍历所有对象，并返回匹配前缀的对象。
> 4. 定义了一个查询名称范围最小和最大编号的函数ask_name_min_max_suffix，用于遍历对象并统计最小和最大编号。
> 5. 主函数do_it循环提示用户输入前缀，并调用ask_name_min_max_suffix函数输出对象的最小和最大编号。
> 6. 定义了UF初始化和终止的函数ufusr。
> 7. 定义了卸载函数ufusr_ask_unload。
>
> 总体来说，这段代码通过遍历和匹配名称前缀，实现了查询NX部件库中具有指定前缀的对象的编号范围的功能，并提示用户输入前缀，输出匹配对象的编号范围。
>
