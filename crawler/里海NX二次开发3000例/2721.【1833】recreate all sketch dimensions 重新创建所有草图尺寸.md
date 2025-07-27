### 【1833】recreate all sketch dimensions 重新创建所有草图尺寸

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_sket.h>  
    #include <uf_part.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
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
            ii,  
            n_dims;  
        tag_t  
            *dims,  
            exp,  
            junk,  
            part = UF_PART_ask_display_part(),  
            sketch = NULL_TAG;  
        double  
            origin[3];  
        char  
            sket_name[UF_OBJ_NAME_LEN];  
        UF_SKET_con_type_t  
            dim_type;  
        UF_SKET_dim_object_t  
            object1,  
            object2;  
        while ((sketch = ask_next_new_sketch(part, sketch)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_name(sketch, sket_name));  
            UF_CALL(UF_SKET_initialize_sketch(sket_name, &junk));  
            uc1601("Before", TRUE);  
            UF_CALL(UF_SKET_ask_dimensions_of_sketch(sketch, &n_dims, &dims));  
            for (ii = 0; ii < n_dims; ii++)  
            {  
                UF_CALL(UF_SKET_read_dimension(sketch, dims[ii], &dim_type,  
                    &object1, &object2, origin, &exp));  
                UF_CALL(UF_SKET_delete_dimensions(1, &dims[ii]));  
                UF_CALL(UF_SKET_create_dimension(sketch, dim_type,  
                    &object1, &object2, origin, &junk));  
            }  
            UF_free(dims);  
            UF_CALL(UF_DISP_regenerate_display());  
            uc1601("After", TRUE);  
            UF_CALL(UF_SKET_terminate_sketch());  
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

> 这段代码是用于NX的二次开发，主要实现以下功能：
>
> 1. 定义了ECHO和UF_CALL两个宏，用于输出错误信息和调用NX API函数。
> 2. ask_next_new_sketch函数用于遍历当前部件中的所有草图，找到第一个V13版本的草图并返回其tag。
> 3. do_it函数是主函数，遍历当前部件中的所有草图，对每个草图执行以下操作：初始化草图。获取草图的所有尺寸。遍历所有尺寸，先删除再重建，从而更新草图。释放尺寸数组。重新生成显示。结束草图。
> 4. 初始化草图。
> 5. 获取草图的所有尺寸。
> 6. 遍历所有尺寸，先删除再重建，从而更新草图。
> 7. 释放尺寸数组。
> 8. 重新生成显示。
> 9. 结束草图。
> 10. ufusr是NX调用的主函数，在初始化NX后调用do_it函数，最后终止NX。
> 11. ufusr_ask_unload函数返回立即卸载，表示二次开发应用程序可以被立即卸载。
>
> 综上所述，这段代码主要用于遍历当前部件中的所有草图，对每个草图执行尺寸的删除和重建操作，从而实现草图尺寸的更新。
>
