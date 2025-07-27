### 【1000】delete a category 删除一个类别

#### 代码

```cpp
    /*HEAD DELETE_A_CATEGORY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的函数。该函数用于在系统日志中打印信息。需要注意的是，目前只提供函数名翻译，不提供额外的说明或评论。 */  
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
    /* qq3123197280 */  
    static tag_t ask_next_of_name_and_type(tag_t part, char *name, int type,  
        tag_t object)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_by_name_and_type(part, name, type, TRUE,  
            &object))) return (object);  
        return (NULL_TAG);  
    }  
    /* qq3123197280 */  
    static void delete_a_category(char *cat_name)  
    {  
        tag_t  
            cat = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        cat = ask_next_of_name_and_type(part, cat_name,   
                                        UF_layer_category_type, cat);  
        if (cat != NULL_TAG) UF_CALL(UF_OBJ_delete_object(cat));  
    }  
    static void do_it(void)  
    {  
        delete_a_category("JUNK");  
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

> 这段代码是用于NX Open C++ API的二次开发代码，主要实现了删除指定名称的图层分类的功能。以下是代码的主要功能：
>
> 1. 函数定义：report_error: 用于在出错时打印错误信息。ask_next_of_name_and_type: 循环查询指定名称和类型的对象，返回第一个查询到的对象。delete_a_category: 删除指定名称的图层分类。do_it: 调用delete_a_category函数，删除名称为"JUNK"的图层分类。ufusr: NX Open的入口函数，初始化后调用do_it函数，最后终止NX Open。ufusr_ask_unload: 插件卸载时立即卸载。
> 2. report_error: 用于在出错时打印错误信息。
> 3. ask_next_of_name_and_type: 循环查询指定名称和类型的对象，返回第一个查询到的对象。
> 4. delete_a_category: 删除指定名称的图层分类。
> 5. do_it: 调用delete_a_category函数，删除名称为"JUNK"的图层分类。
> 6. ufusr: NX Open的入口函数，初始化后调用do_it函数，最后终止NX Open。
> 7. ufusr_ask_unload: 插件卸载时立即卸载。
> 8. 删除指定图层分类：在delete_a_category函数中，通过UF_PART_ask_display_part获取当前显示的部件。使用ask_next_of_name_and_type函数循环查询名称为参数cat_name的图层分类对象。如果查询到，则调用UF_OBJ_delete_object删除该图层分类对象。
> 9. 在delete_a_category函数中，通过UF_PART_ask_display_part获取当前显示的部件。
> 10. 使用ask_next_of_name_and_type函数循环查询名称为参数cat_name的图层分类对象。
> 11. 如果查询到，则调用UF_OBJ_delete_object删除该图层分类对象。
> 12. 错误处理：在report_error函数中，使用UF_get_fail_message获取错误信息，并使用UF_print_syslog打印到系统日志。在ask_next_of_name_and_type和UF_OBJ_delete_object调用后，通过UF_CALL宏进行错误处理。
> 13. 在report_error函数中，使用UF_get_fail_message获取错误信息，并使用UF_print_syslog打印到系统日志。
> 14. 在ask_next_of_name_and_type和UF_OBJ_delete_object调用后，通过UF_CALL宏进行错误处理。
> 15. NX Open初始化和终止：在ufusr函数中，首先调用UF_initialize进行NX Open初始化。然后调用do_it执行删除操作。最后调用UF_terminate终止NX Open。
> 16. 在ufusr函数中，首先调用UF_initialize进行NX Open初始化。
> 17. 然后调用do_it执行删除操作。
> 18. 最后调用UF_terminate终止NX Open。
>
> 综上所述，这段代码实现了通过NX Open C++ API删除指定名称的图层分类的功能。
>
