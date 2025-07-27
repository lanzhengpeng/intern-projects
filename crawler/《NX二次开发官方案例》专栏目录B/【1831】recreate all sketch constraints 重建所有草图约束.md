### 【1831】recreate all sketch constraints 重建所有草图约束

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_sket.h>  
    #include <uf_sket_types.h>  
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
    /* ARGSUSED 里海译:翻译ARGSUSED为“参数未使用”。

该宏常用于C语言中，表示函数或方法的参数虽然声明但未在函数体中使用。这可以避免编译器警告关于未使用的参数。 */  
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
    /* ARGSUSED 里海译:翻译ARGSUSED为“参数未使用”。 */  
    static void do_it(void)  
    {  
        int  
            geom_count,  
            ii,  
            n_cons;  
        tag_t  
            *cons,  
            junk,  
            part = UF_PART_ask_display_part(),  
            sketch = NULL_TAG;  
        char  
            sket_name[UF_OBJ_NAME_LEN];  
        UF_SKET_con_type_t  
            con_type;  
        UF_SKET_con_geom_t  
            *con_geoms;  
        UF_SKET_con_class_t  
            con_class = UF_SKET_geo_cons;  
        while ((sketch = ask_next_new_sketch(part, sketch)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_name(sketch, sket_name));  
            UF_CALL(UF_SKET_initialize_sketch(sket_name, &junk));  
            UF_CALL(UF_SKET_ask_constraints_of_sketch(sketch, con_class,  
                                                          &n_cons, &cons));  
            for (ii = 0; ii < n_cons; ii++)  
            {  
                UF_CALL(UF_SKET_read_geometric_constraint(sketch, cons[ii],  
                                        &con_type, &geom_count, &con_geoms));  
                UF_CALL(UF_SKET_delete_constraints(1, &cons[ii]));  
                UF_CALL(UF_SKET_create_geometric_constraint(sketch, con_type,  
                                               geom_count, con_geoms, &junk));  
                UF_free(con_geoms);  
            }  
            UF_free(cons);  
            UF_CALL(UF_SKET_terminate_sketch());  
        }  
    }  
    /* ARGSUSED 里海译:根据文中内容，ARGSUSED 的翻译是“参数未使用”。 */  
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

> 这是一段用于NX二次开发的代码，主要实现了以下功能：
>
> 1. 定义了一个ECHO函数，用于打印错误信息到列表窗口和系统日志。
> 2. 定义了一个report_error函数，用于报告函数调用失败时的错误信息。
> 3. 定义了一个ask_next_new_sketch函数，用于遍历当前零件中的所有草图，并找到下一个新版本的草图。
> 4. 定义了一个do_it函数，遍历当前零件中的所有草图，对每个草图进行以下操作：初始化草图获取草图的所有几何约束删除草图的所有几何约束重新创建草图的所有几何约束结束草图
> 5. 初始化草图
> 6. 获取草图的所有几何约束
> 7. 删除草图的所有几何约束
> 8. 重新创建草图的所有几何约束
> 9. 结束草图
> 10. 定义了一个ufusr函数，这是NX二次开发程序的入口函数。在这个函数中，初始化NX系统，调用do_it函数，然后终止NX系统。
> 11. 定义了一个ufusr_ask_unload函数，返回立即卸载二次开发程序的标志。
>
> 整体来说，这段代码的主要目的是遍历当前零件中的所有草图，对每个草图执行删除并重新创建所有几何约束的操作。
>
