### 【1964】report all sketch geometry prev13 报告所有草图几何 PREV13

#### 代码

```cpp
    /*HEAD REPORT_ALL_SKETCH_GEOMETRY_PREV13 CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_sket.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的。 */  
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
    static tag_t ask_next_sketch(tag_t part, tag_t sketch)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_sketch_type, &sketch));  
        return (sketch);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_objs,  
            subtype,  
            type;  
        tag_t  
            obj,  
            part = UF_PART_ask_display_part(),  
            sketch = NULL_TAG;  
        char  
            view_name[16];  
        uf_list_p_t  
            objs;  
        while ((sketch = ask_next_sketch(part, sketch)) != NULL_TAG)  
        {  
            UF_CALL(UF_SKET_ask_info(sketch, 5, view_name, &objs));  
            printf("Sketch %s:\n", view_name);  
            UF_CALL(UF_SKET_ask_info(sketch, 3, view_name, &objs));  
            UF_CALL(UF_MODL_ask_list_count(objs, &n_objs));  
            printf("  %d objs:\n", n_objs);  
            for (ii=0; ii<n_objs; ii++)  
            {  
                UF_CALL(UF_MODL_ask_list_item(objs, ii, &obj));  
                UF_CALL(UF_OBJ_ask_type_and_subtype(obj, &type, &subtype));  
                printf("    %d.  %d (%d/%d)\n", ii+1, obj, type, subtype);  
            }  
            UF_CALL(UF_MODL_delete_list(&objs));  
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

> 这是NX的二次开发代码，主要功能是遍历当前显示的零件中的所有草图，并打印出草图名称和草图中的所有对象。代码的主要功能如下：
>
> 1. 定义了宏UF_CALL，用于调用NX API函数，并记录错误信息。
> 2. 定义了report_error函数，用于打印错误信息。
> 3. 定义了ask_next_sketch函数，用于遍历当前零件中的所有草图，并返回下一个草图的对象句柄。
> 4. do_it函数是主函数，遍历当前零件中的所有草图，打印草图名称，然后遍历草图中的所有对象，并打印对象类型和子类型。
> 5. ufusr函数是NX的二次开发入口函数，调用do_it函数实现功能，并在最后调用UF_terminate终止NX的初始化。
> 6. ufusr_ask_unload函数用于立即卸载二次开发，以便释放资源。
>
> 总的来说，这段代码实现了遍历零件中的所有草图，并打印出草图名称和草图中的所有对象的功能，提供了对NX草图对象进行遍历和查询的示例。
>
