### 【2727】set all smart points visible 将所有智能点设置为可见

#### 代码

```cpp
    /*HEAD SET_ALL_SMART_POINTS_VISIBLE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_so.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译如下：

注意：UF_print_syslog 是在 V18 中新增的，请只提供翻译，不要添加任何额外内容。 */  
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
    static tag_t ask_next_smart_point(tag_t part, tag_t point)  
    {  
        logical  
            is_so;  
        int  
            subtype,  
            type;  
        /*  
            Note: UF_OBJ_cycle_objs_in_part will not find invisible smart  
            points so UF_OBJ_cycle_all is used here instead  
        */  
        while ((point = UF_OBJ_cycle_all(part, point)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(point, &type, &subtype));  
            if (type == UF_point_type)  
            {  
                UF_CALL(UF_SO_is_so(point, &is_so));  
                if (is_so) return point;  
            }  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part(),  
            point = NULL_TAG;  
        while ((point = ask_next_smart_point(part, point)) != NULL_TAG)  
            UF_CALL(UF_SO_set_visibility_option(point, UF_SO_visible));  
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

> 这段代码是NX二次开发的示例，其主要功能是设置NX部件中的所有智能点可见。以下是代码的详细说明：
>
> 1. 包含头文件：代码包含了必要的NX API头文件，如uf.h、uf_ui.h等。
> 2. 错误处理函数：定义了一个错误处理函数report_error，用于在函数调用失败时输出错误信息。
> 3. 获取下一个智能点：定义了一个函数ask_next_smart_point，用于遍历部件中的所有点，并返回下一个智能点。这里使用了UF_OBJ_cycle_all而不是UF_OBJ_cycle_objs_in_part，因为后者不会返回不可见的智能点。
> 4. 设置所有智能点可见：定义了一个函数do_it，用于遍历所有智能点，并调用UF_SO_set_visibility_option将其设置为可见。
> 5. ufusr函数：这是NX二次开发的主入口函数。首先调用UF_initialize进行初始化，然后调用do_it设置所有智能点可见，最后调用UF_terminate终止。
> 6. 卸载函数：定义了卸载函数ufusr_ask_unload，返回UF_UNLOAD_IMMEDIATELY表示立即卸载。
>
> 综上所述，这段代码实现了遍历当前显示的NX部件，找出所有智能点并将其设置为可见的功能。
>
