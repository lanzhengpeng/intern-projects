### 【1541】list promotion occurrences 列出提升实例

#### 代码

```cpp
    /*HEAD LIST_PROMOTION_OCCURRENCES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的函数。该函数用于打印系统日志。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static tag_t ask_next_promotion(tag_t part, tag_t obj)  
    {  
        int  
            subtype,  
            type;  
        while ((obj = UF_OBJ_cycle_all(part, obj)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(obj, &type, &subtype));  
            if ((type == UF_solid_type) && (subtype == UF_solid_body_subtype) &&  
                UF_OBJ_is_object_a_promotion(obj)) return obj;  
        }  
        return NULL_TAG;  
    }  
    static tag_t ask_occurrence_of_promotion(tag_t prom_object)  
    {  
        int  
            ii,  
            np;  
        tag_t  
            part_occ = NULL_TAG,  
           *path;  
        UF_CALL(UF_MODL_ask_promotion_path(prom_object, TRUE, &path, &np));  
        for (ii = np - 1; ii >= 0; ii--)  
            part_occ = UF_ASSEM_ask_part_occ_of_inst(part_occ, path[ii]);  
        UF_free(path);  
        return (UF_ASSEM_find_occurrence(part_occ, prom_object));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part(),  
            prom_body = NULL_TAG;  
        while ((prom_body = ask_next_promotion(part, prom_body)) != NULL_TAG)  
            WRITE_D(ask_occurrence_of_promotion(prom_body));  
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

> 这段代码是NX的二次开发示例代码，其主要功能是遍历当前显示的零件中所有提升体，并获取它们的实例路径。
>
> 主要函数及功能如下：
>
> 1. report_error()：报告错误信息，用于记录函数调用失败时的错误码和错误信息。
> 2. write_integer_to_listing_window()：将整数写入列表窗口，用于记录提升体的实例路径。
> 3. ask_next_promotion()：遍历零件中的提升体，并返回下一个提升体对象。
> 4. ask_occurrence_of_promotion()：获取提升体在装配中的实例路径，并返回提升体在装配中的实例标识。
> 5. do_it()：主函数，遍历当前显示的零件中的所有提升体，并打印它们的实例路径。
> 6. ufusr()：NX的二次开发入口函数，初始化NX环境，调用do_it()函数，然后终止NX环境。
> 7. ufusr_ask_unload()：询问是否立即卸载，返回立即卸载标志。
>
> 总体来说，该代码通过遍历零件中的提升体，并获取每个提升体在装配中的实例路径，实现了提升体实例的查询功能。
>
