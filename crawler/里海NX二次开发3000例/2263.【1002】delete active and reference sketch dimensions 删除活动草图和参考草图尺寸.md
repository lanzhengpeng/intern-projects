### 【1002】delete active and reference sketch dimensions 删除活动草图和参考草图尺寸

#### 代码

```cpp
    /*HEAD DELETE_ALL_SKETCH_DIMENSIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_sket.h>  
    #include <uf_part.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能，用于打印系统日志信息。 */  
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
    static tag_t ask_next_new_sketch(tag_t part, tag_t sketch)  
    {  
        int  
            subtype,  
            type;  
        while  
            (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_sketch_type, &sketch)) &&  
            (sketch != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(sketch, &type, &subtype));  
            if (subtype == UF_v13_sketch_subtype) return (sketch);  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            n_dims,  
            *statuses;  
        tag_t  
            *dims,          
            junk,  
            part = UF_PART_ask_display_part(),  
            sketch = NULL_TAG;  
        char  
            sket_name[UF_OBJ_NAME_LEN];  
        while ((sketch = ask_next_new_sketch(part, sketch)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_name(sketch, sket_name));  
            UF_CALL(UF_SKET_initialize_sketch(sket_name, &junk));  
            UF_CALL(UF_SKET_ask_dimensions_of_sketch(sketch, &n_dims, &dims));  
            UF_CALL(UF_SKET_terminate_sketch());  
            UF_CALL(UF_OBJ_delete_array_of_objects(n_dims, dims, &statuses ));  
            UF_free(dims);  
            UF_free(statuses);  
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

> 这段代码是用于删除NX模型中所有草图尺寸的二次开发代码。具体功能如下：
>
> 1. 包含了必要的NX Open头文件，如uf.h、uf_ui.h等。
> 2. 定义了ECHO和UF_CALL宏，用于打印变量值和报告UF函数调用错误。
> 3. report_error函数用于在UF函数调用出错时打印错误信息到系统日志和列表窗口。
> 4. ask_next_new_sketch函数用于遍历当前显示部件中的所有草图，并返回下一个V13草图。
> 5. do_it函数用于执行删除操作。首先初始化草图，然后获取草图中的所有尺寸，最后删除这些尺寸。
> 6. ufusr函数是NX二次开发程序的入口点。首先初始化NX Open，然后调用do_it函数执行删除操作，最后终止NX Open。
> 7. ufusr_ask_unload函数用于告诉NX立即卸载该二次开发程序。
>
> 整体来看，这段代码实现了NX模型中所有草图尺寸的批量删除功能。
>
