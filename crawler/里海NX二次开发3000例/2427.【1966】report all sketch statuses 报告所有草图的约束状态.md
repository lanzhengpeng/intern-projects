### 【1966】report all sketch statuses 报告所有草图的约束状态

#### 代码

```cpp
    /*HEAD REPORT_ALL_SKETCH_STATUSES CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_object_types.h>  
    #include <uf_sket.h>  
    #include <uf_csys.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog在V18中是新增的，请不要添加任何废话，只提供翻译。 */  
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
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
            dof;  
        tag_t  
            orig_wcs,  
            junk,  
            part = UF_PART_ask_display_part(),  
            sketch = NULL_TAG;  
        UF_SKET_status_t  
            status;  
        char  
            sketch_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_CSYS_ask_wcs(&orig_wcs));  
        WRITE_D(UF_SKET_under_constrained);  
        WRITE_D(UF_SKET_well_constrained);  
        WRITE_D(UF_SKET_over_constrained);  
        WRITE_D(UF_SKET_inconsistently_constrained);  
        WRITE_D(UF_SKET_not_evaluated);  
        UF_CALL(UF_UI_write_listing_window("\n"));  
        while ((sketch = ask_next_new_sketch(part, sketch)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_name(sketch, sketch_name));  
            WRITE_S(sketch_name);  
            UF_CALL(UF_SKET_initialize_sketch(sketch_name, &junk));  
            UF_CALL(UF_SKET_ask_sketch_status(sketch, &status, &dof));  
            WRITE_D(status);  
            WRITE_D(dof);  
            UF_CALL(UF_SKET_terminate_sketch());  
        }  
        UF_CALL(UF_CSYS_set_wcs(orig_wcs));  
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

> 这是段NX二次开发代码，主要功能是遍历零件中的所有草图，并输出每个草图的状态信息。
>
> 代码的主要步骤包括：
>
> 1. 包含必要的NX API头文件。
> 2. 定义宏ECHO和WRITE_D，用于输出变量值。
> 3. 定义宏UF_CALL，用于调用NX API函数，并检查返回码，如果出错则输出错误信息。
> 4. 定义函数report_error，用于输出错误信息。
> 5. 定义函数write_string_to_listing_window和write_integer_to_listing_window，用于输出字符串和整数到列表窗口。
> 6. 定义函数ask_next_new_sketch，用于遍历零件中的所有草图，并返回下一个新版本的草图对象。
> 7. 定义函数do_it，用于遍历零件中的所有草图，输出草图名称和状态信息。
> 8. 定义ufusr函数，这是NX二次开发程序的入口函数，用于初始化NX环境，调用do_it函数，然后终止NX环境。
> 9. 定义ufusr_ask_unload函数，用于在程序卸载时立即释放内存。
>
> 整体来说，这段代码实现了遍历零件中的所有草图，输出每个草图的状态信息的功能，包括草图名称、约束状态和自由度。这有助于开发者了解零件中草图的状态，以便进行后续处理。
>
